#pragma once
#include"InterEnemy.h"
using namespace std;         //  ���O��Ԏw��
//�^�C�g���̓G
class TitleEnemy :public InterEnemy {
public:
	TitleEnemy();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//�`��

private:

};

