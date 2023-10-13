#include "MapSceneActor.h"
#include"ImageManager.h"
void MapSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);
	UI ui[Max];
	ui[Screen].sprite = IKESprite::Create(ImageManager::MAPSCREEN, { 0,0 });
	ui[Screen].pos = { 0,0 };
	ui[Screen].size={1280.f,720.f};

	ui[NormalMAP].sprite = IKESprite::Create(ImageManager::MAP_NORMAL, { 0,0 });
	ui[NormalMAP].pos = { 40,360 };
	ui[NormalMAP].size = { 256.f,256.f };

	ui[BossMAP].sprite = IKESprite::Create(ImageManager::MAP_BOSS, { 0,0 });
	ui[BossMAP].pos = { 640,360 };
	ui[BossMAP].size = { 256.f,256.f };



	for (int i = 0; i < Max;i++) {
		UIs.push_back(std::move(ui[i]));
	}


}

void MapSceneActor::Finalize() {
}

void MapSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	for (UI& ui : UIs) {
		ui.sprite->SetPosition(ui.pos);
		ui.sprite->SetSize(ui.size);
	}
	
}

void MapSceneActor::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		dxCommon->PostDraw();
	} else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		dxCommon->PostDraw();
	}


}

void MapSceneActor::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	for (UI& ui : UIs) {
		ui.sprite->Draw();
	}
}

void MapSceneActor::BackDraw(DirectXCommon* dxCommon) {
}

void MapSceneActor::IntroUpdate(DebugCamera* camera) {
}

void MapSceneActor::MainUpdate(DebugCamera* camera) {
}

void MapSceneActor::FinishUpdate(DebugCamera* camera) {
}

void MapSceneActor::ImGuiDraw() {
}
