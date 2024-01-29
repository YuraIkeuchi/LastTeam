#pragma once
#include"DirectXCommon.h"
#include "IKETexture.h"
#include <DirectXMath.h>
#include <ObjCommon.h>
#include "Player.h"
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
	void SpearMove();
	void HatenaMove();
	void HealMove();
private:
	XMFLOAT3 SetPanelPos(const int width, const int height);
public:
	//gettersetter
	const bool GetHit() { return m_Hit; }
	const bool GetBuff() { return m_Buff; }
	const bool GetAlive() { return m_Alive; }
	const bool GetIsFixed() { return isFixed; }
	const bool GetAttack() { return m_Attack; }
	const string GetName() { return m_Name; }

	const int GetNowWidth() { return m_NowWidth; }
	const int GetNowHeight() { return m_NowHeight; }
	const int GetTimer() { return m_Timer; }
	const float GetDamage() { return m_Damage; }
	const std::string GetStateName() { return StateName; }
	const int GetPoisonToken() { return m_PoisonToken; }

	void SetHit(const bool Hit) { m_Hit = Hit; }
	void SetSound(const bool Sound) { m_Sound = Sound; }
	void SetBuff(const bool Buff) { m_Buff=Buff; }
	void SetTimer(const int Timer) { m_Timer = Timer; }
	void SetName(const string name) { m_Name = name; }
	void SetDamage(const float Damage) { m_Damage = Damage; }
	void SetIsFixed(bool flag) { isFixed = flag; }
	void SetPoisonToken(const int PoisonToken) { m_PoisonToken = PoisonToken; }

	void SetPlayer(Player* player) { this->player = player; }
private:
	Player* player;
	enum EffectPattern {
		Slash,
		Stone,
		Poison,
		Spear,
		Hatena,
		Heal,
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
	bool m_Attack = false;//�U������̏u��
	int m_BirthTimer = 0;
	int m_Timer = {};
	bool m_Buff = false;
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

	enum EffectType {
		EFFECT_FALL,
		EFFECT_BOUND,
		EFFECT_HIT,
	}_EffectType = EFFECT_FALL;

	bool m_Sound = false;
};