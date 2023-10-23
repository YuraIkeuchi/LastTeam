#pragma once
#include "IKESprite.h"
#include <string>
#include <vector>
#include <memory>
#include "ActionUI.h"
#include "AttackArea.h"
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
	void AddSkill(const string& Tag);
private:
	
	//�U���G���A�̐���
	void BirthArea();
	//�s��UI�̐���
	void BirthActUI(const string& Tag);
	//�X�L���̎g�p
	void UseSkill();
	//�s���̏I��
	void FinishAct();

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
	//�e�s����
	int m_ActCount[ACT_PATTERN] = {};

	//�S�s����
	int m_AllActCount = {};

	//�s����UI
	vector<unique_ptr<ActionUI>> actui;

	//�U���G���A
	vector<AttackArea*> attackarea;
	//�s����
	vector<int> m_Act;

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

};