#pragma once
#include"DirectXCommon.h"
#include "IKETexture.h"
#include <DirectXMath.h>
#include <ObjCommon.h>

using namespace std;         //  ���O��Ԏw��

//�U���G���A�N���X
class AttackArea :
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
	AttackArea(string& userName,string& stateName = (string)"None");
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
	static void (AttackArea::* stateTable[])();

private:
	void SlashMove();
	void StoneMove();
	void PoisonMove();
private:
	XMFLOAT3 SetPanelPos(const int width, const int height);
public:
	//gettersetter
	const bool GetHit() { return m_Hit; }
	const bool GetAlive() { return m_Alive; }
	const bool GetIsFixed() { return isFixed; }
	const string GetName() { return m_Name; }

	const int GetNowWidth() { return m_NowWidth; }
	const int GetNowHeight() { return m_NowHeight; }
	const int GetTimer() { return m_Timer; }
	const float GetDamage() { return m_Damage; }
	const std::string GetStateName() { return StateName; }
	const int GetPoisonToken() { return m_PoisonToken; }

	void SetHit(const bool Hit) { m_Hit = Hit; }
	void SetTimer(const int Timer) { m_Timer = Timer; }
	void SetName(const string name) { m_Name = name; }
	void SetDamage(const float Damage) { m_Damage = Damage; }
	void SetIsFixed(bool flag) { isFixed = flag; }
	void SetPoisonToken(const int PoisonToken) { m_PoisonToken = PoisonToken; }
private:

	enum EffectPattern {
		Slash,
		Stone,
		Poison,
	}_EffectState = Slash;
	//�p�l��
	struct Panel {
		unique_ptr<IKETexture> tex = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		bool predict = false;
	};
	Panel panels;
	string m_Name = "none";
	//�����֌W
	bool m_Alive = false;
	int m_BirthTimer = 0;
	int m_Timer = {};
	//�q�b�g�������ǂ���
	bool m_Hit = false;
	//���̕␳�������Ȃ�
	bool isFixed = false;
	int m_NowWidth = {};
	int m_NowHeight = {};
	int m_PoisonToken = {};
	float m_Damage = {};
	std::string StateName = "NONE";
	//�㏸�x
	float m_AddPower = 0.0f;
	//�d�͉����x
	float m_Gravity = 0.02f;

	enum ThornState {
		THORN_UP,
		THORN_END,
	}_ThornState = THORN_UP;

	float m_Frame = {};
};