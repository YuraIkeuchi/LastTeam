#pragma once
#include "BaseScene.h"
#include "Passive.h"

class MapScene : public BaseScene {
public:
	//èâä˙âª
	void Initialize(DirectXCommon* dxCommon) override;
	//çXêV
	void Update(DirectXCommon* dxCommon) override;
	//ï`âÊ
	void Draw(DirectXCommon* dxCommon) override;
	//äJï˙
	void Finalize() override;
private:
	/// ï`âÊ
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

	unique_ptr<IKESprite>screen = nullptr;
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

	int pickIndex = Middle;
	int pickHierarchy = nowHierarchy + 1;
	int pickNextIndex = 0;

	XMFLOAT2 scroll = { 0,0 };
	int vel = 10;


	std::vector<unique_ptr<IKESprite>> roads;
	std::vector<XMFLOAT2> roadsPos;

	bool moved = false;
	float mov_frame = 0.0f;
	float kMoveFrame = 10.0f;


	float eFrame = 0;
	float eAdd = 0.01f;

	XMFLOAT2 charaPos = { homeX ,homeY[Middle] };
	XMFLOAT2 framePos = { homeX + interbal ,homeY[Middle] };

};

