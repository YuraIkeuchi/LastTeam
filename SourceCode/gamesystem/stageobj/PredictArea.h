#pragma once
#include "ObjCommon.h"
#include <array>
#include "IKETexture.h"

using namespace DirectX;
using namespace std;

static const int PREDICT_HEIGHT = 4;
static const int PREDICT_WIDTH = 8;
class PredictArea {
public:
	PredictArea(const string& name);
private:

	//�p�l��
	struct Panel {
		unique_ptr<IKETexture> tex = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		bool predict = false;
	};

public:
	//������
	bool Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();

	void ResetPredict();

private:
	//�F�̃`�J�`�J�I��
	void FlashArea();
	
public:

	//gettersetter
	bool SetPredict(int width, int height, bool Flag) {
		return panels[width][height].predict = Flag;
	}

	void SetTimer(const int Timer) { m_Timer = Timer; }
	void SetTargetTimer(const int TargetTimer) { m_TargetTimer = TargetTimer; }
	void SetFlashStart(const bool FlashStart) { m_FlashStart = FlashStart; }
private:

	int m_Timer = {};
	int m_TargetTimer = {};
	//�p�l��
	Panel panels[PREDICT_WIDTH][PREDICT_HEIGHT];

	std::string name = "None";

	float m_SinAngle = {};
	float m_SinAngle2 = {};
	bool m_FlashStart = false;
	float m_AddAngle = {};
};