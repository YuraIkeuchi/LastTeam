#pragma once
#include "IKESprite.h"
#include <string>
#include <vector>
#include <memory>
#include "ActionUI.h"
#include "AttackArea.h"
#include "SkillBase.h"
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
	void AddSkill(const int ID);
private:
	
	//�U���G���A�̐���
	void BirthArea();
	//�s��UI�̐���
	void BirthActUI();
	//�X�L���̎g�p
	void UseSkill();
	//�s���̏I��
	void FinishAct();
	//
	void GaugeUpdate();
	void AddSkillID(vector<SkillBase*>bullet);
public:
	//gettersetter
	const bool GetCounter() { return m_Counter; }
	const float GetPosScore() { return m_PosScore; }
	const float GetGrazeScore() { return m_GrazeScore; }
	vector<AttackArea*>GetAttackArea() { return attackarea; }

	void SetCounter(const bool isCounter) { this->m_Counter = isCounter; }
	void SetPosScore(const float PosScore) { this->m_PosScore = PosScore; }
	void SetGrazeScore(const float GrazeScore) { this->m_GrazeScore = GrazeScore; }
	
private:
	static const int ACT_PATTERN = 3;
private:

	struct ActState {
		int ActID;
	};

	vector<ActState> m_Act;
	//�e�s����
	int m_ActCount[ACT_PATTERN] = {};

	//�S�s����
	int m_AllActCount = {};

	//�s����UI
	vector<unique_ptr<ActionUI>> actui;

	unique_ptr<IKESprite> skillUI = nullptr;
	unique_ptr<IKESprite> gaugeUI = nullptr;

	XMFLOAT2 basesize = { 45.f,400.f };

	//�U���G���A
	vector<AttackArea*> attackarea;
	//��ɓ��ꂽID
	vector<int> ID;

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

	enum SkillType {
		SKILL_NORMAL,
		SKILL_STRONG,
		SKILL_SPECIAL
	}_SkillType = SKILL_NORMAL;
	//�Q�[�W
	float m_GaugeCount = 0;
	//�Q�[�W�}�b�N�X
	float kGaugeCountMax = 180;

};