#include "GameoverScene.h"
#include "ImageManager.h"
#include <SceneManager.h>
#include <StagePanel.h>
#include <GameStateManager.h>
#include <Helper.h>
//初期化
void GameoverScene::Initialize(DirectXCommon* dxCommon) {
	float scale = PANEL_SIZE * 0.1f;
	//共通の初期化
	BaseInitialize(dxCommon);
	Audio::GetInstance()->StopWave(AUDIO_MAIN);
	Audio::GetInstance()->LoopWave(AUDIO_OVER, 0.02f);
	std::string BaseName = "Resources/csv/EnemySpawn/Clear/ClearMap.csv";
	GameStateManager::GetInstance()->SetEnemySpawnText(BaseName);

	player_ = make_unique<Player>();
	player_->LoadResource();
	player_->InitState({ 0.0f,0.1f,0.0f });
	player_->Initialize();

	for (int i = 0; i < ATTACH_MAX; i++) {
		attach[i].object = make_unique<IKEObject3d>();
		attach[i].object->Initialize();
		attach[i].object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PANEL));
		attach[i].object->SetScale({ PANEL_SIZE,0.01f,PANEL_SIZE });
		attach[i].line = make_unique<IKETexture>(ImageManager::PANNELLINE, XMFLOAT3{}, XMFLOAT3{ 1.f,1.f,1.f }, XMFLOAT4{ 1.f,1.f,1.f,1.f });
		attach[i].line->TextureCreate();
		attach[i].line->Initialize();
		attach[i].line->SetScale({ scale ,scale ,scale });
		attach[i].line->SetRotation({ 90.0f,0.0f,0.0f });
	}

	attach[0].position = { 2.0f,-20.0f,0.0f };
	attach[1].position = { -4.0f,-20.0f,0.0f };
	//ゲームオーバーのサイズ
	const int PartsWidth_Cut = 1000;
	const int PartsHeight_Cut = 128;
	//セレクトのサイズ
	const int SelectWidth_Cut = 200;
	const int SelectHeight_Cut = 64;


	const int OverCount = OVER_MAX;
	for (int i = 0; i < OVER_MAX; i++) {
		gameover[i] = IKESprite::Create(ImageManager::GAMEOVERBACK, { 0.0f,0.0f });
		int number_index_y = i / OverCount;
		int number_index_x = i % OverCount;
		gameover[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * PartsWidth_Cut, static_cast<float>(number_index_y) * PartsHeight_Cut },
			{ static_cast<float>(PartsWidth_Cut), static_cast<float>(PartsHeight_Cut) });
		gameover[i]->SetAnchorPoint({ 0.5f,0.5f });
		gameover[i]->SetSize({ PartsWidth_Cut,PartsHeight_Cut });
		gameover[i]->SetScale(0.8f);
		
		select[i] = IKESprite::Create(ImageManager::GAMEOVER_SELECT, { 0.0f,0.0f });
		int number_index_y2 = i / OverCount;
		int number_index_x2 = i % OverCount;
		select[i]->SetTextureRect(
			{ static_cast<float>(number_index_x2) * SelectWidth_Cut, static_cast<float>(number_index_y2) * SelectHeight_Cut },
			{ static_cast<float>(SelectWidth_Cut), static_cast<float>(SelectHeight_Cut) });
		select[i]->SetAnchorPoint({ 0.5f,0.5f });
		select[i]->SetScale(1.3f);
		m_OverSize[i] = { SelectWidth_Cut, SelectHeight_Cut };
	}

	stick = IKESprite::Create(ImageManager::GAMEOVER_STICK, { 0.0f,0.0f });
	stick->SetAnchorPoint({ 0.5f,0.5f });
	stick->SetScale(1.5f);

	check = IKESprite::Create(ImageManager::RESULTNOWCHECK, { 0.0f,0.0f });
	check->SetAnchorPoint({ 0.5f,0.5f });
	check->SetScale(1.5f);
	check->SetPosition({ 1150.0f,680.0f });
	//stick->SetPosition({ 640.0f,500.0f });

	select[SELECT_NO]->SetColor({ 1.0f,0.15f,0.15f,1.0f });
	select[SELECT_YES]->SetColor({ 0.15f,1.0f,0.15f,1.0f });

	m_OverPos[0] = { 640.0f,-150.0f };
	m_OverPos[1] = { 640.0f,-150.0f };

	m_AfterOverPos[0] = { 640.0f,120.0f };
	m_AfterOverPos[1] = { 640.0f,-150.0f };

	m_SelectPos[0] = { 230.0f,900.0f };
	m_SelectPos[1] = { 1000.0f,900.0f };

	m_AfterSelectPos[0] = { 230.0f,600.0f };
	m_AfterSelectPos[1] = { 1000.0f,600.0f };

	attach[0].line->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
	attach[1].line->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

	//敵
	EnemyManager::SetPlayer(player_.get());
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Initialize();

	s_LastStage = false;
}
//更新
void GameoverScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if (player_->GetSelectEnd()) {			//バトル
		SceneChanger::GetInstance()->SetChangeStart(true);
		player_->SetSelectEnd(false);
	}

	if (SceneChanger::GetInstance()->GetChange()) {			//真っ暗になったら変わる
		if (player_->GetSelectType() == 1) {		//コンティニューをしてマップに戻る
			s_Countinue = true;
			if (GameStateManager::GetInstance()->GetNoDeath()) {
				GameStateManager::GetInstance()->SetNoDeath(false);
			}
			s_PlayBGM = true;
			SceneManager::GetInstance()->ChangeScene("MAP");
		}
		else if(player_->GetSelectType() == 2) {		//諦めてタイトルに戻る
			s_Countinue = false;
			s_PlayBGM = true;
			SceneManager::GetInstance()->ChangeScene("TITLE");
		}
		Audio::GetInstance()->StopWave(AUDIO_OVER);
		SceneChanger::GetInstance()->SetChange(false);
	}

	for (int i = 0; i < ATTACH_MAX; i++) {
		attach[i].object->SetPosition(attach[i].position);
		attach[i].object->Update();
		attach[i].line->SetPosition({ attach[i].position.x,attach[i].position.y + 0.05f,attach[i].position.z });
		attach[i].line->Update();
	}
	for (int i = 0; i < OVER_MAX; i++) {
		gameover[i]->SetPosition(m_OverPos[i]);
		select[i]->SetPosition(m_SelectPos[i]);
		select[i]->SetSize(m_OverSize[i]);
		select[i]->SetRotation(m_SelectRot[i]);
	}
	stick->SetPosition(m_StickPos);
	GameOverMove();
	player_->GameOverUpdate(m_Timer);
	enemyManager->GameOverUpdate();
	//ライト更新
	lightGroup->Update();
	//カメラワーク更新
	camerawork->ClearUpdate(camera);
	SceneChanger::GetInstance()->Update();
}
//描画
void GameoverScene::Draw(DirectXCommon* dxCommon) {
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
void GameoverScene::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	for (int i = 0; i < OVER_MAX; i++) {
		gameover[i]->Draw();
		select[i]->Draw();
	}
	if(!player_->GetSelectMove() && player_->GetSelectType() != 0)
	check->Draw();
	stick->Draw();
	IKESprite::PostDraw();
	SceneChanger::GetInstance()->Draw();
}
//背面描画
void GameoverScene::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	//StageBack::GetInstance()->Draw(dxCommon);
	IKESprite::PostDraw();
	IKEObject3d::PreDraw();
	player_->Draw(dxCommon);
	enemyManager->Draw(dxCommon);
	for (int i = 0; i < ATTACH_MAX; i++) {
		attach[i].object->Draw();
	}
	IKEObject3d::PostDraw();
	IKETexture::PreDraw2(dxCommon, AddBlendType);
	for (int i = 0; i < ATTACH_MAX; i++) {
		attach[i].line->Draw();
	}
	IKETexture::PostDraw();
}
//ImGui描画
void GameoverScene::ImGuiDraw(DirectXCommon* dxCommon) {
}
//解放
void GameoverScene::Finalize() {
}

