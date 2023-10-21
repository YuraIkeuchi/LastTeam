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

	string dungeon;
	vector<int> dungeons;

	unique_ptr<IKESprite>screen = nullptr;
	unique_ptr<IKESprite> frame = nullptr;


	array<UI, 20> UIs;
	XMFLOAT2 basePos[7] = {
		{340.f,360.f},
		{640.f,360.f},
		{940.f,360.f},
		{1240.0f,360.f},
		{1540.0f,360.f},
		{1840.0f,360.f},
		{2140.0f,360.f},
	};
	XMFLOAT2 interbal = { 300.f,200.f };
	XMFLOAT2 scroll = { 0,0 };
	int vel = 10;


	bool moved = false;
	float mov_frame = 0.0f;
	float kMoveFrame = 10.0f;


	float eFrame = 0;
	float eAdd = 0.01f;

	int oldMap = 0;
	int nowMap = 0;
	XMFLOAT2 framePos= basePos[0];
};

