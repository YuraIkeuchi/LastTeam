#include "MapScene.h"
#include <ImageManager.h>
#include <Helper.h>
#include "CsvLoader.h"
#include <sstream>

#include "SceneManager.h"

//遷移しうるシーン
#include "BattleScene.h"

void MapScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);

	dxCommon->SetFullScreen(true);
	screen = IKESprite::Create(ImageManager::MAPSCREEN, { 0,0 });
	screen->SetSize({ 1280.f,720.f });

	UIs[StartMAP].sprite = IKESprite::Create(ImageManager::MAP_START, { 0,0 });
	UIs[StartMAP].pos = homePos;
	UIs[StartMAP].size = { 128.f,128.f };
	UIs[StartMAP].sprite->SetAnchorPoint({ 0.5f,0.5f });

	frame = IKESprite::Create(ImageManager::MAP_FRAME, { 0,0 });
	frame->SetPosition(framePos);
	frame->SetSize({ 128.f,128.f });
	frame->SetAnchorPoint({ 0.5f,0.5f });

	MapCreate();
}

void MapScene::Update(DirectXCommon* dxCommon) {

	Move();
	for (UI& ui : UIs) {
		if (!ui.sprite) { continue; }
		ui.sprite->SetPosition({ ui.pos.x + scroll.x, ui.pos.y + scroll.y });
		ui.sprite->SetSize(ui.size);
	}
	frame->SetPosition({ framePos.x + scroll.x, framePos.y + scroll.y });

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
		ImGuiDraw();
		dxCommon->PostDraw();
	} else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		ImGuiDraw();
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
	frame->Draw();
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
	} else if (r == max) {
		itr.sprite = IKESprite::Create(ImageManager::MAP_BOSS, { 0,0 });
		itr.Tag = BOSS;
	} else {
		itr.sprite = IKESprite::Create(ImageManager::MAP_HEAL, { 0,0 });
		itr.Tag = HEAL;

	}
	itr.size = { 128.f,128.f };
	itr.sprite->SetAnchorPoint({ 0.5f,0.5f });

	return itr;
}

void MapScene::MapCreate() {
	string csv_ = "Resources/csv/map.csv";
	int r_num = Helper::GetInstance()->GetRanNum(0, 3);
	r_num = 3;
	
	//mapのあとに数字をくっつける
	std::stringstream ss;
	ss << "map" << r_num;
	std::string r_map = ss.str();
	//指定してゲットする
	LoadCSV::LoadCsvParam_String(csv_, dungeon, r_map);
	//けたすうしゅとくする
	int Len = (int)dungeon.length();
	dungeons.resize(Len);
	//1文字ずつ格納
	for (int i = 0; i < Len; ++i) {
		dungeons[i] = (int)(dungeon[i] - '0');
	}
	XMFLOAT2 _basePos = { homePos.x + interbal.x,homePos.y };
	for (int i = 0; i < Len; ++i) {
		//この+1はスタートを除く
		switch (dungeons[i]) {
		case 1:
			UIs[nowUiNum] = RandPannel();
			UIs[nowUiNum].pos = _basePos;
			nowUiNum++;
			break;
		case 2: {
			XMFLOAT2 b_pos[2] = {
				{_basePos.x, _basePos.y - interbal.y},
			{ _basePos.x, _basePos.y + interbal.y } };

			for (int i = 0; i < 2; i++) {
				UIs[nowUiNum] = RandPannel();
				UIs[nowUiNum].pos = b_pos[i];
				nowUiNum++;
			}
			break;
		}
		case 3: {
			XMFLOAT2 b_pos[3] = {
		{_basePos.x, _basePos.y - interbal.y},
		{_basePos.x, _basePos.y},
		{_basePos.x, _basePos.y + interbal.y }};
			for (int i = 0; i < 3; i++) {
				UIs[nowUiNum] = RandPannel();
				UIs[nowUiNum].pos = b_pos[i];
				nowUiNum++;
			}
			break;
		}
		default:
			break;
		}
		_basePos.x += interbal.x;
	}

}



void MapScene::ImGuiDraw() {
	ImGui::Begin("Map");
	ImGui::Text("%f", framePos.x);
	ImGui::Text("%f", eFrame);

	ImGui::End();
}

void MapScene::Move() {
	Input* input = Input::GetInstance();
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

	int vel = 0;
	if (input->PushButton(input->LB)) {
		vel = -10;
	} else if (input->PushButton(input->RB)) {
		vel = 10;
	}

	if (input->TiltStick(input->L_RIGHT)) {
		if (moved) { return; }
		if (nowMap == nowUiNum - 1) { return; }
		nowMap++;
		moved = true;
	}
	if (input->TiltStick(input->L_LEFT)) {
		if (moved) { return; }
		if (nowMap == 0) { return; }
		nowMap--;
		moved = true;
	}
	if ((input->TriggerButton(input->B))) {
		SceneManager::GetInstance()->ChangeScene<BattleScene>();

	}

	if (moved) {
		if (Helper::GetInstance()->FrameCheck(mov_frame, 1 / kMoveFrame)) {
			moved = false;
			oldMap = nowMap;
			mov_frame = 0.0f;
			return;
		}
		framePos.x = Ease(In, Quad, mov_frame, UIs[oldMap].pos.x, UIs[nowMap].pos.x);
		framePos.y = Ease(In, Quad, mov_frame, UIs[oldMap].pos.y, UIs[nowMap].pos.y);
		scroll.x = Ease(In, Quad, mov_frame, scroll.x, -UIs[nowMap].pos.x / 2);
	}
	if (Helper::GetInstance()->FrameCheck(eFrame, eAdd)) {
		eAdd *= -1.0f;
		eFrame = 0.99f;
	}
	if (eFrame == 0.0f) {
		eAdd *= -1.0f;
	}
	XMFLOAT2 size = frame->GetSize();
	size.x = Ease(InOut, Quad, eFrame, 128.f, 128.f * 1.3f);
	size.y = Ease(InOut, Quad, eFrame, 128.f, 128.f * 1.3f);
	frame->SetSize(size);
	scroll.x += vel;
	scroll.x = clamp(scroll.x, -3000.f, 340.f);
}

void MapScene::Finalize() {

}