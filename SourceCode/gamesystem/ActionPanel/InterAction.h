#pragma once
#include "ObjCommon.h"
#include "CollisionPrimitive.h"
#include <string>
#include <IKETexture.h>
using namespace std;         //  ���O��Ԏw��

class InterAction :
	public ObjCommon {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:

public:
	//������
	virtual bool Initialize() = 0;
	//�X�V
	void Update();
	/// �|�[�Y�̂Ƃ�
	virtual void Pause() = 0;
	//�`��
	virtual void Draw(DirectXCommon* dxCommon)override;

	void ImGuiDraw();//ImGui�̕`��

	virtual void EffecttexDraw(DirectXCommon* dxCommon) = 0;

	void Collide();
protected:

	virtual void Action() = 0;//�{�X���L�̏���

	virtual void ImGui_Origin() = 0;//�{�X���ꂼ���ImGui
public:
	//gettersetter
	bool GetAlive() { return m_Alive; }
protected:
	string m_Tag;
	unique_ptr<IKETexture> m_Pannel = nullptr;
	XMFLOAT3 m_PannelScale = {0.15f,0.15f,0.15f};
	XMFLOAT3 m_PannelRot = { 90.0f,0.0f,0.0f };
	float m_Radius = 0.35f;
	bool m_Alive = true;
};