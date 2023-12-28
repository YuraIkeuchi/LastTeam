#pragma once
#include"InterEnemy.h"
#include "Shake.h"

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

	bool RockCollide();
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
	float m_BaseScale = 0.2f;
	float m_Frame = {};
	int m_Timer = {};
	bool m_Hit = false;
	XMFLOAT3 m_ReturnPos = {};
	//�V�F�C�N�p�ϐ�
	XMFLOAT3 m_ShakePos = { 0.0f,0.0f,0.0f };
	unique_ptr<Shake> shake;
};
