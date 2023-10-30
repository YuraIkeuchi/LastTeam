#include "SkillManager.h"
#include <random>
#include <Helper.h>
#include <fstream>
#include <iostream>
#include <sstream>

SkillManager* SkillManager::GetInstance()
{
	static SkillManager instance;

	return &instance;
}

void SkillManager::Initialize()
{
	//��U3�Ɏw��(���ۂ�CSV�Ƃ��ɂȂ邩��)
	skill.resize(6);
	//�����͂����������悤�����邩������Ȃ�
	skill[0] = new AttackSkill();
	skill[1] = new AttackSkill();
	skill[2] = new AttackSkill();
	skill[3] = new AttackSkill();
	skill[4] = new AttackSkill();
	skill[5] = new AttackSkill();

	//csv�ǂݎ��
	for (int i = 1; i < 7; i++)
	{
		CreateSkill(i);
	}

	std::vector<std::vector<int>> area =
	{
		{1,0,0},
		{0,0,1},
		{1,0,0}
	};
	int distanceX = 1;
	int distanceY = -1;
	AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[0]);
	if (atkSkill != nullptr)
	{
		atkSkill->SetArea(area);
		atkSkill->SetDistance(distanceX, distanceY);
	}

	//���ԓ���ւ��Ă�
	std::shuffle(skill.begin(), skill.end(), std::default_random_engine());
}

void SkillManager::ImGuiDraw() {
	for (SkillBase* newskill : skill) {
		if (newskill != nullptr) {
			newskill->ImGuiDraw();
		}
	}
}

int SkillManager::GetID() {
	int result = 0;
	int randskill = 0;

	//�����_���Ŏ擾(�����͌�Œ���)
	randskill = Helper::GetInstance()->GetRanNum(0, (int)(skill.size() - 1));
	if (!skill[randskill]->GetBirth()) {
		skill[randskill]->SetBirth(true);
		result = skill[randskill]->GetID();
	}
	else {
		randskill = Helper::GetInstance()->GetRanNum(0, (int)(skill.size() - 1));
		skill[randskill]->SetBirth(true);
		result = skill[randskill]->GetID();
	}
	m_RandNum = randskill;
	return result;
}

float SkillManager::GetDamage() {
	float result = {};
	
	if (skill[m_RandNum]->GetSkillType() != SkillType::damege)
	{
		assert(0);
	}

	AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[m_RandNum]);
	result = atkSkill->GetDamege();
	
	return result;
}

int SkillManager::GetDelay() {
	int result = {};
	result = skill[m_RandNum]->Getlatency();

	return result;
}

void SkillManager::ResetBirth() {
	for (SkillBase* newskill : skill) {
		if (newskill != nullptr) {
			newskill->SetBirth(false);
		}
	}
}

//�X�L����CSV��ǂݎ��
void SkillManager::LoadCsvSkill(std::string& FileName, const int id) {
	
	std::ifstream file;
	std::stringstream popcom;

	file.open(FileName);
	popcom << file.rdbuf();
	file.close();

	std::string line;
	while (std::getline(popcom, line)) {
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}
		else if (word.find("ID") == 0) {
			std::getline(line_stream, word, ',');
			skill[id - 1]->SetID(std::stoi(word));
		}
		else if (word.find("SkillType") == 0) {
			std::getline(line_stream, word, ',');
			if (std::stoi(word) == 0)
			{
				skill[id - 1]->SetSkillType(SkillType::damege);
			}
			else if (std::stoi(word) == 1)
			{
				skill[id - 1]->SetSkillType(SkillType::buff);
			}
			else if (std::stoi(word) == 2)
			{
				skill[id - 1]->SetSkillType(SkillType::debuff);
			}
			else if (std::stoi(word) == 3)
			{
				skill[id - 1]->SetSkillType(SkillType::max);
			}
		}
		else if (word.find("Delay") == 0) {
			std::getline(line_stream, word, ',');
			skill[id - 1]->SetLatency(std::stoi(word));
		}
		else if (word.find("Name") == 0) {
			std::getline(line_stream, word, ',');
			skill[id - 1]->SetName(word);
		}
		else if (word.find("InvocatingTime") == 0) {
			std::getline(line_stream, word, ',');
			skill[id - 1]->SetInvocatingTime(std::stoi(word));
		}
		else if (word.find("restraintTime") == 0) {
			std::getline(line_stream, word, ',');
			skill[id - 1]->SetRestraintTime(std::stoi(word));
		}
		else if (word.find("Rarity") == 0) {
			std::getline(line_stream, word, ',');
			skill[id - 1]->SetRarity(std::stoi(word));
		}
		else if (word.find("PopRate") == 0) {
			std::getline(line_stream, word, ',');
			skill[id - 1]->SetPopRate(std::stoi(word));
		}
		else if (word.find("Damege") == 0) {
			std::getline(line_stream, word, ',');
			AttackSkill *atkSkill = dynamic_cast<AttackSkill*>(skill[id - 1]);
			if (atkSkill != nullptr)
			{
				atkSkill->SetDamege(std::stof(word));
			}
			break;
		}
	}
}

bool SkillManager::CreateSkill(int id) {

	std::string directory = "Resources/csv/skill/Skill";

	std::stringstream ss;
	if (id > 10) {
		ss << directory << id << ".csv";
	}
	else {
		ss << directory << "0" << id << ".csv";
	}
	std::string csv_ = ss.str();
	LoadCsvSkill(csv_, id);

	//Player::GetInstance()->SetDelayTimer(m_Delay);
	return true;
}