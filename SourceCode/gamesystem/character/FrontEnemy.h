#pragma once
#include"InterEnemy.h"
#include "Sickle.h"

using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class FrontEnemy :public InterEnemy {
public:
	FrontEnemy();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ClearAction()override;//�N���A�X�V
	void GameOverAction()override;//�Q�[���I�[�o�[�X�V
	void ImGui_Origin()override;
	void DeathSpecial()override;
	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	//�֐��|�C���^
	static void(FrontEnemy::* stateTable[])();
	static void(FrontEnemy::* attackTable[])();
private:
	void Inter();//�ҋ@
	void Attack();//�U��
	void Teleport();//�ړ�
	void WarpEnemy(bool Attack);//�G�̃��[�v����
	void AttackMove();//�U�����̓���

	//��������͍U�����̃X�e�[�g
	void FrontPlayerWarp();//�v���C���[�̑O
	void FrontAttack();//�U��
//���@�w
	void BirthMagic();
	void BirthArea(const int Width, const int Height);//�U���G���A
	void BirthPredict(const int Width, const int Height);//�\���G���A
private:
	static const int BULLET_NUM = 5;
private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
	
	enum AttackState {
		ATTACK_WARP,
		ATTACK_FRONT
	}_AttackState = ATTACK_WARP;
	std::vector<std::vector<int>> m_Area = {};
	// �U���G���A
	std::vector<unique_ptr<Sickle>> sickle;


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
	//�U���̃C���^�[�o���Ƃ�
	vector<int>m_AttackLimit;

	int m_ShotDir = {};
	float m_AfterRotY = {};
	bool m_ChangeRot = {};

	int m_AttackWidth = {};
	int m_AttackHeight = {};

	XMFLOAT3 m_SicklePos = {};
};

