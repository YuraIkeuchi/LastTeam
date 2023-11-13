#pragma once
#include "ObjCommon.h"

enum PolterType {
	TYPE_FOLLOW,
	TYPE_BOUND,
};
//�{�X�����ލU�������N���X
class EnemyBullet :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	EnemyBullet();

	bool Initialize() override;//������
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

	/// <summary>
	/// ImGui�`��
	/// </summary>
	void ImGuiDraw();

	bool Collide();	//�����蔻��

private://�X�e�[�g
	static void (EnemyBullet::* stateTable[])();
private:
	void Throw();//������

public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }

	void SetAlive(const bool Alive) { m_Alive = Alive; }
	void SetPolterType(const int PolterType) { m_PolterType = PolterType; }

	void SetTargetTimer(const int TargetTimer) { m_TargetTimer = TargetTimer; }

private:
	unique_ptr<IKEObject3d> m_Pannel = nullptr;
	//���݂̃}�X�ԍ�
	int m_NowWidth = {};
	int m_NowHeight = {};
	bool m_Alive = true;//�����t���O
	int m_PolterType = TYPE_FOLLOW;

	int m_TargetTimer = {};//�o�����Ԃ̖ڕW

	int m_ThrowTimer = {};

	enum ThrowType {
		THROW_SET,
		THROW_INTER,
		THROW_PLAY,
	};

	int m_ThrowType = THROW_SET;
	XMFLOAT3 m_AfterPos = {};
	float m_Frame = {};

	float m_BaseScale = {};
	XMFLOAT2 m_Angle = {};

	float m_AddSpeed = {};//�����x

	int m_AliveTimer = {};

	//���������
	enum ThrowDir {
		DIR_STRAIGHT,//�܂�����
		DIR_SLASHUP,//�΂ߏ�
		DIR_SLASHDOWN,//�΂߂���
	};
	int m_ThrowDir = {};

	XMFLOAT3 m_PanelPos = {};
};
