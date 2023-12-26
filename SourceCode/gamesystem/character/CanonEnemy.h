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
	void WarpEnemy();//�G�̃��[�v����
	//���@�w
	void BirthMagic();
private:
static const int BULLET_NUM = 3;
private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
	vector<unique_ptr<EnemyBullet>> bullets;//�|���^�[�K�C�X�g

	enum CanonType {
		CANON_SET,
		CANON_THROW,
		CANON_END,
	}_CanonType = CANON_SET;

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
	int m_BulletNum = {};
	float m_RotFrame = {};
	int m_ShotDir = {};
	float m_AfterRotY = {};
	bool m_ChangeRot = {};
	float m_BaseScale = {};
	float m_ScaleFrame = {};
};

