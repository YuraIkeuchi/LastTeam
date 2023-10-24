#pragma once
#include "ObjCommon.h"
#include "CollisionPrimitive.h"
#include <string>
#include <IKETexture.h>
using namespace std;         //  ���O��Ԏw��
enum State {
	STATE_SPAWN = 0,
	STATE_ALIVE,
	STATE_VANISH,
};

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
	const bool GetAlive() { return m_Alive; }
	const int GetSkillID() { return m_SkillID; }
	void SetState(const int state) { _state= state; }
	void SetSkillID(const int SkillID) { m_SkillID = SkillID; }

protected:
	string m_Tag;
	unique_ptr<IKETexture> m_Pannel = nullptr;
	XMFLOAT3 m_PannelScale = {0.15f,0.15f,0.15f};
	XMFLOAT3 m_PannelRot = { 90.0f,0.0f,0.0f };
	float m_Radius = 0.35f;
	bool m_Alive = true;

	float m_VanishFrame = 0.f;
	float kVanishMax = 45.f;
	//�֐��|�C���^
	static void(InterAction::* stateTable[])();
	int _state = STATE_SPAWN;
	int m_SkillID = {};
private:
	void Spawn();//�ҋ@
	void Alive();
	void Vanish();

};