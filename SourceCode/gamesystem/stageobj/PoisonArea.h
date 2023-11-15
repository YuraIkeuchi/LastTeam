#pragma once
#include"DirectXCommon.h"
#include "ObjCommon.h"
#include <DirectXMath.h>

using namespace std;         //  ���O��Ԏw��

//�U���G���A�N���X
class PoisonArea :
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
	PoisonArea();
public:
	//������
	bool Initialize()override;
	//�X�e�[�^�X������
	void InitState(const int width, const int height);
	//�X�V
	void Update()override;
	//�`��
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui
	void ImGuiDraw();
private:
	XMFLOAT3 SetPanelPos(const int width, const int height);

public:
	//gettersetter
	const bool GetHit() { return m_Hit; }
	const bool GetAlive() { return m_Alive; }
	const string GetName() { return m_Name; }

	const int GetNowWidth() { return m_NowWidth; }
	const int GetNowHeight() { return m_NowHeight; }
	const float GetDamage() { return m_Damage; }
	const std::string GetStateName() { return StateName; }

	void SetHit(const bool Hit) { m_Hit = Hit; }
	void SetName(const string name) { m_Name = name; }
	void SetDamage(const float Damage) { m_Damage = Damage; }
	void SetStateName(const std::string name) { StateName = name; }
private:
	string m_Name = "none";
	//�����֌W
	bool m_Alive = false;
	int m_AliveTimer = {};

	//�q�b�g�������ǂ���
	bool m_Hit = false;

	int m_NowWidth = {};
	int m_NowHeight = {};
	float m_Damage = {};
	std::string StateName = "NONE";
};