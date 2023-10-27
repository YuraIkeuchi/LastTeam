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


	UIs[0][Middle].sprite = IKESprite::Create(ImageManager::MAP_START, { 0,0 });
	UIs[0][Middle].pos = { homeX ,homeY[Middle] };
	UIs[0][Middle].open = true;
	UIs[0][Middle].hierarchy = 0;
	UIs[0][Middle].size = { 128.f,128.f };
	UIs[0][Middle].sprite->SetAnchorPoint({ 0.5f,0.5f });
	homeX += interbal;

	MapCreate();

	switch (dungeons[0]) {
	case 1:
		UIs[0][Middle].nextIndex[0] = 1;
		UIs[0][Middle].nextIndex[1] = -1;
		UIs[0][Middle].nextIndex[2] = -1;
		break;
	case 2:
		UIs[0][Middle].nextIndex[0] = 0;
		UIs[0][Middle].nextIndex[1] = 2;
		UIs[0][Middle].nextIndex[2] = -1;
		break;
	case 3:
		UIs[0][Middle].nextIndex[0] = 0;
		UIs[0][Middle].nextIndex[1] = 1;
		UIs[0][Middle].nextIndex[2] = -1;
		break;
	default:
		break;
	}

	chara = IKESprite::Create(ImageManager::MAP_CHARA, { 0,0 });
	chara->SetPosition(charaPos);
	chara->SetSize({ 128.f,128.f });
	chara->SetAnchorPoint({ 0.5f,1.0f });


	frame = IKESprite::Create(ImageManager::MAP_FRAME, { 0,0 });
	frame->SetPosition(framePos);
	frame->SetSize({ 128.f,128.f });
	frame->SetAnchorPoint({ 0.5f,0.5f });

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < INDEX; j++) {
			if (!UIs[i][j].sprite) { continue; }
			for (int l = 0; l < 3; l++) {
				if (UIs[i][j].nextIndex[l] == -1) { continue; }
				for (int k = 0; k < 10; k++) {
					int next = UIs[i][j].nextIndex[l];
					XMFLOAT2 pos = {
					Ease(In,Linear,(float)k / 10.0f,UIs[i][j].pos.x,UIs[i + 1][next].pos.x),
					Ease(In,Linear,(float)k / 10.0f,UIs[i][j].pos.y,UIs[i + 1][next].pos.y)
					};
					unique_ptr<IKESprite> road = IKESprite::Create(ImageManager::FEED, pos);
					road->SetAnchorPoint({ 0.5f,0.5f});
					road->SetSize({10.f,10.f});
					roadsPos.push_back(std::move(pos));
					roads.push_back(std::move(road));
				}
			}
		}
	}
}

