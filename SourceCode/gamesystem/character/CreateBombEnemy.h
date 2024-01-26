#pragma once
#include"InterEnemy.h"
#include "Bomb.h"

using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class CreateBombEnemy :public InterEnemy {
public:
	CreateBombEnemy();
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
	static void(CreateBombEnemy::* stateTable[])();

private:
	void Inter();//�ҋ@
	void Attack();//�U��
	void Teleport();//�ړ�
	void BirthBomb();//
	void WarpEnemy();//�G�̃��[�v����

	//���@�w
	void BirthMagic();
private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;

	enum BombType {
		Bomb_SET,
		Bomb_THROW,
		Bomb_END,
	}_BombType = Bomb_SET;

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
	bool isBombThrow;

	std::vector<unique_ptr<Bomb>> bomb;
	//�C���^�[�o���Ƃ�
	vector<int>m_Limit;

};

