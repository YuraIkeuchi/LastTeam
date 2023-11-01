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
	//一旦3に指定(実際はCSVとかになるかな)
	skill.resize(7);
	//ここはもう少しやりようがあるかもしれない
	skill[0] = new AttackSkill();
	skill[1] = new AttackSkill();
	skill[2] = new AttackSkill();
	skill[3] = new AttackSkill();
	skill[4] = new AttackSkill();
	skill[5] = new AttackSkill();
	skill[6] = new AttackSkill();
	//skill[5]->Create(nameF, 6, 0.0f, 0.0f, 0.0f, 1, 1);


	//csv読み取り
	for (int i = 1; i < 8; i++)
	{
		CreateSkill(i);
	}

	std::vector<std::vector<int>> area =
	{
		{1,1,1},
		{1,0,1},
		{1,1,1}
	};
	int distanceX = 1;
	int distanceY = -1;
	AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[0]);
	if (atkSkill != nullptr)
	{
		atkSkill->SetArea(area);
		atkSkill->SetDistance(distanceX, distanceY);
	}
}

//更新(ほんますまん)
void SkillManager::Update() {	
	for (auto i = 0; i < deckui.size(); i++) {
		if (deckui[i] == nullptr)continue;
		deckui[i]->SetActCount(i);
		deckui[i]->Update();


		if (!deckui[i]->GetAlive()) {
			deckui.erase(cbegin(deckui) + i);
		}
	}
}
//UIの描画(ほんますまんpart2)
void SkillManager::UIDraw() {
	for (auto i = 0; i < deckui.size(); i++) {
		if (deckui[i] == nullptr)continue;
		deckui[i]->Draw();
	}
}

void SkillManager::ImGuiDraw() {
	//for (SkillBase* newskill : skill) {
	//	if (newskill != nullptr) {
	//		newskill->ImGuiDraw();
	//	}
	//}

	ImGui::Begin("Mana");
	ImGui::Text("Num:%d", m_DeckNum);
	ImGui::Text("m_DeckRemain:%d", m_DeckRemain);
	for (int i = 0; i < m_DeckDate.size(); i++) {
		ImGui::Text("Data[%d]:%d", i, m_DeckDate[i]);
	}
	ImGui::End();
}

int SkillManager::GetID(const int BirthNum) {
	int result = 0;
	if (skill[m_DeckDate[BirthNum + m_DeckRemain]]->GetDeckIn()) {
		m_BirthMax = m_DeckDate[BirthNum + m_DeckRemain];
		skill[m_DeckDate[BirthNum + m_DeckRemain]]->SetBirth(true);
		deckui[BirthNum]->SetUse(true);
		result = skill[m_DeckDate[BirthNum + m_DeckRemain]]->GetID();
	}
	return result;
}
//ダメージ
float SkillManager::GetDamage() {
	float result = {};

	
	if (skill[m_BirthMax]->GetSkillType() != SkillType::damege)
	{
		assert(0);
	}

	AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[m_BirthMax]);
	result = atkSkill->GetDamege();

	return result;
}
//ディレイ
int SkillManager::GetDelay() {
	int result = {};
	result = skill[m_BirthMax]->Getlatency();

	return result;
}
//リセット
void SkillManager::ResetBirth() {
	for (SkillBase* newskill : skill) {
		if (newskill != nullptr) {
			newskill->SetBirth(false);
		}
	}
}

vector<std::vector<int>> SkillManager::GetArea() {
	AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[0]);
	vector<std::vector<int>> result;
	result = atkSkill->GetArea();

	return result;
}

//スキルのCSVを読み取る
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

//デッキチェック
void SkillManager::DeckCheck(const int DeckNumber, const int DeckCount) {
	
	//デッキに入るようにする
	skill[DeckNumber]->SetDeckIn(true);
	m_DeckDate.push_back(DeckNumber);
	unsigned int seed = (int)(std::chrono::system_clock::now())
		.time_since_epoch()
		.count();
	//デッキをシャッフルする
	std::shuffle(m_DeckDate.begin(), m_DeckDate.end(), std::default_random_engine(seed));
	//デッキのUIを作成
	BirthDeckUI(DeckNumber, DeckCount);
}
//デッキのクリア
void SkillManager::DeckClear() {
	if (!m_DeckDate.empty()) {
		m_DeckDate.clear();
		deckui.clear();
	}
}
//デッキの状態を取る(残ってるデッキとか)
void SkillManager::SetDeckState(const int DeckNum) {
	m_DeckNum = DeckNum;
	m_DeckRemain = (int)(m_DeckDate.size()) - m_DeckNum;
}
//UIの生成
void SkillManager::BirthDeckUI(const int DeckNumber, const int DeckCount) {
	//デッキUIのセット
	DeckUI* newdeckUi = nullptr;
	newdeckUi = new DeckUI();
	newdeckUi->Initialize();
	newdeckUi->InitState(DeckCount);
	deckui.emplace_back(newdeckUi);
	//手に入れたスキルのUIの更新
	for (auto i = 0; i < m_DeckDate.size(); i++) {
		deckui[i]->SetID(m_DeckDate[i]);
	}
}
//攻撃エリアの取得
void SkillManager::GetAreaDate(int& DisX, int& DisY) {
	AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[0]);
	
	int l_DistanceX = {};
	int l_DistanceY = {};
	std::vector<std::vector<int>> l_Area;
	l_DistanceX = atkSkill->GetDistanceX();
	l_DistanceY = atkSkill->GetDistanceX();
	DisX = l_DistanceX;
	DisY = l_DistanceY;
}
