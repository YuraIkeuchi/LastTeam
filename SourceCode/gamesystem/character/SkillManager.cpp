#include "SkillManager.h"
#include <random>
#include <Helper.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>

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
	//skill[0]->Create(nameA, 1);
	skill[1] = new AttackSkill();
	//skill[1]->Create(nameB, 2, 0.0f, 0.0f, 0.0f, 1, 1);
	skill[2] = new AttackSkill();
	//skill[2]->Create(nameC, 3, 0.0f, 0.0f, 0.0f, 1, 1);
	skill[3] = new AttackSkill();
	//skill[3]->Create(nameD, 4, 0.0f, 0.0f, 0.0f, 1, 1);
	skill[4] = new AttackSkill();
	//skill[4]->Create(nameE, 5, 0.0f, 0.0f, 0.0f, 1, 1);
	skill[5] = new AttackSkill();
	//skill[5]->Create(nameF, 6, 0.0f, 0.0f, 0.0f, 1, 1);

	//csv�ǂݎ��
	for (int i = 1; i < 7; i++)
	{
		CreateSkill(i);
	}


	//���ԓ���ւ��Ă�
	//std::shuffle(skill.begin(), skill.end(), std::default_random_engine());
}

void SkillManager::ImGuiDraw() {
	for (SkillBase* newskill : skill) {
		if (newskill != nullptr) {
			newskill->ImGuiDraw();
		}
	}

	ImGui::Begin("Mana");
	ImGui::Text("Num:%d", m_RandNum);
	ImGui::Text("Num:%d", m_BirthMax);
	for (int i = 0; i < m_DeckDate.size(); i++) {
		ImGui::Text("Data[%d]:%d", i, m_DeckDate[i]);
	}
	ImGui::End();
}

int SkillManager::GetID(const int BirthNum) {
	int result = 0;
	if (skill[m_DeckDate[BirthNum]]->GetDeckIn()) {
	/*	m_RandNum = BirthNum;
		m_BirthMax = BirthNum;*/
		skill[m_DeckDate[BirthNum]]->SetBirth(true);
		result = skill[m_DeckDate[BirthNum]]->GetID();
	}
	return result;
}

float SkillManager::GetDamage() {
	float result = {};
	result = skill[m_RandNum]->GetDamege();
	
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

//�f�b�L�`�F�b�N
void SkillManager::DeckCheck(const int DeckNumber) {
	
	//if (m_DeckDate.size() != 0) {	//�f�b�L�f�[�^��0�ȊO�Ȃ�f�[�^�������Ă�
	//	
	//}
	//�f�b�L�ɓ���悤�ɂ���
	skill[DeckNumber]->SetDeckIn(true);
	m_DeckDate.push_back(DeckNumber);
	unsigned int seed = (int)(std::chrono::system_clock::now())
		.time_since_epoch()
		.count();
	//�f�b�L���V���b�t������
	std::shuffle(m_DeckDate.begin(), m_DeckDate.end(), std::default_random_engine(seed));
}
//�f�b�L�̃N���A
void SkillManager::DeckClear() {
	if (!m_DeckDate.empty()) {
		m_DeckDate.clear();
	}
}