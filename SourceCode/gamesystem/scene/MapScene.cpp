#include "MapScene.h"
#include <ImageManager.h>
#include <Helper.h>
#include "CsvLoader.h"

void MapScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);

	dxCommon->SetFullScreen(true);
	screen = IKESprite::Create(ImageManager::MAPSCREEN, { 0,0 });
	screen->SetSize({ 1280.f,720.f });


	UIs[Tutorial].sprite = IKESprite::Create(ImageManager::MAP_START, { 0,0 });
	UIs[Tutorial].pos = basePos[0];
	UIs[Tutorial].size = { 128.f,128.f };
	UIs[Tutorial].sprite->SetAnchorPoint({ 0.5f,0.5f });

	MapCreate();





}

void MapScene::Update(DirectXCommon* dxCommon) {
	if (Input::GetInstance()->Pushkey(DIK_D)) {
		scroll.x += 5;
	}
	if (Input::GetInstance()->Pushkey(DIK_A)) {
		scroll.x -= 5;
	}	if (Input::GetInstance()->Pushkey(DIK_W)) {
		scroll.y -= 5;
	}	if (Input::GetInstance()->Pushkey(DIK_S)) {
		scroll.y += 5;
	}

	for (UI& ui : UIs) {
		if (!ui.sprite) { continue; }
		ui.sprite->SetPosition({ ui.pos.x + scroll.x, ui.pos.y + scroll.y });
		ui.sprite->SetSize(ui.size);
	}

}

void MapScene::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		dxCommon->PostDraw();
	}
	else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		dxCommon->PostDraw();
	}


}

void MapScene::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	screen->Draw();
	for (UI& ui : UIs) {
		if (!ui.sprite) { continue; }
		ui.sprite->Draw();
	}
}

void MapScene::BackDraw(DirectXCommon* dxCommon) {
}

MapScene::UI MapScene::RandPannel() {


	int min = ImageManager::MAP_NORMAL;
	int max = ImageManager::MAP_BOSS;
	int r = Helper::GetInstance()->GetRanNum(min, max);

	UI itr;
	if (r == min) {
		itr.sprite = IKESprite::Create(ImageManager::MAP_NORMAL, { 0,0 });
		itr.Tag = BATTLE;
	}
	else if (r == max) {
		itr.sprite = IKESprite::Create(ImageManager::MAP_BOSS, { 0,0 });
		itr.Tag = BOSS;
	}
	else {
		itr.sprite = IKESprite::Create(ImageManager::MAP_HEAL, { 0,0 });
		itr.Tag = HEAL;

	}

	return itr;
}

void MapScene::MapCreate() {
	string csv_ = "Resources/csv/map.csv";
	dungeon.resize(1);
	//指定してゲットする
	LoadCSV::LoadCsvParam_String(csv_, dungeon, "map");
	//けたすうしゅとくする
	int Len = (int)dungeon[0].length();

	dungeons.resize(Len);
	//1文字ずつ格納
	for (int i = 0; i < Len; ++i) {
		dungeons[i] = (int)(dungeon[0][i] - '0');
	}
	for (int i = 0; i < 2;i++) {
		switch (dungeons[i]) {
		case 1:
			UIs[1] = RandPannel();
			UIs[1].pos = basePos[1];
			UIs[1].size = { 128.f,128.f };
			UIs[1].sprite->SetAnchorPoint({ 0.5f,0.5f });
			break;
		case 2:
			UIs[2] = RandPannel();
			UIs[2].pos = { basePos[2].x, basePos[2].y- interbal.y};
			UIs[2].size = { 128.f,128.f };
			UIs[2].sprite->SetAnchorPoint({ 0.5f,0.5f });
			
			UIs[3] = RandPannel();
			UIs[3].pos = { basePos[2].x, basePos[2].y + interbal.y };
			UIs[3].size = { 128.f,128.f };
			UIs[3].sprite->SetAnchorPoint({ 0.5f,0.5f });
			break;
		default:
			break;
		}
	}

}



void MapScene::ImGuiDraw() {
}

void MapScene::Finalize() {

}