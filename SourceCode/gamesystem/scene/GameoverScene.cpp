#include "GameoverScene.h"
#include "ImageManager.h"
#include <SceneManager.h>
#include <StagePanel.h>
//初期化
void GameoverScene::Initialize(DirectXCommon* dxCommon) {
	float scale = PANEL_SIZE * 0.1f;
	//共通の初期化
	BaseInitialize(dxCommon);
	Audio::GetInstance()->StopWave(AUDIO_MAIN);

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

	attach[0].position = { 2.0f,0.0f,0.0f };
	attach[1].position = { -4.0f,0.0f,0.0f };
	//ゲームオーバーのサイズ
	const int PartsWidth_Cut = 1000;
	const int PartsHeight_Cut = 128;
	//セレクトのサイズ
	const int SelectWidth_Cut = 128;
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

	m_OverPos[0] = { 640.0f,150.0f };
	m_OverPos[1] = { 640.0f,-150.0f };

	m_SelectPos[0] = { 230.0f,600.0f };
	m_SelectPos[1] = { 1000.0f,600.0f };

	attach[0].line->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
	attach[1].line->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
}
//更新
void GameoverScene::Update(DirectXCommon* dxCommon) {
	m_Timer++;
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->B)|| input->TriggerKey(DIK_SPACE))) {			//バトル
		SceneChanger::GetInstance()->SetChangeStart(true);
	}

	if (SceneChanger::GetInstance()->GetChange()) {			//真っ暗になったら変わる
		SceneManager::GetInstance()->ChangeScene("TITLE");
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
	}
	player_->GameOverUpdate(m_Timer);
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
	ImGui::Begin("Gameover");
	ImGui::Text("OverTime:%d",m_Timer);
	ImGui::End();
	player_->ImGuiDraw();
	//SceneChanger::GetInstance()->ImGuiDraw();
}
//解放
void GameoverScene::Finalize() {
}