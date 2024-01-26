#pragma once
#include"InterEnemy.h"
#include "RegeneArea.h"
using namespace std;         //  ���O��Ԏw��
//���X�{�X�̂����̓G
class SupportEnemy2 :public InterEnemy {
public:
	SupportEnemy2();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ClearAction()override;//�N���A�X�V
	void GameOverAction()override;//�Q�[���I�[�o�[�X�V
	void ImGui_Origin()override;
	void DeathSpecial()override;
	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	//�֐��|�C���^
	static void(SupportEnemy2::* stateTable[])();
	static void(SupportEnemy2::* attackTable[])();
private:
	void Inter();//�ҋ@
	void Attack();//�U��
	void Teleport();//�ړ�
	void BirthArea(const int Width, const int Height, const string& name);//�U���G���A
	void BirthPredict(const int Width, const int Height, const string& name);//�\���G���A

	void WarpEnemy();//�G�̃��[�v����
	void AttackMove();//�U�����̓���
	void BirthRegene();//���W���l�}�X�쐬

	//���@�w
	void BirthMagic();
private:
	static const int BULLET_NUM = 5;
private:
	// �񕜃G���A
	vector<unique_ptr<RegeneArea>> regenearea;
	int m_AttackCount = {};
	int _charaState = STATE_INTER;

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
	//�U���̃C���^�[�o���Ƃ�
	vector<int>m_AttackLimit;
	int m_BulletNum = {};


	int m_ShotDir = {};
	float m_AfterRotY = {};
	bool m_ChangeRot = {};

	int m_SupportType = {};

	enum AttackState {
		ATTACK_REGENE,
		ATTACK_SHIELD,
	};
	int m_AttackState = ATTACK_REGENE;
};

