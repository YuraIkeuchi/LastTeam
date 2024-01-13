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
	void ClearAction()override;
	void GameOverAction()override;//�Q�[���I�[�o�[�X�V
	void ImGui_Origin()override;
	void Draw(DirectXCommon* dxCommon) override;//�`��

		//�X�e�[�^�X������
	void InitState(const int width, const int height,const XMFLOAT3& pos);

private:
	//�֐��|�C���^
	static void(EnemyRock::* stateTable[])();

	bool RockCollide();

	void BirthParticle();

	void RockParticle();
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
	int m_ParticleTimer = {};
	bool m_Hit = false;
	XMFLOAT3 m_ReturnPos = {};
	//�V�F�C�N�p�ϐ�
	XMFLOAT3 m_ShakePos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_AfterPos = {};
	unique_ptr<Shake> shake;

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

	enum RockState {
		ROCK_MAGIC,
		ROCK_FOLLOW,
		ROCK_BIRTH,
	}_RockState = ROCK_MAGIC;
};
