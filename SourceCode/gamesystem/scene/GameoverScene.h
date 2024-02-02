#pragma once
#include "IKESprite.h"
#include "BaseScene.h"
#include "EnemyManager.h"
/// �^�C�g���V�[��
class GameoverScene : public BaseScene {
public:
	~GameoverScene() override {};
public:
	/// ������
	void Initialize(DirectXCommon* dxCommon) override;
	/// �I��
	void Finalize() override;
	/// ���t���[���X�V
	void Update(DirectXCommon* dxCommon) override;
	/// �`��
	void Draw(DirectXCommon* dxCommon) override;

private:
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void FrontDraw(DirectXCommon* dxCommon);

	void GameOverMove();
private:
	static const int ATTACH_MAX = 2;
	static const int OVER_MAX = 2;
private://�����o�ϐ�
	unique_ptr<EnemyManager> enemyManager = nullptr;
	int m_Timer = {};
	//�p�l��
	struct GameOverAttach {
		unique_ptr<IKEObject3d> object = nullptr;
		unique_ptr<IKETexture> line = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		float frame = {};
	};

	//�^�C�g���̃p�[�c�ɕK�v�Ȃ���
	array<float, OVER_MAX> m_Angle = { 0.0f,170.0f };
	array<float, OVER_MAX> m_Angle2;

	unique_ptr<IKESprite> gameover[OVER_MAX];
	unique_ptr<IKESprite> select[OVER_MAX];
	unique_ptr<IKESprite> stick;

	array<XMFLOAT2,OVER_MAX> m_OverPos;
	array<XMFLOAT2,OVER_MAX> m_SelectPos;
	array<float, OVER_MAX> m_SelectRot;
	array<XMFLOAT2,OVER_MAX> m_OverSize;
	array<float, OVER_MAX> m_AddPower;
	array<GameOverAttach,ATTACH_MAX> attach;
	XMFLOAT2 m_StickPos = { 640.0f,900.0f };

	array<XMFLOAT2, OVER_MAX> m_AfterOverPos;
	array<XMFLOAT2, OVER_MAX> m_AfterSelectPos;

	enum OverParts {
		PARTS_OVER,
		PARTS_COUNTINUE
	};

	enum SelectParts {
		SELECT_YES,
		SELECT_NO
	};

	enum OverMove {
		MOVE_NONE,
		MOVE_OVER,
		MOVE_COUNTINUE,
		MOVE_PANEL,
		MOVE_YES_SELECT,
		MOVE_NO_SELECT
	}_OverType = MOVE_NONE;

	array<float,OVER_MAX> m_Frame = {};
	//�d�͉����x
	float m_Gravity = 1.0f;
};

