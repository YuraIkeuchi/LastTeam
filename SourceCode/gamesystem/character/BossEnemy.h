#pragma once
#include"InterEnemy.h"
#include "EnemyBullet.h"
#include "EnemyThorn.h"

using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class BossEnemy :public InterEnemy {
public:
	BossEnemy();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	//�֐��|�C���^
	static void(BossEnemy::* stateTable[])();
	static void(BossEnemy::* attackTable[])();
private:
	void Inter();//�ҋ@
	void Attack();//�U��
	void Teleport();//�ړ�
	void BulletAttack();//�e�̍U��
	void RowAttack();
	void RandomAttack();
	void BirthBullet();//
	void BirthArea(const int Width,const int Height,const string& name);//�U���G���A
	void BirthPredict(const int Width,const int Height,const string& name);//�\���G���A

	//�X�L����CSV��ǂݎ��
	void LoadCsvSkill(std::string& FileName, const int id);
	//CSV���쐬
	bool CreateSkill(int id);
	void PlayerCollide();
	void WarpEnemy();//�G�̃��[�v����
	void AttackMove();//�U�����̓���
//���@�w
	void BirthMagic();
private:
	static const int BULLET_NUM = 5;
private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
	vector<unique_ptr<EnemyBullet>> bullets;//�|���^�[�K�C�X�g

	enum BossType {
		Boss_SET,
		Boss_THROW,
		Boss_END,
	}_BossType = Boss_SET;

	enum AttackState {
		ATTACK_BULLET,
		ATTACK_ROW,
		ATTACK_RANDOM,
	}_AttackState = ATTACK_RANDOM;
	std::vector<std::vector<int>> m_Area = {};
	// �U���G���A
	std::vector<unique_ptr<EnemyThorn>> enethorn;
	

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
	int m_BulletNum = {};

	float m_RotFrame = {};
	int m_ShotDir = {};
	float m_AfterRotY = {};
	bool m_ChangeRot = {};

	
};

