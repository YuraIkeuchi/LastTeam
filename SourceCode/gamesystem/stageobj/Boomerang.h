#pragma once
#include "ObjCommon.h"
#include "IKETexture.h"
#include "Player.h"

//�u�[�������̍U��(�G)
class Boomerang :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Boomerang();

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
	static void (Boomerang::* stateTable[])();
private:
	void Throw();//������

public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }

	void SetAlive(const bool Alive) { m_Alive = Alive; }

	void SetTargetTimer(const int TargetTimer) { m_TargetTimer = TargetTimer; }
	void SetPlayer(Player* player) { this->player = player; }

private:
	unique_ptr<IKETexture> shadow_tex;
	Player* player;
	//�p�l��
	struct Panel {
		unique_ptr<IKETexture> tex = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		bool predict = false;
	};
	Panel panels;
	//���݂̃}�X�ԍ�
	int m_NowWidth = {};
	int m_NowHeight = {};
	bool m_Alive = true;//�����t���O

	int m_TargetTimer = {};//�o�����Ԃ̖ڕW

	int m_ThrowTimer = {};

	enum ThrowType {
		THROW_SET,
		THROW_PLAY,
	};

	int m_ThrowType = THROW_SET;
	XMFLOAT3 m_AfterPos = {};
	float m_Frame = {};

	float m_BaseScale = {};
	XMFLOAT2 m_Angle = {};

	float m_AddSpeed = {};//�����x

	int m_AliveTimer = {};

	float m_Damage = {};
	
	XMFLOAT3 m_TargetPos = {};

	enum MoveDir {
		MOVE_STRAIGHT,
		MOVE_UP,
		MOVE_DOWN,
		MOVE_RETURN,
	}_MoveDir = MOVE_UP;

	float m_Length = {};
	float m_Speed = {};
};
