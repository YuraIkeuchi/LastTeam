#include "MapScene.h"
#include <ImageManager.h>
#include <Helper.h>
#include "CsvLoader.h"
#include <sstream>
#include <SceneChanger.h>
#include "SceneManager.h"
#include <TutorialTask.h>
#include <GameStateManager.h>
#include <StagePanel.h>


array<array<int, 3>, 15> MapScene::mapKinds;
bool MapScene::isStart = true;
void (MapScene::* MapScene::stateTable[])() = {
	&MapScene::InitState,//
	&MapScene::MainState,//
	&MapScene::CheckState,//
};


void MapScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);

	Audio::GetInstance()->LoopWave(AUDIO_MAIN, 0.02f);
	//ポストエフェクト
	PlayPostEffect = false;

	screen = IKESprite::Create(ImageManager::MAPSCREEN, { 0,0 });
	screen->SetSize({ 1280.f,720.f });

	cheack = IKESprite::Create(ImageManager::MAP_CHEACK, { 640.f,360.f });
	cheack->SetColor({1.2f,1.2f,1.2f,1});
	cheack->SetSize({ 0.f,0.f });
	cheack->SetAnchorPoint({ 0.5f,0.5f });
	if (s_Countinue) {		//コンティニューをしていた場合CSVからゲームデータ引き継ぎ(このシーンではマップデータ)
		GameStateManager::GetInstance()->OpenGameDate();
		nowHierarchy = GameStateManager::GetInstance()->GetHierarchy();
		nowIndex = GameStateManager::GetInstance()->GetIndex();
	}
	const int NumberCount = 2;
	const float l_Width_Cut = 320.0f;
	const float l_Height_Cut = 64.0f;
	for (auto i = 0; i < NumberCount; i++) {
		cheack_OK[i] = IKESprite::Create(ImageManager::MAP_CHEACK_OK, { 640.f,392.f });
		cheack_NO[i] = IKESprite::Create(ImageManager::MAP_CHEACK_NO, { 640.f,492.f });

		int number_index_y = i / NumberCount;
		int number_index_x = i % NumberCount;
		cheack_OK[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		cheack_NO[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });

		cheack_OK[i]->SetSize({ 0.f,0.f });
		cheack_OK[i]->SetAnchorPoint({ 0.5f,0.5f });
		cheack_OK[i]->SetColor({ 1.2f,1.2f,1.2f,1 });

		cheack_NO[i]->SetSize({ 0.f,0.f });
		cheack_NO[i]->SetAnchorPoint({ 0.5f,0.5f });
		cheack_NO[i]->SetColor({ 1.2f,1.2f,1.2f,1 });
	}
	comment[0] = IKESprite::Create(ImageManager::MAPCOMMENT00, { 1260.f-512.f,600.f });
	comment[1] = IKESprite::Create(ImageManager::MAPCOMMENT01, { 1260.f-512.f,600.f });
	comment[2] = IKESprite::Create(ImageManager::MAPCOMMENT02, { 1260.f-512.f,600.f });
	comment[3] = IKESprite::Create(ImageManager::MAPCOMMENT03, { 1260.f-512.f,600.f });

	nowComment = 0;

	UIs[0][Middle].sprite = IKESprite::Create(ImageManager::MAP_START, { 0,0 });
	UIs[0][Middle].pos = { homeX ,homeY[Middle] };
	UIs[0][Middle].open = true;
	UIs[0][Middle].hierarchy = 0;
	UIs[0][Middle].size = { 186.f,186.f };
	UIs[0][Middle].sprite->SetAnchorPoint({ 0.5f,0.5f });
	homeX += interbal;

	if (nowHierarchy == 0) {
		mapKinds[1] = { BATTLE,BATTLE,BATTLE };
		mapKinds[2] = { BATTLE,-1,PASSIVE };
		mapKinds[3] = { -1,BATTLE,-1 };
		mapKinds[4] = { PASSIVE,-1,BATTLE };
		mapKinds[5] = { -1,BOSS,-1 };
		mapKinds[6] = { PASSIVE,-1,BATTLE };
		mapKinds[7] = { PASSIVE,PASSIVE,PASSIVE };
		mapKinds[8] = { BATTLE,-1,PASSIVE };
		mapKinds[9] = { -1,BOSS,-1 };
		mapKinds[10] = { PASSIVE,PASSIVE,BATTLE };
		mapKinds[11] = { PASSIVE,BATTLE,BATTLE };
		mapKinds[12] = { BATTLE,PASSIVE,PASSIVE };
		mapKinds[13] = { -1,BOSS,-1 };
	}
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
	chara->SetSize({ 0.f,0.f });
	chara->SetAnchorPoint({ 0.5f,1.0f });


	frame = IKESprite::Create(ImageManager::MAP_FRAME, { 0,0 });
	frame->SetPosition(framePos);
	frame->SetSize({ 128.f,128.f });
	frame->SetAnchorPoint({ 0.5f,0.5f });


	startButton = IKESprite::Create(ImageManager::MAPSTART, { 0,0 });
	startButton->SetPosition({640.f,550.f});
	startButton->SetSize({ 512.f,192.f });
	startButton->SetAnchorPoint({ 0.5f,0.5f });

	onomatope = std::make_unique<Onomatope>();

	//道の処理
	{
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < INDEX; j++) {
				if (!UIs[i][j].sprite) { continue; }
				for (int l = 0; l < 3; l++) {
					if (UIs[i][j].nextIndex[l] == -1) { continue; }
					for (int k = 0; k < 10; k++) {
						int next = UIs[i][j].nextIndex[l];
						XMFLOAT2 pos = {
						Ease(In,Linear,(float)k / 10.0f,UIs[i][j].pos.x,UIs[i + 1][next].pos.x),
						Ease(In,Linear,(float)k / 10.0f,UIs[i][j].pos.y+30.f,UIs[i + 1][next].pos.y+30.f)
						};
						unique_ptr<IKESprite> road = IKESprite::Create(ImageManager::MAPROAD, pos);
						road->SetAnchorPoint({ 0.5f,0.5f });
						road->SetSize({ 16.f,16.f });
						roadsPos.push_back(std::move(pos));
						roads.push_back(std::move(road));
					}
				}
			}
		}
		for (int i = 0; i < 10; i++) {
			unique_ptr<IKESprite> road = IKESprite::Create(ImageManager::MAPROAD, {-100.f,0.f});
			road->SetAnchorPoint({ 0.5f,0.5f });
			road->SetSize({ 16.f,16.f });
			road->SetColor({ 1.f,1.f,0.f,1.f });
			starRoads.push_back(std::move(road));
		}
		starRoadsPos.resize(10);
	}
	for (int i = 0; i < MaxLength; i++) {
		for (int j = 0; j < INDEX; j++) {
			UIs[i][j].open = false;
		}
	}
	BlackOut();

	for (array<UI, INDEX>& ui : UIs) {
		for (int i = 0; i < INDEX; i++) {
			if (!ui[i].sprite) { continue; }
			ui[i].sprite->SetPosition({ ui[i].pos.x + scroll.x, ui[i].pos.y + scroll.y });
			ui[i].sprite->SetColor(ui[i].color);
			ui[i].sprite->SetSize(ui[i].size);
		}
	}

	for (int i = 0; i < roads.size(); i++) {
		roads[i]->SetPosition({ roadsPos[i].x + scroll.x,roadsPos[i].y + scroll.y });
	}
	lastScroll = MaxLength * interbal;
	if (nowHierarchy == 0) {
		scroll.x = -lastScroll;
		for (int i = 0; i < MaxLength; i++) {
			for (int j = 0; j < INDEX; j++) {
				UIs[i][j].open = false;
			}
		}
		charaSize = { 0.f,0.f };
		chara->SetSize(charaSize);
		m_State = State::initState;
		isStart = true;
	} else {
		m_State = State::mainState;
		scroll.x = -UIs[nowHierarchy][nowIndex].pos.x + 128.f;
		charaSize = { 128.f,128.f };
		chara->SetSize(charaSize);
	}

	//ここが新しく書いた場所
	pickHierarchy = nowHierarchy + 1;
	pickIndex = nowIndex;

	switch (UIs[pickHierarchy][pickIndex].Tag) {
	case BATTLE:
		nowComment = 1;
		break;
	case PASSIVE:
		nowComment = 2;
		break;
	case BOSS:
		nowComment = 3;
		break;
	default:
		break;
	}

	oldHierarchy = nowHierarchy;
	oldIndex = nowIndex;

	charaPos = { UIs[nowHierarchy][nowIndex].pos.x, UIs[nowHierarchy][nowIndex].pos.y + 30.f};
	framePos = UIs[pickHierarchy][pickIndex].pos;
	chara->SetPosition({ charaPos.x + scroll.x, charaPos.y + scroll.y });
	frame->SetPosition({ framePos.x + scroll.x, framePos.y + scroll.y });
	for (array<UI, INDEX>& ui : UIs) {
		for (int i = 0; i < INDEX; i++) {
			if (!ui[i].sprite) { continue; }
			ui[i].sprite->SetPosition({ ui[i].pos.x + scroll.x, ui[i].pos.y + scroll.y });
			ui[i].sprite->SetColor(ui[i].color);
			ui[i].sprite->SetSize(ui[i].size);
		}
	}
	for (int i = 0; i < roads.size(); i++) {
		roads[i]->SetPosition({ roadsPos[i].x + scroll.x,roadsPos[i].y + scroll.y });
	}

	GameStateManager::GetInstance()->SetMapData(nowIndex, nowHierarchy);
	if ((nowHierarchy == 0) || (nowHierarchy == 5 || nowHierarchy == 9) && !s_Countinue) {
		GameStateManager::GetInstance()->SaveGame();
	}
}

