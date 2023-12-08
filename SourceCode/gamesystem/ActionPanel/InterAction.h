#pragma once
#include "ObjCommon.h"
#include "CollisionPrimitive.h"
#include <string>
#include <IKETexture.h>
#include "Player.h"
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
	void GetSkillData();
protected:

	virtual void Action() = 0;//�{�X���L�̏���

	virtual void ImGui_Origin() = 0;//�{�X���ꂼ���ImGui
public:
	//gettersetter
	const bool GetAlive() { return m_Alive; }
	const bool GetDelete() { return m_Delete; }
	const bool GetDiscard() { return m_Discard; }
	const int GetSkillID() { return m_SkillID; }
	const float GetDamage() { return m_Damage; }
	const int GetDelay() { return m_Delay; }
	const string GetStateName() { return StateName; }
	const std::vector<std::vector<int>> GetArea() { return m_Area; }

	void SetState(const int state) { _state= state; }
	void SetAlive(const bool Alive) { m_Alive = Alive; }
	void SetPlayer(Player* player) { this->player = player; }

protected:
	Player* player = nullptr;
	string m_Tag;
	unique_ptr<IKETexture> m_Pannel = nullptr;

	XMFLOAT3 m_PannelScale = {0.15f,0.15f,0.15f};
	XMFLOAT3 m_PannelRot = { 0.0f,0.0f,0.0f };
	float m_Radius = 0.35f;
	bool m_Alive = true;
	bool m_Delete = false;
	bool m_Discard = false;
	float m_VanishFrame = 0.f;
	float kVanishMax = 45.f;
	//�֐��|�C���^
	static void(InterAction::* stateTable[])();
	int _state = STATE_SPAWN;
	int m_SkillID = {};
	float m_Damage = {};
	int m_Delay = {};
	int m_SkillType = {};
	std::vector<std::vector<int>> m_Area;
	int m_DistanceX = {};
	int m_DistanceY = {};
	string StateName = {};

	struct Shadow {
		unique_ptr<IKETexture> tex;
		//�e�̕ϐ�
		XMFLOAT3 Position = {};
		XMFLOAT3 Scale = {};
	};

	Shadow shadow;
private:
	void Spawn();//�ҋ@
	void Alive();
	void Vanish();

};