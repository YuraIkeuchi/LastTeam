#pragma once
#include"InterEnemy.h"

using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class Bomb :public InterEnemy {
public:
	Bomb();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	//�֐��|�C���^
	static void(Bomb::* stateTable[])();

private:
	void Inter();//�ҋ@
	void Attack();//�U��
private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
	bool _isPlayerDamage = false;
	bool _isEnemyDamage = false;

	enum BombType {
		Bomb_SET,
		Bomb_THROW,
		Bomb_END,
	}_BombType = Bomb_SET;
};
