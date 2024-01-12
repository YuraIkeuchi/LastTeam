#pragma once
#include"DirectXCommon.h"
#include "ObjCommon.h"
#include <DirectXMath.h>
#include "Player.h"

using namespace std;         //  ���O��Ԏw��

//�g�Q�N���X
class EnemyTornade :
	public ObjCommon {
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	EnemyTornade();
public:
	//������
	bool Initialize() override;
	//�X�e�[�^�X������
	void InitState(const int width, const int height);
	//�X�V
	void Update() override;
	//�`��
	void Draw(DirectXCommon* dxCommon) override;
	//ImGui
	void ImGuiDraw();
private:
	XMFLOAT3 SetPanelPos(const int width, const int height);
	void Move();
	void Collide();
public:
	//gettersetter
	const bool GetAlive() { return m_Alive; }
	const float GetDamage() { return m_Damage; }

	void SetSound(const bool Sound) { m_Sound = Sound; }
	void SetHit(const bool Hit) { m_Hit = Hit; }
	void SetPlayer(Player* player) { this->player = player; }

private:
	Player* player;
	//�����֌W
	bool m_Alive = false;
	int m_AliveTimer = {};

	//�q�b�g�������ǂ���
	bool m_Hit = false;

	int m_NowWidth = {};
	int m_NowHeight = {};
	float m_Damage = 20.0f;

	enum ThornState {
		THORN_UP,
		THORN_STOP,
		THORN_END,
	}_ThornState = THORN_UP;

	float m_Frame = {};
	bool m_Sound = false;
};