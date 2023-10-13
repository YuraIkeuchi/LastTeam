#include "MapSceneActor.h"
#include"ImageManager.h"
void MapSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);
	UI ui[Max];
	ui[Screen].sprite = IKESprite::Create(ImageManager::MAPSCREEN, { 0,0 });
	ui[Screen].pos = { 640,360 };
	ui[Screen].size = { 1280.f,720.f };

	ui[StartMAP].sprite = IKESprite::Create(ImageManager::MAP_START, { 0,0 });
	ui[StartMAP].pos = basePos;
	ui[StartMAP].size = { 128.f,128.f };


	ui[NormalMAP].sprite = IKESprite::Create(ImageManager::MAP_NORMAL, { 0,0 });
	ui[NormalMAP].pos = { basePos.x + interbal.x ,basePos.y };
	ui[NormalMAP].size = { 128.f,128.f };

	ui[BossMAP].sprite = IKESprite::Create(ImageManager::MAP_BOSS, { 0,0 });
	ui[BossMAP].pos = { basePos.x + interbal.x,basePos.y - interbal.y };
	ui[BossMAP].size = { 128.f,128.f };

	ui[HealMAP].sprite = IKESprite::Create(ImageManager::MAP_HEAL, { 0,0 });
	ui[HealMAP].pos = { basePos.x + interbal.x,basePos.y + interbal.y };
	ui[HealMAP].size = { 128.f,128.f };


	for (int i = Screen; i < Max; i++) {
		if (i != Screen) { ui[i].isPannel = true; }
		ui[i].sprite->SetAnchorPoint({ 0.5f,0.5f });
		UIs.push_back(std::move(ui[i]));
	}


}

void MapSceneActor::Finalize() {
}

void MapSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
	if (Input::GetInstance()->Pushkey(DIK_D)) {
		scroll.x+=5;
	}
	if (Input::GetInstance()->Pushkey(DIK_A)) {
		scroll.x-= 5;
	}	if (Input::GetInstance()->Pushkey(DIK_W)) {
		scroll.y-= 5;
	}	if (Input::GetInstance()->Pushkey(DIK_S)) {
		scroll.y+= 5;
	}

	for (UI& ui : UIs) {
		if (ui.isPannel) {
			ui.sprite->SetPosition({ ui.pos.x + scroll.x, ui.pos.y + scroll.y});
		} else {
			ui.sprite->SetPosition(ui.pos);
		}
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
