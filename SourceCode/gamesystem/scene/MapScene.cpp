#include "MapScene.h"
#include <ImageManager.h>
#include <Helper.h>
#include "CsvLoader.h"
#include <sstream>
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
	itr.size = { 128.f,128.f };
	itr.sprite->SetAnchorPoint({ 0.5f,0.5f });

	return itr;
}

void MapScene::MapCreate() {
	string csv_ = "Resources/csv/map.csv";
	dungeon.resize(1);
	int r_num=Helper::GetInstance()->GetRanNum(0,3);
	std::stringstream ss;
	ss << "map"<< r_num;
	std::string r_map = ss.str();
	
	//指定してゲットする
	LoadCSV::LoadCsvParam_String(csv_, dungeon, r_map);
	//けたすうしゅとくする
	int Len = (int)dungeon[0].length();

	dungeons.resize(Len);
	//1文字ずつ格納
	for (int i = 0; i < Len; ++i) {
		dungeons[i] = (int)(dungeon[0][i] - '0');
	}
	int nowSpawn = 1;
	for (int i = 0; i < Len; ++i) {
		//この+1はスタートを除く
		XMFLOAT2 _basePos = basePos[i + 1];
		switch (dungeons[i]) {
		case 1:
			UIs[nowSpawn] = RandPannel();
			UIs[nowSpawn].pos = _basePos;
			nowSpawn++;
			break;
		case 2: {
			XMFLOAT2 b_pos[2] = {
				{_basePos.x, _basePos.y - interbal.y},
			{ _basePos.x, _basePos.y + interbal.y } };

			for (int i = 0; i < 2; i++) {
				UIs[nowSpawn] = RandPannel();
				UIs[nowSpawn].pos = b_pos[i];
				nowSpawn++;
			}
			break;
		}
		default:
			break;
		}
	}

}



void MapScene::ImGuiDraw() {
}

void MapScene::Finalize() {

}