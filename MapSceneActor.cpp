//#include "MapSceneActor.h"
//#include"ImageManager.h"
//#include <Helper.h>
//void MapSceneActor::Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
//	//共通の初期化
//	BaseInitialize(dxCommon);
//	dxCommon->SetFullScreen(true);
//	screen = IKESprite::Create(ImageManager::MAPSCREEN, { 0,0 });
//	screen->SetSize({ 1280.f,720.f });
//
//
//	UIs[Tutorial].sprite = IKESprite::Create(ImageManager::MAP_START, { 0,0 });
//	UIs[Tutorial].pos = basePos[0];
//	UIs[Tutorial].size = { 128.f,128.f };
//	UIs[Tutorial].sprite->SetAnchorPoint({ 0.5f,0.5f });
//
//	for (auto i = 0; i < (FirstChoice - Tutorial); i++) {
//		UIs[i + 1] = RandPannel();
//		UIs[i + 1].pos = { basePos[1].x,basePos[1].y + (interbal.y * i) };
//		UIs[i + 1].size = { 128.f,128.f };
//		UIs[i + 1].sprite->SetAnchorPoint({ 0.5f,0.5f });
//	}
//	int j = 0;
//	for (int i = FirstChoice; i < SecondChoice; i++) {
//		UIs[i + 1] = RandPannel();
//		UIs[i + 1].pos = { basePos[2].x,basePos[2].y + (interbal.y * j) };
//		UIs[i + 1].size = { 128.f,128.f };
//		UIs[i + 1].sprite->SetAnchorPoint({ 0.5f,0.5f });
//		j++;
//	}
//	int k = 0;
//	for (int i = SecondChoice; i < ThirdChoice; i++) {
//		UIs[i + 1] = RandPannel();
//		UIs[i + 1].pos = { basePos[3].x,basePos[3].y + (interbal.y * k) };
//		UIs[i + 1].size = { 128.f,128.f };
//		UIs[i + 1].sprite->SetAnchorPoint({ 0.5f,0.5f });
//		k++;
//	}
//}
//
//void MapSceneActor::Finalize() {
//}
//
//void MapSceneActor::Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) {
//	if (Input::GetInstance()->Pushkey(DIK_D)) {
//		scroll.x += 5;
//	}
//	if (Input::GetInstance()->Pushkey(DIK_A)) {
//		scroll.x -= 5;
//	}	if (Input::GetInstance()->Pushkey(DIK_W)) {
//		scroll.y -= 5;
//	}	if (Input::GetInstance()->Pushkey(DIK_S)) {
//		scroll.y += 5;
//	}
//
//	for (UI& ui : UIs) {
//		if (!ui.sprite) { continue; }
//		ui.sprite->SetPosition({ ui.pos.x + scroll.x, ui.pos.y + scroll.y });
//		ui.sprite->SetSize(ui.size);
//	}
//
//}
//
//void MapSceneActor::Draw(DirectXCommon* dxCommon) {
//	//ポストエフェクトをかけるか
//	if (PlayPostEffect) {
//		postEffect->PreDrawScene(dxCommon->GetCmdList());
//		BackDraw(dxCommon);
//		FrontDraw(dxCommon);
//		postEffect->PostDrawScene(dxCommon->GetCmdList());
//		dxCommon->PreDraw();
//		postEffect->Draw(dxCommon->GetCmdList());
//		dxCommon->PostDraw();
//	} else {
//		postEffect->PreDrawScene(dxCommon->GetCmdList());
//		postEffect->Draw(dxCommon->GetCmdList());
//		postEffect->PostDrawScene(dxCommon->GetCmdList());
//		dxCommon->PreDraw();
//		BackDraw(dxCommon);
//		FrontDraw(dxCommon);
//		dxCommon->PostDraw();
//	}
//
//
//}
//
//void MapSceneActor::FrontDraw(DirectXCommon* dxCommon) {
//	IKESprite::PreDraw();
//	screen->Draw();
//	for (UI& ui : UIs) {
//		if (!ui.sprite) { continue; }
//		ui.sprite->Draw();
//	}
//}
//
//void MapSceneActor::BackDraw(DirectXCommon* dxCommon) {
//}
//
//void MapSceneActor::IntroUpdate(DebugCamera* camera) {
//}
//
//void MapSceneActor::MainUpdate(DebugCamera* camera) {
//}
//
//void MapSceneActor::FinishUpdate(DebugCamera* camera) {
//}
//
//MapSceneActor::UI MapSceneActor::RandPannel() {
//
//
//	int min = ImageManager::MAP_NORMAL;
//	int max = ImageManager::MAP_BOSS;
//	int r = Helper::GetInstance()->GetRanNum(min, max);
//	
//	UI itr;
//	if (r == min) {
//		itr.sprite = IKESprite::Create(ImageManager::MAP_NORMAL, { 0,0 });
//		itr.Tag = BATTLE;
//	} else if (r == max) {
//		itr.sprite = IKESprite::Create(ImageManager::MAP_BOSS, { 0,0 });
//		itr.Tag = BOSS;
//	} else {
//		itr.sprite = IKESprite::Create(ImageManager::MAP_HEAL, { 0,0 });
//		itr.Tag = HEAL;
//
//	}
//	
//	return itr;
//}
//
//
//
//void MapSceneActor::ImGuiDraw() {
//}
