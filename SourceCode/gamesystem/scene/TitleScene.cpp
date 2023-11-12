#include "TitleScene.h"
#include "input.h"
#include "ImageManager.h"
#include <Player.h>
#include <StagePanel.h>
#include <SceneManager.h>
#include "MobEnemy.h"
#include "TutorialTask.h"
#include <GameStateManager.h>
// 遷移しうるシーン
#include "BattleScene.h"
#include "MapScene.h"
#include "TutorialScene.h"
#include "TextManager.h"
//初期化
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);

	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::TITLE);
	if (!s_GameLoop) {
		SceneChanger::GetInstance()->Initialize();
		s_GameLoop = true;
	}

	//プレイヤー
	Player::GetInstance()->LoadResource();
	Player::GetInstance()->InitState({ -4.0f,0.1f,2.0f });
	Player::GetInstance()->Initialize();
	//ステージの床
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->Initialize();

	//敵
	enemy = make_unique<MobEnemy>();
	enemy->Initialize();

	//タイトル
	title_[TITLE_BACK] = IKESprite::Create(ImageManager::TITLEBACK, { 0.0f,0.0f });
	title_[TITLE_TEXT] = IKESprite::Create(ImageManager::TITLETEXT, { 640.0f,200.0f },{1.0f,1.0f,1.0f,1.0f},{0.5f,0.5f});

	//丸影
	lightGroup->SetCircleShadowActive(0, true);
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	camerawork->Update(camera);
	// 全オブジェクト更新
	game_object_manager_->Update();

	//各クラス更新
	camerawork->Update(camera);
	//プレイヤー
	lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3({ Player::GetInstance()->GetPosition().x, 0.5f, Player::GetInstance()->GetPosition().z }));
	lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));
	lightGroup->Update();
	
	//各クラス更新
	Player::GetInstance()->TitleUpdate();
	StagePanel::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemy->Update();

	if ((input->TriggerButton(input->B))) {			//バトル
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = PLAY;
		//チュートリアルのタスク
		TutorialTask::GetInstance()->SetTutorialState(TASK_END);

	}
	if (input->TriggerKey(DIK_SPACE)) {			//マップ
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = MAP;
		//チュートリアルのタスク
		TutorialTask::GetInstance()->SetTutorialState(TASK_END);
	}
	if (input->TriggerButton(input->X)) {			//チュートリアル
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = TUTORIAL;
		//チュートリアルのタスク
		TutorialTask::GetInstance()->SetTutorialState(TASK_MOVE);
	}
	if (SceneChanger::GetInstance()->GetChange()) {			//真っ暗になったら変わる
		if (_SceneType == PLAY) {
			SceneManager::GetInstance()->ChangeScene<BattleScene>();
		} else if(_SceneType == MAP) {
			SceneManager::GetInstance()->ChangeScene<MapScene>();
		}
		else {
			SceneManager::GetInstance()->ChangeScene<TutorialScene>();
		}
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
	////完全に前に書くスプライト
	//for (int i = 0; i < 3; i++) {
	//	if (i != 0) {
	//		if (texts[(size_t)i - 1]->GetFinish()) {
	//			texts[i]->Draw();

	//		}
	//	} else {
	//		texts[i]->Draw();
	//	}
	//}
	//Font::PostDraw();
	text_->TestDraw(dxCommon);
	IKESprite::PreDraw();
	title_[TITLE_TEXT]->Draw();
	SceneChanger::GetInstance()->Draw();
	IKESprite::PostDraw();

}
//背面描画
void TitleScene::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	title_[TITLE_BACK]->Draw();
	IKESprite::PostDraw();
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	Player::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();

	enemy->Draw(dxCommon);

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//ImGui描画
void TitleScene::ImGuiDraw(DirectXCommon* dxCommon) {
	camerawork->ImGuiDraw();
}
//解放
void TitleScene::Finalize() {
}
