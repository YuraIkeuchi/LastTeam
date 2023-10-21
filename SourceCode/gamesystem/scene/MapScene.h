#pragma once
#include "BaseScene.h"

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
		XMFLOAT2 pos = { 0,0 };
		XMFLOAT2 size = { 0,0 };
		int Tag = BATTLE;
	};

	UI  RandPannel();

	void MapCreate();

	void ImGuiDraw();

private:
	enum {
		StartMAP = 0,
		NormalMAP,
		BossMAP,
		HealMAP,
		Max
	};
	enum {
		Tutorial = 0,
		FirstChoice = 3,
		SecondChoice = 8,
		ThirdChoice = 15,
		MaxCHOICE,
	};
	enum {
		BATTLE = 1,
		BOSS,
		HEAL,
	};

	vector<string> dungeon;
	vector<int> dungeons;
	//map<int, string> randMaps;
	unique_ptr<IKESprite>screen = nullptr;

	array<UI, MaxCHOICE> UIs;
	XMFLOAT2 basePos[5] = {
		{340.f,360.f},
		{640.f,360.f},
		{940.f,360.f},
		{1240.0f,-240.f},
		{1540.0f,-560.f}
	};
	XMFLOAT2 interbal = { 300.f,200.f };
	XMFLOAT2 scroll = { 0,0 };
};