void MapScene::Update(DirectXCommon* dxCommon) {
	BlackOut();
	Move();
	for (array<UI, INDEX>& ui : UIs) {
		for (int i = 0; i < INDEX; i++) {
			if (!ui[i].sprite) { continue; }
			ui[i].sprite->SetPosition({ ui[i].pos.x + scroll.x, ui[i].pos.y + scroll.y });
			ui[i].sprite->SetColor(ui[i].color);
			ui[i].sprite->SetSize(ui[i].size);
		}
	}
	chara->SetPosition({ charaPos.x + scroll.x, charaPos.y + scroll.y });
	frame->SetPosition({ framePos.x + scroll.x, framePos.y + scroll.y });
	for (int i = 0; i < roads.size();i++) {
		roads[i]->SetPosition({ roadsPos[i].x + scroll.x,roadsPos[i].y + scroll.y });
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
	for (unique_ptr<IKESprite>& road : roads) {
		road->Draw();
	}
	for (array<UI, INDEX>& ui : UIs) {
		for (int i = 0; i < INDEX; i++) {
			if (!ui[i].sprite) { continue; }
			ui[i].sprite->Draw();
		}
	}
	chara->Draw();
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

void MapScene::RoadUpdate() {




}

void MapScene::MapCreate() {
	string csv_ = "Resources/csv/map.csv";
	int r_num = Helper::GetInstance()->GetRanNum(0, 3);
	//テストプレイ用
	r_num = 4;
	//mapのあとに数字をくっつける
	std::stringstream ss;
	ss << "map" << r_num;
	std::string r_map = ss.str();
	//指定してゲットする
	LoadCSV::LoadCsvParam_String(csv_, dungeon, r_map);
	//けたすうしゅとくする
	int Len = (int)dungeon.length();
	MaxLength = Len;
	dungeons.resize(Len);
	//1文字ずつ格納
	for (int i = 0; i < Len; ++i) {
		dungeons[i] = (int)(dungeon[i] - '0');
	}
	for (int i = 0; i < Len; ++i) {
		//この+1はスタートを除く
		size_t hierarchy = (size_t)i + 1;
		switch (dungeons[i]) {
		case 1: {
			UIs[hierarchy][Middle] = RandPannel();
			UIs[hierarchy][Middle].pos = { homeX ,homeY[Middle] };
			UIs[hierarchy][Middle].hierarchy = i + 1;
			if (i != Len - 1) {
				switch (dungeons[i + 1]) {
				case 1:
					UIs[hierarchy][Middle].nextIndex[0] = 1;
					UIs[hierarchy][Middle].nextIndex[1] = -1;
					UIs[hierarchy][Middle].nextIndex[2] = -1;
					break;
				case 2:
					UIs[hierarchy][Middle].nextIndex[0] = 0;
					UIs[hierarchy][Middle].nextIndex[1] = 2;
					UIs[hierarchy][Middle].nextIndex[2] = -1;
					break;
				case 3:
					UIs[hierarchy][Middle].nextIndex[0] = 0;
					UIs[hierarchy][Middle].nextIndex[1] = 1;
					UIs[hierarchy][Middle].nextIndex[2] = 2;
					break;
				default:
					break;
				}
			} else {
				UIs[hierarchy][Middle].nextIndex[0] = -1;
				UIs[hierarchy][Middle].nextIndex[1] = -1;
				UIs[hierarchy][Middle].nextIndex[2] = -1;

			}
			break;
		}
		case 2: {
			UIs[hierarchy][Top] = RandPannel();
			UIs[hierarchy][Top].pos = { homeX ,homeY[Top] };
			UIs[hierarchy][Top].hierarchy = i + 1;

			UIs[hierarchy][Bottom] = RandPannel();
			UIs[hierarchy][Bottom].pos = { homeX ,homeY[Bottom] };
			UIs[hierarchy][Bottom].hierarchy = i + 1;

			if (i != Len - 1) {
				switch (dungeons[i + 1]) {
				case 1:
					UIs[hierarchy][Top].nextIndex[0] = 1;
					UIs[hierarchy][Top].nextIndex[1] = -1;
					UIs[hierarchy][Top].nextIndex[2] = -1;
					UIs[hierarchy][Bottom].nextIndex[0] = 1;
					UIs[hierarchy][Bottom].nextIndex[1] = -1;
					UIs[hierarchy][Bottom].nextIndex[2] = -1;
					break;
				case 2:
					UIs[hierarchy][Top].nextIndex[0] = 0;
					UIs[hierarchy][Top].nextIndex[1] = 2;
					UIs[hierarchy][Top].nextIndex[2] = -1;
					UIs[hierarchy][Bottom].nextIndex[0] = 0;
					UIs[hierarchy][Bottom].nextIndex[1] = 2;
					UIs[hierarchy][Bottom].nextIndex[2] = -1;
					break;
				case 3:
					UIs[hierarchy][Top].nextIndex[0] = 0;
					UIs[hierarchy][Top].nextIndex[1] = 1;
					UIs[hierarchy][Top].nextIndex[2] = 2;
					UIs[hierarchy][Bottom].nextIndex[0] = 0;
					UIs[hierarchy][Bottom].nextIndex[1] = 1;
					UIs[hierarchy][Bottom].nextIndex[2] = 2;
					break;
				default:
					break;
				}
			} else {
				UIs[hierarchy][Top].nextIndex[0] = -1;
				UIs[hierarchy][Top].nextIndex[1] = -1;
				UIs[hierarchy][Top].nextIndex[2] = -1;
				UIs[hierarchy][Bottom].nextIndex[0] = -1;
				UIs[hierarchy][Bottom].nextIndex[1] = -1;
				UIs[hierarchy][Bottom].nextIndex[2] = -1;

			}
			break;
		}
		case 3: {
			for (int j = 0; j < 3; j++) {
				UIs[hierarchy][j] = RandPannel();
				UIs[hierarchy][j].pos = { homeX ,homeY[j] };
				UIs[hierarchy][j].hierarchy = i + 1;
				if (i != Len - 1) {
					switch (dungeons[i + 1]) {
					case 1:
						UIs[hierarchy][j].nextIndex[0] = 1;
						UIs[hierarchy][j].nextIndex[1] = -1;
						UIs[hierarchy][j].nextIndex[2] = -1;
						break;
					case 2:
						UIs[hierarchy][j].nextIndex[0] = 0;
						UIs[hierarchy][j].nextIndex[1] = 2;
						UIs[hierarchy][j].nextIndex[2] = -1;
						break;
					case 3:
						UIs[hierarchy][j].nextIndex[0] = 0;
						UIs[hierarchy][j].nextIndex[1] = 1;
						UIs[hierarchy][j].nextIndex[2] = 2;
						break;
					default:
						break;
					}
				} else {
					UIs[hierarchy][j].nextIndex[0] = -11;
					UIs[hierarchy][j].nextIndex[1] = -1;
					UIs[hierarchy][j].nextIndex[2] = -1;
				}
			}
			break;
		}
		default:
			break;
		}
		homeX += interbal;
	}

}



void MapScene::ImGuiDraw() {
	ImGui::Begin("Map");
	ImGui::Text("%f", framePos.x);
	ImGui::Text("%f", eFrame);
	ImGui::Text("HIERARCHY:%d", UIs[nowHierarchy][nowIndex].hierarchy);
	ImGui::Text("PICKHIERARCHY:%d", UIs[pickHierarchy][pickIndex].hierarchy);
	ImGui::Text("PICKINDEX:%d", pickIndex);
	//ImGui::Text("PICKNow:%d", pickNow);
	//for (int i = 0; i < 3; i++) {
	//	ImGui::Text("Index[%d]%d", i, UIs[nowHierarchy][nowIndex].nextIndex[i]);
	//}
	ImGui::End();
}

void MapScene::BlackOut() {

	for (int i = 0; i < clearHierarchy;i++) {
		for (int j = 0; j < INDEX;j++) {
			UIs[i][j].open = true;
		}
	}


	for (array<UI, INDEX>& ui : UIs) {
		for (int i = 0; i < INDEX;i++) {
			if (!ui[i].sprite) { continue; }
			if (ui[i].open) { continue; }
			ui[i].color = {0.5f,0.5f,0.5f,1.f};
		}
	}

}

void MapScene::Move() {
	Input* input = Input::GetInstance();
	int vel = 0;
	if (input->PushButton(input->LB)) {
		vel = -10;
	} else if (input->PushButton(input->RB)) {
		vel = 10;
	}

	if (input->TiltStick(input->L_UP)) {
		if (moved) { return; }
		if (pickNextIndex == 0) { return; }
		if (UIs[nowHierarchy][nowIndex].nextIndex[pickNextIndex - 1] == -1) { return; }
		pickNextIndex--;
	}
	if (input->TiltStick(input->L_DOWN)) {
		if (moved) { return; }
		if (pickNextIndex == 2) { return; }
		if (UIs[nowHierarchy][nowIndex].nextIndex[pickNextIndex + 1] == -1) { return; }
		pickNextIndex++;
	}

	if (input->TriggerButton(input->B)) {
		if (moved) { return; }
		nowIndex = pickIndex;
		nowHierarchy = pickHierarchy;
		moved = true;
	}
		pickIndex = UIs[oldHierarchy][oldIndex].nextIndex[pickNextIndex];
		framePos = UIs[pickHierarchy][pickIndex].pos;

	if (moved) {
		if (Helper::GetInstance()->FrameCheck(mov_frame, 1 / kMoveFrame)) {
			moved = false;
			oldIndex = nowIndex;
			oldHierarchy = nowHierarchy;
			if (pickHierarchy < MaxLength) {
				pickHierarchy = nowHierarchy + 1;
			}
			pickNextIndex = 0;
			mov_frame = 0.0f;
			return;
		}
		charaPos.x = Ease(In, Quad, mov_frame, UIs[oldHierarchy][oldIndex].pos.x, UIs[nowHierarchy][nowIndex].pos.x);
		charaPos.y = Ease(In, Quad, mov_frame, UIs[oldHierarchy][oldIndex].pos.y, UIs[nowHierarchy][nowIndex].pos.y);
		scroll.x = Ease(In, Quad, mov_frame, scroll.x, -UIs[nowHierarchy][nowIndex].pos.x / 2);
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