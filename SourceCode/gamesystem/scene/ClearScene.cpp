#include "ClearScene.h"
#include "ImageManager.h"
#include <SceneManager.h>
#include <Helper.h>
#include <GameStateManager.h>
#include <StageBack.h>
//初期化
void ClearScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	sprite = IKESprite::Create(ImageManager::GAMECLEARBACK, { 0.0f,0.0f });
	noDeath = IKESprite::Create(ImageManager::NODEATH, { 0.0f,0.0f });
	noDeath->SetAnchorPoint({0.5f,0.5f});
	noDamage = IKESprite::Create(ImageManager::NODAMAGE, { 0.0f,0.0f });
	noDamage->SetAnchorPoint({ 0.5f,0.5f });
	Audio::GetInstance()->StopWave(AUDIO_MAIN);
	for (int i = 0; i < 3; i++) {
		RandShineInit();
	}
	std::string BaseName = "Resources/csv/EnemySpawn/Clear/ClearMap.csv";
	GameStateManager::GetInstance()->SetEnemySpawnText(BaseName);

	player_ = make_unique<Player>();
	player_->LoadResource();
	player_->InitState({ -0.75f,0.1f,-1.5f });
	player_->Initialize();
	//敵
	EnemyManager::SetPlayer(player_.get());
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->SetClear(true);
	enemyManager->Initialize();

	//ステージパネルの初期化
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->SetPlayer(player_.get());
	StagePanel::GetInstance()->Initialize(0.0f);

	//丸影のためのやつ
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	for (int i = 0; i < SPOT_NUM; i++) {
		lightGroup->SetPointLightActive(i, true);
	}

	pointLightPos[0] = { 1.5,  1, 1 };
	pointLightPos[1] = { -1.5, 1, 1 };

	pointLightColor[0] = { 1.0f, 1.0f, 1.0f };
	pointLightColor[1] = { 1.0f, 1.0f, 1.0f };
	
	pointLightAtten[0] = { 2.0f, 4.0f, 4.0f };
	pointLightAtten[1] = { 2.0f, 4.0f, 4.0f };

	pointLightPower[0] = { 20.0f, 20.0f, 20.0f };
	pointLightPower[1] = { 20.0f, 20.0f, 20.0f };

	for (int i = 0; i < SPOT_NUM; i++) {
		m_AddAngleX[i] = (float)Helper::GetRanNum(0, 3);
		m_AddAngleZ[i] = (float)Helper::GetRanNum(0, 3);
	}

	object = make_unique<IKEObject3d>();
	object->Initialize();
	object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PANEL));
	object->SetScale({ PANEL_SIZE,0.01f,PANEL_SIZE });
	object->SetPosition({ -0.75f,0.0f,-1.5f });
	object->SetColor({ 0.5f,0.3f,0.1f,1.0f });

	onomatope = make_unique<Onomatope>();
	/// <summary>
	///	音入(エンドロール音希望)
	/// </summary>
	Audio::GetInstance()->PlayWave("Resources/Sound/SE/drumRoll.wav", 0.04f);
}
//更新
void ClearScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->B)||input->TriggerKey(DIK_SPACE)) && (!SceneChanger::GetInstance()->GetChangeStart())) {			//バトル
		SceneChanger::GetInstance()->SetChangeStart(true);
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button.wav", 0.15f);
	}

	if (SceneChanger::GetInstance()->GetChange()) {			//真っ暗になったら変わる
		SceneManager::GetInstance()->ChangeScene("TITLE");
		SceneChanger::GetInstance()->SetChange(false);
	}
	///スポットライト
	for (int i = 0; i < SPOT_NUM; i++) {
		lightGroup->SetPointLightPos(i, pointLightPos[i]);
		lightGroup->SetPointLightColor(i, pointLightColor[i]);
		lightGroup->SetPointLightAtten(i, pointLightAtten[i]);
	}
	//スポットライトの動き
	MoveSpotLight();
	
	m_AppTimer++;
	if (m_AppTimer == 340) {		//クリアのスプライトが出る
		/// <summary>
		///	音入(ライトがあったときのバンッみたいな音)
		/// </summary>
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Dodon.wav", 0.04f);
		_AppState = APP_NOTICE;
	}
	else if (m_AppTimer == 500) {	//カメラが引く
		_AppState = APP_VANISH;
		camerawork->SetClearEnd(true);
	}
	else if (m_AppTimer == 550) {		//しーん遷移
		SceneChanger::GetInstance()->SetChangeStart(true);
	}
	
	
	if (m_AppTimer == 400) {
		/// <summary>
		///	音入(祝い音敵なやつ(口笛とか拍手音みたいなやつ))
		/// </summary>
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Fin.wav", 0.04f);
	}

	if (SceneChanger::GetInstance()->GetChange()) {			//真っ暗になったら変わる
		s_PlayBGM = true;
		SceneManager::GetInstance()->ChangeScene("TITLE");
		SceneChanger::GetInstance()->SetChange(false);
	}
	
	if (m_AppTimer >= 340.0f) {
		m_ClearSpritePos.y = Ease(In, Cubic, 0.5f, m_ClearSpritePos.y, 0.0f);
		m_ClearNeoPos.y = Ease(In, Cubic, 0.5f, m_ClearNeoPos.y, 50.0f);
	}
	//オノマトペの生成
	BirthOnomatope();
	sprite->SetPosition(m_ClearSpritePos);
	 noDeath->SetPosition(m_ClearNeoPos);
	 noDamage->SetPosition({ m_ClearNeoPos.x,m_ClearNeoPos.y + 64.f });
	StagePanel::GetInstance()->Update();
	enemyManager->ClearUpdate();
	player_->ClearUpdate();
	object->Update();
	onomatope->Update();
	//ライト更新
	lightGroup->Update();
	//�e�N���X�X�V
	//カメラワーク更新
	camerawork->ClearUpdate(camera);
	ShineEffectUpdate();
	SceneChanger::GetInstance()->Update();
}
//描画
void ClearScene::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
}
//前面描画
void ClearScene::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	onomatope->Draw();
	sprite->Draw();
	if (GameStateManager::GetInstance()->GetNoDeath()) {
		noDeath->Draw();
	}
	if (GameStateManager::GetInstance()->GetNoDamage()) {
		noDamage->Draw();
	}
	/*for (ShineEffect& shine : shines) {
		shine.tex->Draw();
	}*/
	IKESprite::PostDraw();

	SceneChanger::GetInstance()->Draw();
}
//背面描画
void ClearScene::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	StageBack::GetInstance()->Draw(dxCommon);
	IKESprite::PostDraw();
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	player_->Draw(dxCommon);
	enemyManager->Draw(dxCommon);
	object->Draw();
	IKEObject3d::PostDraw();
}
//ImGui描画
void ClearScene::ImGuiDraw(DirectXCommon* dxCommon) {
}
//解放
void ClearScene::Finalize() {
}