void MapScene::Update(DirectXCommon* dxCommon) {
	SceneChanger::GetInstance()->Update();
	if (m_State == State::initState &&
		SceneChanger::GetInstance()->GetChangeState() == 1) {
		return;
	}

	if (Helper::FrameCheck(eFrame, eAdd)) {
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

	onomatope->Update();
	(this->*stateTable[(size_t)m_State])();
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
	for (unique_ptr<IKESprite>& road : starRoads) {
		if (!end && !moved) {
			road->Draw();
		}
	}
	for (array<UI, INDEX>& ui : UIs) {
		for (int i = 0; i < INDEX; i++) {
			if (!ui[i].sprite) { continue; }
			ui[i].sprite->Draw();
		}
	}
	chara->Draw();
	if (!end) {
		frame->Draw();
	}
	onomatope->Draw();
	IKESprite::PostDraw();
	IKESprite::PreDraw();
	comment[nowComment]->Draw();
	cheack->Draw();
	cheack_OK[1 - nowCheack]->Draw();
	cheack_NO[nowCheack]->Draw();
	if (isStart&& m_State != State::initState) {
		startButton->Draw();
	}
	IKESprite::PostDraw();

	SceneChanger::GetInstance()->Draw();
}

void MapScene::BackDraw(DirectXCommon* dxCommon) {
}

MapScene::UI MapScene::RandPannel() {
	int min = ImageManager::MAP_NORMAL;
	int max = ImageManager::MAP_BOSS;
	int r = Helper::GetRanNum(min, max);

	UI itr;
	if (r == min) {
		itr.sprite = IKESprite::Create(ImageManager::MAP_NORMAL, { 0,0 });
		itr.Tag = BATTLE;
	} else if (r == max) {
		itr.sprite = IKESprite::Create(ImageManager::MAP_BOSS, { 0,0 });
		itr.Tag = BOSS;
	} else {
		itr.sprite = IKESprite::Create(ImageManager::MAP_HEAL, { 0,0 });
		itr.Tag = PASSIVE;

	}
	itr.size = { 128.f,128.f };
	itr.sprite->SetAnchorPoint({ 0.5f,0.5f });

	return itr;
}

MapScene::UI MapScene::TestPannel(int Index, int Hierarchy) {
	//[nowHierarchy] [nowIndex]
	int r = mapKinds[Hierarchy][Index];

	UI itr;
	switch (r) {
	case BATTLE:
		itr.sprite = IKESprite::Create(ImageManager::MAP_NORMAL, { 0,0 });
		itr.Tag = BATTLE;
		break;
	case PASSIVE:
		itr.sprite = IKESprite::Create(ImageManager::MAP_HEAL, { 0,0 });
		itr.Tag = PASSIVE;
		break;
	case BOSS:
		itr.sprite = IKESprite::Create(ImageManager::MAP_BOSS, { 0,0 });
		itr.Tag = BOSS;
		break;
	default:
		assert(0);
		break;
	}

	itr.size = { 186.f,186.f };
	itr.sprite->SetAnchorPoint({ 0.5f,0.5f });
	return itr;
}

void MapScene::RoadUpdate() {
	for (int k = 0; k < 10; k++) {
		XMFLOAT2 pos = {
		Ease(In,Linear,(float)k / 10.0f,UIs[nowHierarchy][nowIndex].pos.x,UIs[pickHierarchy][pickIndex].pos.x),
		Ease(In,Linear,(float)k / 10.0f,UIs[nowHierarchy][nowIndex].pos.y+30.f,UIs[pickHierarchy][pickIndex].pos.y + 30.f)
		};
		starRoadsPos[k] = pos;
	}
	for (int i = 0; i < 10; i++) {
		starRoads[i]->SetPosition({ starRoadsPos[i].x + scroll.x,starRoadsPos[i].y + scroll.y });
	}
}

void MapScene::MapCreate() {
	string csv_ = "Resources/csv/map.csv";
	int r_num = Helper::GetRanNum(0, 3);
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
			UIs[hierarchy][Middle] = TestPannel(Middle, (int)hierarchy);
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
			UIs[hierarchy][Top] = TestPannel(Top, (int)hierarchy);
			UIs[hierarchy][Top].pos = { homeX ,homeY[Top] };
			UIs[hierarchy][Top].hierarchy = i + 1;

			UIs[hierarchy][Bottom] = TestPannel(Bottom, (int)hierarchy);
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
				UIs[hierarchy][j] = TestPannel(j, (int)hierarchy);
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
					UIs[hierarchy][j].nextIndex[0] = -1;
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
	//チュートリアル(後で変える)
	UIs[1][Middle].sprite = IKESprite::Create(ImageManager::MAP_TUTORIAL, { 0,0 });
	UIs[1][Middle].Tag = TUTORIAL;
	UIs[1][Middle].sprite->SetPosition(UIs[1][Middle].pos);
	UIs[1][Middle].sprite->SetAnchorPoint({ 0.5f,0.5f });
}

void MapScene::ImGuiDraw() {
	ImGui::Begin("Map");
	//ImGui::Text("%f", framePos.x);
	//ImGui::Text("%f", eFrame);
	//ImGui::Text("HIERARCHY:%f", scroll.x);
	//ImGui::Text("PICKHIERARCHY:%f", -UIs[nowHierarchy][nowIndex].pos.x);
	//ImGui::Text("PICK:%d", -UIs[nowHierarchy][nowIndex].pos.x);
	//ImGui::Text("PICKINDEX:%d", pickIndex);
	//ImGui::Text("PosX:%f,PosY:%f", charaPos.x, charaPos.y);
	ImGui::Text("nowindel:%d", nowIndex);
	ImGui::Text("nowHie:%d", nowHierarchy);
	//for (int i = 0; i < 3; i++) {
	//	ImGui::Text("Index[%d]%d", i, UIs[nowHierarchy][nowIndex].nextIndex[i]);
	//}
	ImGui::End();
}

void MapScene::BlackOut() {

	for (int i = 0; i < clearHierarchy + 1; i++) {
		for (int j = 0; j < INDEX; j++) {
			UIs[i][j].open = true;
		}
	}


	for (array<UI, INDEX>& ui : UIs) {
		for (int i = 0; i < INDEX; i++) {
			if (!ui[i].sprite) { continue; }
			if (ui[i].open) {
				ui[i].color = { 1.0f,1.0f,1.0f,1.f };
			} else {
				ui[i].color = { 0.5f,0.5f,0.5f,1.f };
			}
		}
	}

}

void MapScene::Move() {
	Input* input = Input::GetInstance();
	if (end) { return; }
	if (input->PushButton(input->RB)) {
		scroll.x+=10;
	}
	if (input->PushButton(input->LB)) {
		scroll.x-=10;
	}
	if ((input->TiltStick(input->L_UP) || input->PushButton(input->UP) ||input->TriggerKey(DIK_W))
		&& !moved) {
		if (pickNextIndex == 0) { return; }
		if (UIs[nowHierarchy][nowIndex].nextIndex[pickNextIndex - 1] == -1) { return; }
		pickNextIndex--;
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Cursor.wav", 0.1f);
	}
	if ((input->TiltStick(input->L_DOWN)|| input->PushButton(input->DOWN) ||input->TriggerKey(DIK_S))
		&& !moved) {
		if (pickNextIndex == 2) { return; }
		if (UIs[nowHierarchy][nowIndex].nextIndex[pickNextIndex + 1] == -1) { return; }
		pickNextIndex++;
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Cursor.wav", 0.1f);
	}

	if ((input->TriggerButton(input->B)|| input->TriggerKey(DIK_SPACE)) && !moved) {
		if (isStart) {
			isStart = false;
		}
		nowIndex = pickIndex;
		nowHierarchy = pickHierarchy;
		clearHierarchy++;
		onomatope->AddOnomato(Foot, { 640.f,360.f });
		onomatope->AddOnomato(Foot, { 100.f,700.f }, 10.f);
		oldScroll = scroll.x;
		moved = true;

		switch (UIs[pickHierarchy][pickIndex].Tag) {
		case BATTLE:
			nowComment = 1;
			break;
		case PASSIVE:
			nowComment = 2;
			break;
		case BOSS:
			nowComment = 3;
			break;
		default:
			break;
		}

		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Run.wav", 0.05f);
	}

	if (!end) {
		pickIndex = UIs[oldHierarchy][oldIndex].nextIndex[pickNextIndex];
		framePos = UIs[pickHierarchy][pickIndex].pos;

		if (oldPickInd != pickIndex) {
			oldPickHis = pickHierarchy;
			oldPickInd = pickIndex;
		}

	}
	if (moved) {
		if (Helper::FrameCheck(mov_frame, 1 / kMoveFrame)) {
			onomatoFrame = 0.f;
			m_State = State::checkState;
			mov_frame = 0.0f;
			oldIndex = nowIndex;
			oldHierarchy = nowHierarchy;
			oldScroll = scroll.x;
			if (nowHierarchy != MaxLength) {
				pickHierarchy = nowHierarchy + 1;
				pickNextIndex = 0;
				pickIndex = UIs[oldHierarchy][oldIndex].nextIndex[pickNextIndex];
				framePos = UIs[pickHierarchy][pickIndex].pos;
			} else {
				pickHierarchy = 0;
				pickNextIndex = 0;
				end = true;
			}
			return;
		}

		charaPos.x = Ease(In, Quad, mov_frame, UIs[oldHierarchy][oldIndex].pos.x, UIs[nowHierarchy][nowIndex].pos.x);
		charaPos.y = Ease(In, Quad, mov_frame, UIs[oldHierarchy][oldIndex].pos.y + 30.f, UIs[nowHierarchy][nowIndex].pos.y+30.f);
		scroll.x = Ease(In, Quad, mov_frame, oldScroll, -UIs[nowHierarchy][nowIndex].pos.x + 128.f);
	}
	scroll.x = clamp(scroll.x, -lastScroll, 340.f);
}

void MapScene::Finalize() {

}

void MapScene::InitState() {
	const float addFrame = 1.0f / 45.f;
	const float addFrameS = 1.0f / 180.f;
	static float scrollFrame = 0.0f;
	static float s_frame = 0.0f;
	if (Helper::FrameCheck(scrollFrame, addFrameS)) {
		if (Helper::FrameCheck(s_frame, addFrame)) {
			m_State = State::mainState;
			scrollFrame = 0.0f;
			s_frame = 0.0f;
		} else {
			charaSize.x = Ease(In, Elastic, s_frame, 0.f, 186.f);
			charaSize.y = Ease(In, Linear, s_frame, 0.f, 186.f);
		}
	} else {
		scroll.x = Ease(In, Linear, scrollFrame, -lastScroll, 0.f);
	}
	chara->SetSize(charaSize);
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
	for (int i = 0; i < roads.size(); i++) {
		roads[i]->SetPosition({ roadsPos[i].x + scroll.x,roadsPos[i].y + scroll.y });
	}
}
void MapScene::MainState() {
	BlackOut();
	Move();
	RoadUpdate();
	if (isStart) {
		startAlpha += 0.02f;
		startButton->SetColor({1,1,1,abs(sinf(startAlpha))});
	}
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
	for (int i = 0; i < roads.size(); i++) {
		roads[i]->SetPosition({ roadsPos[i].x + scroll.x,roadsPos[i].y + scroll.y });
	}
}

void MapScene::CheckState() {
	const float addFrame = 1.0f / 15.f;
	if (SceneChanger::GetInstance()->GetChangeState() == 1) {
		delayFrame = 0.f;
		m_State = State::mainState;
	}

	if (UIs[nowHierarchy][nowIndex].Tag == TUTORIAL) {
		if (TutorialClosed()) { return; }
		if (Helper::FrameCheck(delayFrame, 1 / 20.f)) {
			if (Helper::FrameCheck(s_frame, addFrame)) {
				Input* input = Input::GetInstance();

				if ((input->TiltStick(input->L_UP)|| input->PushButton(input->UP) ||input->TriggerKey(DIK_W)) &&
					nowCheack == 1) {
					nowCheack--;
					cFrame = 0.f;
					cAdd = 0.03f;
					Audio::GetInstance()->PlayWave("Resources/Sound/SE/Cursor.wav", 0.1f);
				}
				if ((input->TiltStick(input->L_DOWN)|| input->PushButton(input->DOWN) || input->TriggerKey(DIK_S)) &&
					nowCheack == 0) {
					nowCheack++;
					cFrame = 0.f;
					cAdd = 0.03f;
					Audio::GetInstance()->PlayWave("Resources/Sound/SE/Cursor.wav", 0.1f);
				}

				if (input->TriggerButton(input->B)|| input->TriggerKey(DIK_SPACE)) {
					Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button.wav", 0.15f);
					moved = false;
					if (nowCheack == 0) {
						SceneChanger::GetInstance()->SetChangeStart(true);
					} else {
						isClose = true;
					}
				}
			} else {
				size_c.x = Ease(Out, Quint, s_frame, 0.f, 640.f);
				size_c.y = Ease(Out, Quint, s_frame, 0.f, 480.f);
				cheack->SetSize(size_c);
				cheackSize.x = Ease(Out, Quint, s_frame, 0.f, 320.f);
				cheackSize.y = Ease(Out, Quint, s_frame, 0.f, 64.f);
				for (int i = 0; i < 2; i++) {
					cheack_OK[i]->SetSize(cheackSize);
					cheack_NO[i]->SetSize(cheackSize);
				}
			}

		}
		if (SceneChanger::GetInstance()->GetChange()) {
			TutorialTask::GetInstance()->SetViewSkill(false);
			SceneManager::GetInstance()->ChangeScene("TUTORIAL");
			SceneChanger::GetInstance()->SetChange(false);
			cheack->SetSize({ 0.0f,0.0f });
		} else {
			if (s_frame==1.0f) {
				if (Helper::FrameCheck(cFrame, cAdd)) {
					cAdd *= -1.0f;
					cFrame = 0.99f;
				}
				if (cFrame == 0.0f) {
					cAdd *= -1.0f;
				}
				XMFLOAT2 cheackSize_ = cheack_OK[1]->GetSize();
				cheackSize_.x = Ease(InOut, Quad, cFrame, 320.f, 320.f * 1.1f);
				cheackSize_.y = Ease(InOut, Quad, cFrame, 64.f, 64.f * 1.1f);
				cheack_OK[1]->SetSize(cheackSize_);
				cheack_NO[1]->SetSize(cheackSize_);
			}
		}
	} else {
		if (Helper::FrameCheck(delayFrame, 1 / 20.f)) {
			SceneChanger::GetInstance()->SetChangeStart(true);
			int num = Helper::GetRanNum(1, 3);
			std::stringstream ss;
			const std::string BaseName = "Resources/csv/EnemySpawn/";
			string levelName = "None";
			bool isBattle = true;
			if (nowHierarchy == 5) {
				ss << BaseName + "Boss/BattleMap0" << 1 << ".csv";
			}
			else if (nowHierarchy == 9) {
				ss << BaseName + "Boss/BattleMap0" << 2 << ".csv";
			}
			else if (nowHierarchy == 13) {
				ss << BaseName + "Boss/BattleMap0" << 3 << ".csv";
				s_LastStage = true;
			}
			else {
				if (nowHierarchy < 5) {	//なんマス目に居るかで難易度が変わる
					levelName = "Weak";
				} else if(nowHierarchy >= 6 && nowHierarchy < 9) {
					levelName = "Strong";
				}
				else if (nowHierarchy >= 10) {
					levelName = "Ultimate";
				}
				if (UIs[nowHierarchy][nowIndex].Tag == BATTLE) {
					ss << BaseName + levelName + "/BattleMap0" << 3 << ".csv";
					isBattle = true;
				} else if (UIs[nowHierarchy][nowIndex].Tag == PASSIVE) {
					ss << BaseName + levelName + "/PassiveMap0" << num << ".csv";
					isBattle = false;
				} else if (UIs[nowHierarchy][nowIndex].Tag == BOSS) {
					ss << BaseName + "Boss/BattleMap0" << 1 << ".csv";
					isBattle = true;
				}
			}
			std::string r_map = ss.str();
			GameStateManager::GetInstance()->SetEnemySpawnText(r_map, isBattle);
			delayFrame = 0.f;
		}
		if (SceneChanger::GetInstance()->GetChange()) {
			TutorialTask::GetInstance()->SetViewSkill(true);
			SceneManager::GetInstance()->ChangeScene("BATTLE");
			SceneChanger::GetInstance()->SetChange(false);
		}
	}

}

bool MapScene::TutorialClosed() {
	if (!isClose) { return false; }
	const float addFrame = 1.0f / 15.f;

	if (Helper::FrameCheck(closeFrame, addFrame)) {
		m_State = State::mainState;
		delayFrame = 0.f;
		closeFrame = 0.f;
		isClose = false;
		return true;
	} else {
		size_c.x = Ease(Out, Quint, closeFrame, 640.f, 0.f);
		size_c.y = Ease(Out, Quint, closeFrame, 480.f, 0.f);
		cheackSize.x = Ease(Out, Quint, closeFrame, 320.f, 0.f);
		cheackSize.y = Ease(Out, Quint, closeFrame, 64.f, 0.f);
		cheack->SetSize(size_c);
		for (int i = 0; i < 2; i++) {
			cheack_OK[i]->SetSize(cheackSize);
			cheack_NO[i]->SetSize(cheackSize);
		}
		return true;
	}
}