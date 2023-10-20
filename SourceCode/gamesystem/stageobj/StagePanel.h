#pragma once
#include "ObjCommon.h"
#include "CollisionPrimitive.h"
#include "AttackAction.h"
#include "GuardAction.h"
#include "SkillAction.h"
#include <any>
#include <array>
#include "IKESprite.h"

using namespace DirectX;
using namespace std;

static const int PANEL_HEIGHT = 4;
static const int PANEL_WIDTH = 8;
class StagePanel {
public:
	static StagePanel* GetInstance();
private:
	
	static const int DIR_MAX = 4;

	//�p�l���̎��
	enum PanelType {
		NO_PANEL = 0,
		ATTACK_PANEL,
		GUARD_PANEL,
		SKILL_PANEL,
		DAMAGE_PANNEL,
	};
	//�p�l��
	struct Panel {
		unique_ptr<IKEObject3d> object = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		int type = NO_PANEL;
		bool isHit = false;
	};

public:
	void LoadResource();
	//������
	bool Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();
	//�p�l����߂�
	void DeletePanel();

	void RandomPanel(int num);
	void ResetPanel();
private:
	//�o�g�����̍X�V
	void BattleUpdate();
	//�X�L���Z�b�g�̍X�V
	void SetUpdate();
	void Collide();
public:
	//gettersetter
	XMFLOAT3 SetPositon(int width, int height) {
		return panels[width][height].position;
	}
private:
	//�p�l��
	Panel panels[PANEL_WIDTH][PANEL_HEIGHT];
	//�X�L��
	vector<unique_ptr<InterAction>> actions;


	unique_ptr<IKESprite> skillUI = nullptr;
	unique_ptr<IKESprite> gaugeUI = nullptr;

	XMFLOAT2 basesize = {45.f,400.f};
	float kGaugeCountMax = 600;
	float gaugeCount = 0;

	//�}�X�̈ʒu
	int m_SelectHeight = 0;
	int m_SelectWidth = 0;

	//����
	enum SelectDir {
		DIR_UP,
		DIR_DOWN,
		DIR_RIGHT,
		DIR_LEFT
	};

	//�e�������̓t���[��
	array<int, DIR_MAX> m_Timer;

	//XMFLOAT3 m_SelectPos = {};

	OBB m_OBB1 = {}, m_OBB2 = {};
};