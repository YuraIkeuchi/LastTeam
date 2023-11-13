#pragma once
#include"InterEnemy.h"
#include "EnemyBullet.h"

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
	void BirthBullet();//
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
	}_AttackState = ATTACK_BULLET;
};

