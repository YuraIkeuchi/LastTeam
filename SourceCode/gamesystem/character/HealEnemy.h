#pragma once
#include"InterEnemy.h"

using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class HealEnemy :public InterEnemy {
public:
	HealEnemy();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ClearAction()override;//�N���A�X�V
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	//�֐��|�C���^
	static void(HealEnemy::* stateTable[])();

private:
	void Inter();//�ҋ@
	void Attack();//�U��
	void Teleport();//�ړ�
	void WarpEnemy();//�G�̃��[�v����
	void AttackMove();//�U�����̓���
	//���@�w
	void BirthMagic();
private:
	int _charaState = STATE_INTER;

	enum HealType {
		Poison_SET,
		Poison_THROW,
		Poison_END,
	}_PoisonType = Poison_SET;

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
};