void GameoverScene::GameOverMove() {
	const float l_AddFrame = 1 / 60.0f;
	if (_OverType == MOVE_NONE) {
		if (Helper::CheckMin(m_Timer, 50, 1)) {
			_OverType = MOVE_OVER;
		}
	}
	else if (_OverType == MOVE_OVER) {		//ゲームオーバーの文字が落ちてくる
		for (int i = 0; i < OVER_MAX; i++) {
		if (Helper::FrameCheck(m_Frame[i], l_AddFrame)) {
			if (Helper::CheckMin(m_Timer, 100, 1)) {
				_OverType = MOVE_COUNTINUE;
				m_Frame = {};
				m_AfterOverPos[PARTS_OVER].y = -150.0f;
				m_AfterOverPos[PARTS_COUNTINUE].y = 120.0f;
			}
		}
		else {
				m_OverPos[i].y = Ease(In, Cubic, m_Frame[i], m_OverPos[i].y, m_AfterOverPos[i].y);
			}
		}
	}
	else if (_OverType == MOVE_COUNTINUE) {		//コンティニューが落ちてくる
		for (int i = 0; i < OVER_MAX; i++) {
		if (Helper::FrameCheck(m_Frame[i], l_AddFrame)) {
			if (Helper::CheckMin(m_Timer, 150, 1)) {
				_OverType = MOVE_PANEL;
				m_Frame = {};
			}
		}
		else {
				m_OverPos[i].y = Ease(In, Cubic, m_Frame[i], m_OverPos[i].y, m_AfterOverPos[i].y);
			}
		}
	}
	else if (_OverType == MOVE_PANEL) {		//パネルとかが浮かび上がる
		for (int i = 0; i < OVER_MAX; i++) {
			if (Helper::FrameCheck(m_Frame[i], l_AddFrame)) {
				m_Frame[i] = 1.0f;
			}
			else {
				m_SelectPos[i].y = Ease(In, Cubic, m_Frame[i], m_SelectPos[i].y, m_AfterSelectPos[i].y);
			}
		
		}
		for (int i = 0; i < ATTACH_MAX; i++) {
			if (Helper::FrameCheck(attach[i].frame, l_AddFrame)) {
				if (player_->GetOverType() == 3) {
					if (player_->GetSelectType() == 1) {
						_OverType = MOVE_YES_SELECT;
						m_Frame[i] = {};
						attach[i].frame = {};
						m_Timer = {};
						m_AddPower[SELECT_YES] = 35.0f;
						Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button.wav", 0.15f);
					}
					else if (player_->GetSelectType() == 2) {
						_OverType = MOVE_NO_SELECT;
						m_Frame[i] = {};
						attach[i].frame = {};
						m_Timer = {};
						m_AddPower[SELECT_NO] = 35.0f;
						Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button.wav", 0.15f);
					}
				}
			}
			else {
				attach[i].position.y = Ease(In, Cubic, attach[i].frame, attach[i].position.y, 0.0f);
			}
		}

		m_StickPos.y = Ease(In, Cubic, attach[0].frame, m_StickPos.y, m_AfterSelectPos[0].y);
	}
	else if(_OverType == MOVE_YES_SELECT) {		//はいの文字が動く
		if (m_AddPower[SELECT_YES] <= 0.0f && m_SelectPos[SELECT_YES].y >= 100.0f) {
			m_SelectPos[SELECT_YES].y = 100.0f;
			if (Helper::FrameCheck(m_Frame[SELECT_YES], 0.01f)) {
				m_Frame[SELECT_YES] = 1.0f;
			}
			else {
				m_SelectRot[SELECT_YES] = Ease(Out, Cubic, m_Frame[SELECT_YES], m_SelectRot[SELECT_YES], 360.0f);
			}
		}
		else {
			m_AddPower[SELECT_YES] -= m_Gravity;
			m_SelectPos[SELECT_YES].y -= m_AddPower[SELECT_YES];
		}

		m_SelectPos[SELECT_NO].y += 2.0f;
		m_SelectRot[SELECT_NO] += 10.0f;
	}
	else {
		m_AddPower[SELECT_NO] -= m_Gravity;
		m_SelectPos[SELECT_NO].y -= m_AddPower[SELECT_NO];
		m_SelectRot[SELECT_NO] += 5.0f;
		m_SelectPos[SELECT_YES].y += 2.0f;
		m_SelectRot[SELECT_YES] += 10.0f;
	}

	if (!player_->GetSelectEnd()) {
		for (int i = 0; i < OVER_MAX; i++) {
			m_Angle[i] += 2.0f;
			m_Angle2[i] = m_Angle[i] * (3.14f / 180.0f);
			m_OverSize[i] = { (sin(m_Angle2[i]) * 32.0f) + (200.0f),
				(sin(m_Angle2[i]) * 16.0f) + (64.0f) };
		}
	}
}