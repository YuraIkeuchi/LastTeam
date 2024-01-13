#pragma once
#include"InterEnemy.h"
#include "PredictArea.h"

using namespace std;         //  ���O��Ԏw��
//���ʂ̓G
class TackleEnemy :public InterEnemy {
public:
	TackleEnemy();
	bool Initialize() override;//������
	void Finalize() override;//�J��
	void Action()override;//�X�V
	void ClearAction()override;//�N���A�X�V
	void GameOverAction()override;//�Q�[���I�[�o�[�X�V
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//�`��
private:
	//�֐��|�C���^
	static void(TackleEnemy::* stateTable[])();

private:
	void Inter();//�ҋ@
	void Attack();
	void Teleport();//�ړ�
	void StandBy();//�ړ�

	void WarpEnemy();//�G�̃��[�v����
	//���@�w
	void BirthMagic();

	bool TackleCollide();

private:

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

	std::unique_ptr<PredictArea> predictArea;
	float predictFrame = 0.f;
	int nextPredict = 0;
	int m_OldWidth = m_NowWidth;
	int m_OldHeight = m_NowHeight;
	//�C���^�[�o���Ƃ�
	vector<int>m_Limit;
	float m_Speed = {};
	float m_Damage = {};
	bool m_Hit = false;
	float m_Frame = {};
};

