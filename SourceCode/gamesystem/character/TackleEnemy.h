#pragma once
#include"InterEnemy.h"
using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class TackleEnemy :public InterEnemy {
public:
	TackleEnemy();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	//�֐��|�C���^
	static void(TackleEnemy::* stateTable[])();

private:
	void Inter();//�ҋ@
	void Attack();
	void Standby();
protected:

private:
	
	int _charaState = STATE_INTER;
};

