#pragma once
#include"InterEnemy.h"
using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class MobEnemy :public InterEnemy {
public:
	MobEnemy();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ClearAction()override;//�N���A�X�V
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	
private:

protected:

private:

};

