#pragma once
#include "IKESprite.h"
#include <string>
#include <vector>
#include <memory>
#include <map>
#include "ActionUI.h"
#include "AttackArea.h"
#include <Passive.h>
using namespace DirectX;
using namespace std;
//�s���̎��
enum ActType {
	ACT_ATTACK,
	ACT_GUARD,
	ACT_SKILL
};
//�Q�[���̏�Ԃ��Ǘ�����N���X
class GameStateManager {
public:
	static GameStateManager* GetInstance();
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();
	//�s��UI�̕`��(����܂�ǂ��Ȃ�����U������)
	void ActUIDraw();
	//�v���C���[�̌��݈ʒu
	void PlayerNowPanel(const int NowWidth, const int NowHeight);
	//�X�L������肷��
	void AddSkill(const int ID,const float damage,const int Delay);
private:
	//�U�������u��
	void AttackTrigger();
	//�U���G���A�̐���
	void BirthArea();
	//�s��UI�̐���
	void BirthActUI(const int ID);
	//�X�L���̎g�p
	void UseSkill();
	//�s���̏I��
	void FinishAct();
	//
	void GaugeUpdate();

	void PassiveCheck();

	//�f�b�L�̏�����
	void DeckInitialize();
public:
	//gettersetter
	const bool GetCounter() { return m_Counter; }
	const float GetPosScore() { return m_PosScore; }
	const float GetGrazeScore() { return m_GrazeScore; }
	vector<AttackArea*>GetAttackArea() { return attackarea; }
	const float GetDiameterVel() { return m_DiameterVel; }


	void SetCounter(const bool isCounter) { this->m_Counter = isCounter; }
	void SetPosScore(const float PosScore) { this->m_PosScore = PosScore; }
	void SetGrazeScore(const float GrazeScore) { this->m_GrazeScore = GrazeScore; }
	void SetDiameterVel(const float DiameterVel) { this->m_DiameterVel = DiameterVel; }

private:
	static const int ACT_PATTERN = 3;
private:

	struct ActState {
		int ActID;//ID
		float ActDamage;//�_���[�W
		int ActDelay;//�f�B���C
	};

	vector<ActState> m_Act;
	//�e�s����
	int m_ActCount[ACT_PATTERN] = {};

	//�S�s����
	int m_AllActCount = {};

	//�s����UI
	vector<unique_ptr<ActionUI>> actui;

	std::list<std::unique_ptr<Passive>> GotPassives;

	unique_ptr<IKESprite> skillUI = nullptr;
	unique_ptr<IKESprite> gaugeUI = nullptr;

	XMFLOAT2 basesize = { 45.f,400.f };

	//�U���G���A
	vector<AttackArea*> attackarea;

	//�J�E���^�[
	bool m_Counter = false;
	int m_CounterTimer = {};
	int m_CounterScore = {};

	//�ʒu�̃X�R�A
	float m_PosScore = {};
	//�O���C�Y�̃X�R�A(��Xint�ɂ���)
	float m_GrazeScore = 0.0f;

	//�S�̃X�R�A
	int m_AllScore = {};

	//�v���C���[�̌��݃p�l��
	int m_NowHeight = {};
	int m_NowWidth = {};
	float m_DiameterVel = 1.0f;


	enum SkillType {
		SKILL_NORMAL,
		SKILL_STRONG,
		SKILL_SPECIAL
	}_SkillType = SKILL_NORMAL;
	//�Q�[�W
	float m_GaugeCount = 0;
	//
	float m_DiameterGauge = 1.0f;
	//�Q�[�W�}�b�N�X
	float kGaugeCountMax = 180;
	bool m_IsReload = true;
	bool m_BirthSkill = false;

	int m_ID = {};
	int m_Delay = {};
	string m_Name;

	vector<int> m_DeckNumber = { 0,2,3,5, };
};