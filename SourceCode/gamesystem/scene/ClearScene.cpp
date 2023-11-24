#include "ClearScene.h"
#include "ImageManager.h"
#include <SceneManager.h>
#include <Helper.h>
//初期化
void ClearScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	sprite = IKESprite::Create(ImageManager::GAMECLEARBACK, { 0.0f,0.0f });

	for (int i = 0; i < 3; i++) {
		RandShineInit();
	}
}
//更新
void ClearScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	if ((input->TriggerButton(input->B)) && (!SceneChanger::GetInstance()->GetChangeStart())) {			//バトル
		SceneChanger::GetInstance()->SetChangeStart(true);
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button.wav", 0.15f);
	}

	if (SceneChanger::GetInstance()->GetChange()) {			//真っ暗になったら変わる
		GameReset({ -4.0f,0.1f,2.0f });
		SceneManager::GetInstance()->ChangeScene("TITLE");
		SceneChanger::GetInstance()->SetChange(false);
	}

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
	sprite->Draw();
	for (ShineEffect& shine : shines) {
		shine.tex->Draw();
	}
	IKESprite::PostDraw();

	SceneChanger::GetInstance()->Draw();
}
//背面描画
void ClearScene::BackDraw(DirectXCommon* dxCommon) {
}
//ImGui描画
void ClearScene::ImGuiDraw(DirectXCommon* dxCommon) {
	ImGui::Begin("Clear");
	ImGui::Text("Clear");
	ImGui::End();
	SceneChanger::GetInstance()->ImGuiDraw();
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