void ClearScene::RandShineInit() {
	float posX = (float)Helper::GetRanNum(128, 1150);
	float posY = (float)Helper::GetRanNum(128, 360);
	float frame = (float)Helper::GetRanNum(30, 45);
	ShineEffect itr;
	itr.tex = IKESprite::Create(ImageManager::SHINE, { posX,posY });
	itr.tex->SetAnchorPoint({ 0.5f,0.5f });
	itr.tex->SetSize(itr.size);
	itr.kFrame = frame;
	shines.push_back(std::move(itr));
}

void ClearScene::ShineEffectUpdate() {
	for (ShineEffect& shine : shines) {
		if (Helper::FrameCheck(shine.frame, 1 / shine.kFrame)) {
			RandShineInit();
			shine.isVanish = true;
		}
		else {
			shine.size.x = Ease(Out, Quad, shine.frame, 0.f, 32.f);
			shine.size.y = Ease(Out, Quad, shine.frame, 0.f, 32.f);
			shine.tex->SetSize(shine.size);
		}
	}
	shines.remove_if([](ShineEffect& shine) {
		return shine.isVanish; });
}
//スポットライトの動き
void ClearScene::MoveSpotLight() {
	const float l_AddAngle = 5.0f;
	const float l_AddFrame = 0.5f;
	const float l_AttenFrame = 0.25f;
	const float l_PosMax = 1.5f;
	const float l_PosMin = -1.5f;

	//sin波によって上下に動く
	if (_AppState == APP_START) {
		for (int i = 0; i < SPOT_NUM; i++) {
			m_Angle[i] += (l_AddAngle - (2.0f * i));
			m_Angle2[i] = m_Angle[i] * (3.14f / 180.0f);
			
		}

		pointLightPos[0].x = (sin(m_Angle2[0] + m_AddAngleX[0]) * 2.0f + (-3.0f));
		pointLightPos[0].z = (sin(m_Angle2[0] + m_AddAngleZ[0]) * 1.0f + (3.0f));
		pointLightPos[1].x = (sin(m_Angle2[1] + m_AddAngleX[1]) * 2.0f + (1.0f));
		pointLightPos[1].z = (sin(m_Angle2[1] + m_AddAngleZ[1]) * 1.0f + (3.0f));

	}
	else if (_AppState == APP_NOTICE) {
		SpotSet(pointLightPos[0], { -0.75,{},-1.5f }, l_AddFrame);
		SpotSet(pointLightPos[1], { -0.75,{},-1.5f }, l_AddFrame);
	}
	else if (_AppState == APP_VANISH) {
		SpotSet(pointLightAtten[0], { 10.0f,{},10.0f }, l_AttenFrame);
		SpotSet(pointLightAtten[1], { 10.0f,{},10.0f }, l_AttenFrame);
		////角度
		//SpotSet(spotLightDir[0], {}, l_AddFrame);
		//SpotSet(spotLightDir[1], {}, l_AddFrame);
		////座標
		//SpotSet(spotLightPos[0], { l_PosMax,spotLightPos[0].y,l_PosMax }, l_AddFrame);
		//SpotSet(spotLightPos[1], { l_PosMax,spotLightPos[1].y,l_PosMin }, l_AddFrame);
	}
}
//スポットライト
void ClearScene::SpotSet(XMFLOAT3& Pos, const XMFLOAT3& AfterPos, const float AddFrame) {
	Pos = { Ease(In,Cubic,AddFrame,Pos.x,AfterPos.x),
		Pos.y,
		Ease(In,Cubic,AddFrame,Pos.z,AfterPos.z),
	};
}
//オノマトペ生成
void ClearScene::BirthOnomatope() {
	XMFLOAT2 l_BirthPos = {};
	if (m_AppTimer == 410 || m_AppTimer == 430) {
		if (m_AppTimer == 410) {
			l_BirthPos.x = 200.0f;
		}
		else {
			l_BirthPos.x = 1000.0f;
		}
		l_BirthPos.y = 600.0f;
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Clap.wav", 0.04f);
		onomatope->AddOnomato(Pachi, l_BirthPos);
	}
	else if (m_AppTimer == 470 || m_AppTimer == 490) {
		if (m_AppTimer == 470) {
			l_BirthPos.x = 200.0f;
		}
		else {
			l_BirthPos.x = 1000.0f;
		}
		l_BirthPos.y = 600.0f;
		onomatope->AddOnomato(Pafu, l_BirthPos);
	}
}