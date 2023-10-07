#pragma once
#include "ObjCommon.h"
#include "CollisionPrimitive.h"
#include <any>
#include <array>
using namespace DirectX;
using namespace std;
class StagePanel
{
public:
	static StagePanel* GetInstance();

private:
	static const int PANEL_HEIGHT = 4;
	static const int PANEL_WIDTH = 8;

	static const int DIR_MAX = 4;
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

	//�p�l���̕ύX
	void PanelChange(const string& Tag);
	//�p�l����߂�
	void DeletePanel();
private:
	//�o�g�����̍X�V
	void BattleUpdate();
	//�X�L���Z�b�g�̍X�V
	void SetUpdate();
	void Collide();
public:
	//gettersetter
	const XMFLOAT3& GetSelectPos() { return m_SelectPos; }
	const bool GetCanSet() { return m_CanSet; }

	void SetSelectPos(const XMFLOAT3& position) { m_SelectPos = position; }
private:
	unique_ptr<IKEObject3d> m_Object[PANEL_WIDTH][PANEL_HEIGHT];
	XMFLOAT3 m_Position[PANEL_WIDTH][PANEL_HEIGHT] = {};
	XMFLOAT4 m_Color[PANEL_WIDTH][PANEL_HEIGHT] = {};

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

	XMFLOAT3 m_SelectPos = {};

	//�p�l���̎��
	enum PanelType {
		NO_PANEL,
		ATTACK_PANEL,
		GUARD_PANEL,
		SKILL_PANEL,
	};

	int m_PanelType[PANEL_WIDTH][PANEL_HEIGHT] = {};
	bool m_PanelHit[PANEL_WIDTH][PANEL_HEIGHT];
	
	//�p�l����u���邩�ǂ���
	bool m_CanSet = false;

	OBB m_OBB1 = {}, m_OBB2 = {};
};