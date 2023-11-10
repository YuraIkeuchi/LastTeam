#pragma once
#include"InterEnemy.h"
#include "EnemyBullet.h"

using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class CanonEnemy :public InterEnemy {
public:
	CanonEnemy();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	//�֐��|�C���^
	static void(CanonEnemy::* stateTable[])();

private:
	void Inter();//�ҋ@
	void Attack();//�U��
	void Teleport();//�ړ�
	void BirthBullet();//
private:
static const int BULLET_NUM = 3;
private:
	int _charaState = STATE_INTER;
	vector<unique_ptr<EnemyBullet>> bullets;//�|���^�[�K�C�X�g
};

