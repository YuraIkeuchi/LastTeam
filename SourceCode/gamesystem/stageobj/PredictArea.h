#pragma once
#include "ObjCommon.h"
#include <array>
#include "IKETexture.h"
#include "StagePanel.h"

using namespace DirectX;
using namespace std;

static const int PREDICT_HEIGHT = 4;
static const int PREDICT_WIDTH = 8;
enum PredictType {
	PREDICT_ATTACK,
	PREDICT_BUFF,
	PREDICT_HATENA,
	PREDICT_HEAL,
};
class PredictArea {
public:
	PredictArea(const string& name);

private:
	static const int PREDICT_MAX = 4;
	float PANELS_SIZE = PANEL_SIZE * 0.1f;

private:

	//�p�l��
	struct Panel {
		unique_ptr<IKETexture> tex[PREDICT_MAX];
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT3 afterPos = {};
		XMFLOAT3 beforePos = {};
		XMFLOAT4 color = { 1,1,1,1 };
		XMFLOAT3 scale = {};
		bool predict = false;

		float frame = 0.f;
		bool isVisible = false;
		bool isVerse = false;
		bool isVanish = false;
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
	void VersePredict(int width, int height);
	void VanishPredict(int width, int height);

	void SetTimer(const int Timer) { m_Timer = Timer; }
	void SetDrawDype(const int DrawDype) { m_DrawDype = DrawDype; }
	void SetTargetTimer(const int TargetTimer) { m_TargetTimer = TargetTimer; }
	void SetFlashStart(const bool FlashStart) { m_FlashStart = FlashStart; }
private:
	int m_DrawDype = PREDICT_ATTACK;
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