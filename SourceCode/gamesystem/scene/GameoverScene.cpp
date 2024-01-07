#include "GameoverScene.h"
#include "ImageManager.h"
#include <SceneManager.h>
#include <StagePanel.h>
//初期化
void GameoverScene::Initialize(DirectXCommon* dxCommon) {
	float scale = PANEL_SIZE * 0.1f;
	//共通の初期化
	BaseInitialize(dxCommon);
	sprite = IKESprite::Create(ImageManager::GAMEOVERBACK, { 0.0f,0.0f });
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
		attach[i].line->SetPosition(attach[i].position);
		attach[i].line->Update();
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
	sprite->Draw();
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