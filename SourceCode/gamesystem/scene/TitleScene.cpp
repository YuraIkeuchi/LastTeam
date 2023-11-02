#include "TitleScene.h"
#include "input.h"
#include "ImageManager.h"
#include <Player.h>
#include <StagePanel.h>
#include <SceneManager.h>
#include "TitleEnemy.h"
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
	wchar_t* sample = TextManager::GetInstance()->SearchText(TextManager::TITLE_01);
	texts[0] = (std::move(std::make_unique<Font>(sample, XMFLOAT2{ 300.f,380.f }, XMVECTOR{ 1.f,1.f,1.f,1.f })));

	wchar_t* sample2 = TextManager::GetInstance()->SearchText(TextManager::TITLE_02);
	texts[1] = (std::move(std::make_unique<Font>(sample2, XMFLOAT2{ 300.f,420.f }, XMVECTOR{ 1.f,0.f,1.f,1.f })));

	wchar_t* sample3 = TextManager::GetInstance()->SearchText(TextManager::TITLE_03);
	texts[2] = (std::move(std::make_unique<Font>(sample3, XMFLOAT2{ 300.f,460.f }, XMVECTOR{ 1.f,0.f,0.f,1.f })));

	if (!s_GameLoop) {
		SceneChanger::GetInstance()->Initialize();
		s_GameLoop = true;
	}

	//プレイヤー
	Player::GetInstance()->LoadResource();
	Player::GetInstance()->InitState({ -4.0f,1.0f,2.0f });
	Player::GetInstance()->Initialize();
	//ステージの床
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->Initialize();



	//敵
	enemy = make_unique<TitleEnemy>();
	enemy->Initialize();
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	camerawork->Update(camera);
	// 全オブジェクト更新
	game_object_manager_->Update();

	//各クラス更新
	camerawork->Update(camera);
	lightGroup->Update();
	Player::GetInstance()->TitleUpdate();
	StagePanel::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemy->Update();

	if ((input->TriggerButton(input->B))) {			//バトル
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = PLAY;

	}
	if (input->TriggerKey(DIK_SPACE)) {			//マップ
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = MAP;
	}
	if (input->TriggerButton(input->X)) {			//チュートリアル
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = TUTORIAL;
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

	SceneChanger::GetInstance()->Update();
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
	//完全に前に書くスプライト
	for (int i = 0; i < 3; i++) {
		if (i != 0) {
			if (texts[(size_t)i - 1]->GetFinish()) {
				texts[i]->Draw();

			}
		} else {
			texts[i]->Draw();
		}
	}
	Font::PostDraw();
	IKESprite::PreDraw();
	SceneChanger::GetInstance()->Draw();
	IKESprite::PostDraw();

}
//背面描画
void TitleScene::BackDraw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	StagePanel::GetInstance()->Draw(dxCommon);
	Player::GetInstance()->Draw(dxCommon);
	//GameStateManager::GetInstance()->Draw(dxCommon);
	IKEObject3d::PostDraw();

	enemy->Draw(dxCommon);

	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	IKETexture::PostDraw();
}
//ImGui描画
void TitleScene::ImGuiDraw(DirectXCommon* dxCommon) {
	//ImGui::Begin("TITLE");
	//ImGui::Text("Title");
	//ImGui::End();
	//SceneChanger::GetInstance()->ImGuiDraw();
}
//解放
void TitleScene::Finalize() {
}
