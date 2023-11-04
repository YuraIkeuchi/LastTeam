#pragma once
#include "BaseScene.h"
#include "Passive.h"
#include"Font.h"
class MapScene : public BaseScene {
public:
	//初期化
	void Initialize(DirectXCommon* dxCommon) override;
	//更新
	void Update(DirectXCommon* dxCommon) override;
	//描画
	void Draw(DirectXCommon* dxCommon) override;
	//開放
	void Finalize() override;
private:
	//関数ポインタ
	static void(MapScene::* stateTable[])();

	enum class State :int{
		initState = 0,
		mainState,
		checkState
	}m_State=State::initState;
	
private:
	void InitState();//待機
	void MainState();
	void CheckState();
private:

	/// 描画
	void FrontDraw(DirectXCommon* dxCommon);
	void BackDraw(DirectXCommon* dxCommon);
private:

	struct UI {
		unique_ptr<IKESprite> sprite = nullptr;
		int nextIndex[3] = {};
		int hierarchy = -1;
		XMFLOAT2 pos = { 0,0 };
		XMFLOAT2 size = { 0,0 };
		XMFLOAT4 color = { 1.f,1.f,1.f,1.f };
		bool open = false;
		int Tag = BATTLE;
	};

	UI  RandPannel();

	void RoadUpdate();

	void MapCreate();

	void ImGuiDraw();

	void BlackOut();

	void Move();
private:
	enum {
		StartMAP = 0,
		NormalMAP,
		BossMAP,
		HealMAP,
		Max
	};
	enum {
		BATTLE = 1,
		BOSS,
		HEAL,
		TUTORIAL,
	};

	enum {
		Top = 0,
		Middle,
		Bottom,
		INDEX,
	};

	std::string dungeon;
	vector<int> dungeons;

	int clearHierarchy = 1;

	unique_ptr<IKESprite> screen = nullptr;
	unique_ptr<IKESprite> cheack= nullptr;
	unique_ptr<IKESprite> frame = nullptr;
	unique_ptr<IKESprite> chara = nullptr;

	array<array<UI, INDEX>, 20> UIs;
	float homeY[INDEX] = { 110.f,360.f,610.f };
	float homeX = 360.f;
	float interbal = 300.f;

	int nowIndex = Middle;
	int nowHierarchy = 0;

	int oldIndex = Middle;
	int oldHierarchy = 0;

	int MaxLength = 0;
	float lastScroll = 0;
	bool end = false;
	int pickIndex = Middle;
	int pickHierarchy = nowHierarchy + 1;
	int pickNextIndex = 0;

	int oldPickHis = pickHierarchy;
	int oldPickInd = pickIndex;


	XMFLOAT2 scroll = { 0,0 };
	int vel = 10;


	std::unique_ptr<Font> font = nullptr;

	std::vector<unique_ptr<IKESprite>> roads;
	std::vector<XMFLOAT2> roadsPos;


	std::vector<unique_ptr<IKESprite>> starRoads;
	std::vector<XMFLOAT2> starRoadsPos;

	bool moved = false;
	float mov_frame = 0.0f;
	float kMoveFrame = 10.0f;


	float eFrame = 0;
	float eAdd = 0.01f;

	XMFLOAT2 charaPos = { homeX ,homeY[Middle] };
	XMFLOAT2 framePos = { homeX + interbal ,homeY[Middle] };

};

