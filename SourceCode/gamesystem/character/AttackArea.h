#pragma once
#include"DirectXCommon.h"
#include "ObjCommon.h"
#include <DirectXMath.h>

using namespace std;         //  ���O��Ԏw��

//�U���G���A�N���X
class AttackArea :
public ObjCommon{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	AttackArea();
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
	void SetHit(const bool Hit) { m_Hit = Hit; }
private:
	//�����֌W
	bool m_Alive = false;
	int m_AliveTimer = {};

	//�q�b�g�������ǂ���
	bool m_Hit = false;
};