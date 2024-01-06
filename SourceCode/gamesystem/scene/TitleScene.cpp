#include "TitleScene.h"
#include "input.h"
#include "ImageManager.h"
#include <StagePanel.h>
#include "MobEnemy.h"
#include "GameStateManager.h"
#include "TextManager.h"
#include <StageBack.h>
//初期化
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);

	if (!s_GameLoop) {
		SceneChanger::GetInstance()->Initialize();
		s_GameLoop = true;
	}

	nowHierarchy = 0;
	clearHierarchy = 1;
	nowIndex = 1;
	s_LastStage = false;

	////�X�e�[�W�̏�
//// プレイヤー生成
//{
//	auto player = GameObject::CreateObject<Player>();	// �v���C���[����
//	player->LoadResource();
//	player->InitState({ -4.0f,0.1f,2.0f });
//	player->Initialize();
//	player->SetTitleFlag(true);

//	GameStateManager::GetInstance()->SetPlayer(player);
//}
	player_ = make_unique<Player>();
	player_->LoadResource();
	player_->SetTitleFlag(true);
	player_->InitState({ -PANEL_SIZE * 2.f,0.1f,PANEL_SIZE });
	player_->Initialize();
	player_->SkipInitialize();
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->SetPlayer(player_.get());
	StagePanel::GetInstance()->Initialize();
	//背景画像
	StageBack::GetInstance()->LoadResource();

	////敵
	InterEnemy::SetPlayer(player_.get());
	enemy = make_unique<MobEnemy>();
	enemy->Initialize();

	//カード
	title_ = IKESprite::Create(ImageManager::TITLETEXT, { 0.0f,0.0f });
	player = IKESprite::Create(ImageManager::MAP_CHARA, { 0.0f,0.0f });
	player->SetAnchorPoint({0.5f,1.0f});
	player->SetSize({256.f,256.f});
	//player->SetIsFlipX(true);
	player->SetPosition({640.f-128.f,360.f+128.f});
	for (int i = 0; i < 11;i++) {
		unique_ptr<IKESprite> passive = IKESprite::Create(ImageManager::PASSIVE_00 + i, { 0,0 });
		passive->SetSize({ 86.f,86.f });
		//passive->SetAnchorPoint({0.5f,0.5f});
		passive->SetPosition({128.0f * i,20.0f});
		passives.push_back(std::move(passive));
	}
	
	for (int i = 0; i < 11; i++) {
		unique_ptr<IKESprite> skill = IKESprite::Create(ImageManager::ATTACK_0 + i, { 0,0 });
		skill->SetSize({ 86.f,86.f });
		//passive->SetAnchorPoint({0.5f,0.5f});
		skill->SetPosition({ 128.0f * i,614.0f });
		skills.push_back(std::move(skill));
	}
	GameStateManager::GetInstance()->DeckReset();
	GameStateManager::GetInstance()->SetGameStart(true);
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	camerawork->Update(camera);
	player_->Update();
	//各クラス更新
	camerawork->Update(camera);

	lightGroup->Update();
	StagePanel::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemy->Update();
	rota += 0.5f * XM_PI / 180.f;
	rota2 += 3.f * XM_PI / 180.f;
	float ease = Ease(InOut,Linear, abs(sinf(rota)),-15.f,15.f);
	float ease_size= Ease(InOut, Cubic, abs(sinf(rota2)), 0.97f, 1.f);
	float ease_size2 = Ease(InOut, Quint, abs(sinf(rota2)), 0.97f, 1.f);
	//player->SetRotation(ease);
	player->SetSize({ 256.f* ease_size,256.f * ease_size2 });
	//player->SetPosition({ 640.f - 128.f,360.f + ease_size });

	for (unique_ptr<IKESprite>& passive : passives) {
		XMFLOAT2 pos = passive->GetPosition();
		pos.x += 2.f;
		if (pos.x >= 1280.f) {
			pos.x = -128.f;
		}
		passive->SetPosition(pos);
	}
	for (unique_ptr<IKESprite>& skill : skills) {
		XMFLOAT2 pos = skill->GetPosition();
		pos.x -= 2.f;
		if (pos.x <= -128.f) {
			pos.x = 1280.f;
		}
		skill->SetPosition(pos);
	}
	if ((input->TriggerButton(input->B)|| input->TriggerKey(DIK_SPACE)) && (!SceneChanger::GetInstance()->GetChangeStart())) {			//バトル
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = PLAY;
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button.wav", 0.15f);
	}
	
	if (SceneChanger::GetInstance()->GetChange()) {			//真っ暗になったら変わる
		player_->PlayerSave();
		SceneManager::GetInstance()->ChangeScene("MAP");
		SceneChanger::GetInstance()->SetChange(false);
	}

}
//描画
void TitleScene::Draw(DirectXCommon* dxCommon) {
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
	} else {
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
void TitleScene::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	title_->Draw();
	IKESprite::PostDraw();
	SceneChanger::GetInstance()->Draw();

}
//背面描画
void TitleScene::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	//title_[TITLE_BACK]->Draw();
	StageBack::GetInstance()->Draw(dxCommon);
	player->Draw();
	for (unique_ptr<IKESprite>& passive:passives) {
		passive->Draw();
	}
	for (unique_ptr<IKESprite>& skill : skills) {
		skill->Draw();
	}

	IKESprite::PostDraw();
	//StagePanel::GetInstance()->Draw(dxCommon);
	//player_->Draw(dxCommon);
	//enemy->Draw(dxCommon);

}
//ImGui描画
void TitleScene::ImGuiDraw(DirectXCommon* dxCommon) {
	//camerawork->ImGuiDraw();
	//player_->ImGuiDraw();
}
//解放
void TitleScene::Finalize() {
}
