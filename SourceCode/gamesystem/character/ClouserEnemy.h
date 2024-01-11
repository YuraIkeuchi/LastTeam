#pragma once
#include"InterEnemy.h"
#include "EnemyRock.h"

using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class ClouserEnemy :public InterEnemy {
public:
	ClouserEnemy();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ClearAction()override;//�N���A�X�V
	void GameOverAction()override;//�Q�[���I�[�o�[�X�V
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	//�֐��|�C���^
	static void(ClouserEnemy::* stateTable[])();
	static void(ClouserEnemy::* attackTable[])();
private:
	void Inter();//�ҋ@
	void Attack();//�U��
	void Teleport();//�ړ�
	void BirthArea(const int Width, const int Height);//�U���G���A
	void BirthPredict(const int Width, const int Height);//�\���G���A
	void WarpEnemy();//�G�̃��[�v����
	void AttackMove();//�U�����̓���
//���@�w
	void BirthMagic();

private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
	
	std::vector<std::vector<int>> m_Area = {};
	// ��G���A
	std::vector<unique_ptr<EnemyRock>> enerock;


	int m_RandWigth = {};
	int m_RandHeight = {};

	//���@�w�n
	enum MagicState {
		MAGIC_BIRTH,
		MAGIC_VANISH,
	};

	struct Magic {
		unique_ptr<IKETexture> tex;
		float Frame = {};
		float Scale = {};
		float AfterScale = {};
		XMFLOAT3 Pos = {};
		bool Alive = false;
		int State = {};
		int Timer = {};
	};
	Magic magic;

	bool m_Warp = false;

	enum WarpState {
		WARP_START,
		WARP_END,
	};

	struct EnemyWarp {
		float Frame = {};
		float Scale = {};
		float AfterScale = 0.5f;
		int State = {};
	};

	EnemyWarp enemywarp;
	//�C���^�[�o���Ƃ�
	vector<int>m_Limit;

	int m_ShotDir = {};
	float m_AfterRotY = {};
	bool m_ChangeRot = {};
};

