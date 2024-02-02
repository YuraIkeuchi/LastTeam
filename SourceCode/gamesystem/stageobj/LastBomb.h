#pragma once
#include "ObjCommon.h"
#include "Player.h"
#include "Explosion.h"
//���e�̍U��(�G)
class LastBomb :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	LastBomb();

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

	void InitState(const XMFLOAT3& pos, const int width, const int height);

	void BirthExplosion();
	void DeleteBomb();

private://�X�e�[�g
	static void (LastBomb::* stateTable[])();
private:
	void Throw();//������

public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }

	void SetAlive(const bool Alive) { m_Alive = Alive; }

	void SetTargetTimer(const int TargetTimer) { m_TargetTimer = TargetTimer; }
	void SetBirthCount(const int BirthCount) { m_BirthCount = BirthCount; }
	void SetPlayer(Player* player) { this->player = player; }

private:
	Player* player;
	//���݂̃}�X�ԍ�
	int m_NowWidth = {};
	int m_NowHeight = {};
	bool m_Alive = true;//�����t���O

	int m_TargetTimer = {};//�o�����Ԃ̖ڕW

	int m_ThrowTimer = {};

	enum BombState {
		BOMB_SET,
		BOMB_THROW,
	}_BombState = BOMB_SET;

	XMFLOAT3 m_AfterPos = {};
	float m_Frame = {};

	float m_BaseScale = {};
	
	float m_Damage = {};
	bool m_Hit = false;

	float m_AddPower = {};
	float m_Gravity = 0.02f;
	bool m_Jump = false;

	int m_BirthCount = {};

	bool m_Attack[4][4] = {false};

	// �U���G���A
	std::vector<unique_ptr<Explosion>> explosion;
};
