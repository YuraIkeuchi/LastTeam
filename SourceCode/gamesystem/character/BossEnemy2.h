#pragma once
#include"InterEnemy.h"
#include "EnemyBullet.h"
#include "EnemyThorn.h"
#include "ConfuEffect.h"
using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class BossEnemy2 :public InterEnemy {
public:
	BossEnemy2();
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
	static void(BossEnemy2::* stateTable[])();
	static void(BossEnemy2::* attackTable[])();
private:
	void Inter();//�ҋ@
	void Attack();//�U��
	void Teleport();//�ړ�
	void SpinningAttack();
	void ShockWaveAttack();
	void Recovery();
	void Stun();

	void BirthArea(const int Width, const int Height, const string& name);//�U���G���A
	void BirthPredict(const int Width, const int Height, const string& name);//�\���G���A
	void BirthChantingHealParticle();

	void SpinningAttackBirthPredict(int AttackCount);
	void SpinningAttackBirthArea(int AttackCount);
	void ShockWaveAttackBirthPredict(int AttackCount);
	void ShockWaveAttackBirthArea(int AttackCount);

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
	unique_ptr<ConfuEffect> confueffect;
	int m_AttackCount = {};
	int _charaState = STATE_INTER;

	enum BossType {
		Boss_SET,
		Boss_THROW,
		Boss_END,
	}_BossType = Boss_SET;

	enum AttackState {
		ATTACK_SPINNING,
		ATTACK_SHOCKWAVE,
		ATTACK_RECOVERY,
		ATTACK_STUN
	}_AttackState = ATTACK_SPINNING;
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

	float m_RotFrame = {};
	int m_ShotDir = {};
	float m_AfterRotY = {};
	bool m_ChangeRot = {};
	bool m_RecoverySaveHP = {};
	float m_TmpHP = {};
	bool m_isStun = {};
	int m_ChantingCount = {};
};

