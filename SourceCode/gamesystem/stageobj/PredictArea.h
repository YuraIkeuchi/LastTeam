#pragma once
#include "ObjCommon.h"
#include <array>

using namespace DirectX;
using namespace std;

static const int PREDICT_HEIGHT = 4;
static const int PREDICT_WIDTH = 8;
class PredictArea {
public:
	PredictArea();
private:

	//パネル
	struct Panel {
		unique_ptr<IKEObject3d> object = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		bool predict = false;
	};

public:
	//初期化
	bool Initialize();
	//更新
	void Update();
	//描画
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();

	void ResetPredict();
public:

	//gettersetter
	bool SetPredict(int width, int height, bool Flag) {
		return panels[width][height].predict = Flag;
	}
private:
	//パネル
	Panel panels[PREDICT_WIDTH][PREDICT_HEIGHT];
};