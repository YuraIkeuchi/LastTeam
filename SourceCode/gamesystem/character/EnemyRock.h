#pragma once
#pragma once
#include"InterEnemy.h"

using namespace std;         //  ���O��Ԏw��
//���e�̓G
class EnemyRock :public InterEnemy {
public:
	EnemyRock();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//�`��

		//�X�e�[�^�X������
	void InitState(const int width, const int height);
private:
	//�֐��|�C���^
	static void(EnemyRock::* stateTable[])();

private:
	void Inter();//����
	void Attack();//�U��
	void Close();//�ҋ@
private:
	int m_NowWidth = {};
	int m_NowHeight = {};
	float m_Damage = {};
	std::string StateName = "NONE";
	//�㏸�x
	float m_AddPower = 0.0f;
	//�d�͉����x
	float m_Gravity = 0.02f;
	float m_BaseScale = {};
	float m_Frame = {};
	int m_Timer = {};
};
