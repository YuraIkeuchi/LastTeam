#pragma once
#include "SkillBase.h"
#include "AttackSkill.h"
#include "NormalSkill.h"
#include "AttackSkill.h"
#include <string>
#include <vector>
#include "DeckUI.h"
class SkillManager
{
public:
	static SkillManager* GetInstance();

	void Initialize();

	void Update();

	void UIDraw();
	void ImGuiDraw();

	void ResetBirth();
	//�f�b�L�ɓ����Ă邩�`�F�b�N
	void DeckCheck(const int DeckNumber,const int DeckCount);
	//�f�b�L�̃N���A
	void DeckClear();
	void GetAreaDate(int& DisX, int& DisY);
private:
	//�X�L����CSV��ǂݎ��
	void LoadCsvSkill(std::string& FileName, const int id);
	//CSV���쐬
	bool CreateSkill(int id);
	//UI�̐���
	void BirthDeckUI(const int DeckNumber, const int DeckCount);
public:
	//ID�����
	int GetID(const int BirthNum);
	//�_���[�W�����
	float GetDamage();
	//�f�B���C�����
	int GetDelay();

	std::vector<std::vector<int>> GetArea();
	void SetDeckState(const int DeckNum);
public:
	const int GetDeckNum() { return m_DeckNum; }
	std::vector<SkillBase*>GetSkillBase() { return skill; }
private:
	std::vector<unique_ptr<DeckUI>> deckui;
	std::vector<SkillBase*> skill;
	/*SkillBase* skillA;
	SkillBase* skillB;
	AttackSkill skillC;*/
	std::string nameA = "����";
	std::string nameB = "�R��";
	std::string nameC = "�@��";
	std::string nameD = "�E��";
	std::string nameE = "���܂�";
	std::string nameF = "�X�J�X";
	int m_DeckNum = {};
	int m_BirthMax = {};
	int m_DeckRemain = {};
	std::vector<int> m_DeckDate;
};