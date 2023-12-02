#pragma once
#include "ObjCommon.h"
#include "CollisionPrimitive.h"
#include "SkillAction.h"
#include <any>
#include <array>
#include "IKESprite.h"
#include "IKETexture.h"
#include "Player.h"
using namespace DirectX;
using namespace std;

static const int PANEL_HEIGHT = 4;
static const int PANEL_WIDTH = 8;
static const float PANEL_SIZE = 1.5f;
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
		unique_ptr<IKETexture> line = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		int type = NO_PANEL;
		bool isHit = false;
		bool isEnemyHit = false;
		bool predict = false;
		bool isPoison = false;
		int PoisonTimer = {};
	};

public:
	void LoadResource();
	//������
	bool Initialize(const float PosY = 0.0f);
	//�X�V
	void Update();
	//�`��
	void Draw(DirectXCommon* dxCommon);
	//�s���J�[�h�̕`��
	void ActDraw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();
	//�p�l����߂�
	void DeletePanel();

	void RandomPanel(int num);
	void ResetPanel();
	void ResetAction();
	void EnemyHitReset();
	void PoisonUpdate();
	//�F�ύX(�G)
	void SetEnemyHit(IKEObject3d* obj,int& width, int& height,bool m_Alive);
	//������ʒu�̃p�l���ԍ�������
	void SetPanelSearch(IKEObject3d* obj, int& width, int& height);
	//�G�̔z�u(���Ԃ�Ȃ��悤��)
	XMFLOAT3 EnemySetPanel(const bool LastEnemy);
	//�ł̔z�u(���Ԃ�Ȃ��悤��)
	void PoisonSetPanel(int& width, int& height);
	//�A�N�V�����̑S�폜
	void DeleteAction();
private:
	//�o�g�����̍X�V
	void BattleUpdate();
	//�X�L���Z�b�g�̍X�V
	void SetUpdate();
	XMFLOAT4 ChangeColor(const int Widht, const int Height);

public:
	static void SetPlayer(Player* player) { StagePanel::player = player; }
	//gettersetter
	XMFLOAT3 SetPositon(int width, int height) {
		return { panels[width][height].position.x,0.0f,panels[width][height].position.z };
	}

	//gettersetter
	bool SetPredict(int width, int height, bool Flag) {
		return panels[width][height].predict = Flag;
	}

	const bool GetAllDelete() { return m_AllDelete; }

private:
	static Player* player;
	//�p�l��
	Panel panels[PANEL_WIDTH][PANEL_HEIGHT];
	//�X�L��
	vector<unique_ptr<InterAction>> actions;


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

	//�X�L�������ׂĎ�ɓ���Ă��邩
	bool m_AllDelete = false;
	int m_ActionCount = {};
};