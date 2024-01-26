#include "SkillManager.h"
#include <StagePanel.h>
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

void SkillManager::Initialize(DirectXCommon* dxCommon)
{	
	dxcommon = dxCommon;
	//ここをいじればGameStateManagerも変わります
	//総スキル+1
	m_SKILLMAX = 30;
	//一旦3に指定(実際はCSVとかになるかな)
	skill.resize(m_SKILLMAX);
	m_Delays.resize(m_SKILLMAX);
	skilltex.resize(m_SKILLMAX);
	//ここはもう少しやりようがあるかもしれない
	for (int i = 0; i < m_SKILLMAX; i++) {
		skill[i] = new AttackSkill();
		skilltex[i].text_ = make_unique<TextManager>();
		skilltex[i].text_->Initialize(dxcommon, LOAD_ATTACK);
		skilltex[i].text_->SetConversation(TextManager::RESULT, { -200.0f,80.0f });
	}
	//スペシャルスキルはこちらに上書きしてください
	skill[9] = new SpecialSkill();
	skill[11] = new SpecialSkill();
	skill[14] = new SpecialSkill();
	skill[21] = new SpecialSkill();
	//csv読み取り
	for (int i = 0; i < m_SKILLMAX; i++)
	{
		CreateSkill(i);
	}
}

//更新(ほんますまん)
void SkillManager::Update() {	
	for (auto i = 0; i < deckui.size(); i++) {
		if (deckui[i] == nullptr)continue;
		deckui[i]->SetActCount(i);
		deckui[i]->Update();
	}
	auto result = std::remove_if(deckui.begin(), deckui.end(),
		[](unique_ptr<DeckUI>& deck) { return !deck->GetAlive(); });
	deckui.erase(result, deckui.end());

}
//UIの描画(ほんますまんpart2)
void SkillManager::UIDraw() {
	IKESprite::PreDraw();
	for (auto i = 0; i < deckui.size(); i++) {
		if (deckui[i] == nullptr)continue;
		deckui[i]->Draw();
	}
	IKESprite::PostDraw();
}

void SkillManager::ImGuiDraw() {
	/*for (SkillBase* newskill : skill) {
		if (newskill != nullptr) {
			newskill->ImGuiDraw();
		}
	}*/

	ImGui::Begin("Mana");
	ImGui::Text("m_DeckNum:%d", m_DeckNum);
	ImGui::End();
	//for (auto i = 0; i < deckui.size(); i++) {
	//	if (deckui[i] == nullptr)continue;
	//	deckui[i]->ImGuiDraw();
	//}
}

int SkillManager::IDSearch(const int BirthNum) {
	int result = 0;
	if (skill[m_DeckDate[BirthNum + m_DeckRemain]]->GetDeckIn()) {
		m_BirthNow = m_DeckDate[BirthNum + m_DeckRemain];
		skill[m_DeckDate[BirthNum + m_DeckRemain]]->SetBirth(true);
		deckui[BirthNum]->SetUse(true);
		result = skill[m_DeckDate[BirthNum + m_DeckRemain]]->GetID();
	}
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

//スキルのデータを渡す(攻撃)
void SkillManager::GetAttackSkillData(float& damage, int& delay, vector<std::vector<int>>& area, vector<std::vector<int>>& timer, int& DisX, int& DisY, string& name,int& Token) {
	if (skill[m_BirthNow]->GetSkillType() != SkillType::damege)
	{
		assert(0);
	}

	AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[m_BirthNow]);
	damage = atkSkill->GetDamege();		//ダメージ
	delay = skill[m_BirthNow]->Getlatency();		//硬直時間
	area = atkSkill->GetArea();		//範囲
	timer = atkSkill->GetTimer();
	name = atkSkill->GetStateName();		//付与状態
	Token = atkSkill->GetPoisonToken();
	//プレイヤーからの距離
	int l_DistanceX = {};
	int l_DistanceY = {};
	l_DistanceX = atkSkill->GetDistanceX();
	l_DistanceY = atkSkill->GetDistanceY();
	DisX = l_DistanceX;
	DisY = l_DistanceY;
}
void SkillManager::GetSkillType(int& SkillType) {
	SkillType = (int)skill[m_BirthNow]->GetSkillType();
}
//スキルのデータを渡す(特殊)
void SkillManager::GetSpecialSkillDate(int& delay,string& name) {
	SpecialSkill* specialskill = dynamic_cast<SpecialSkill*>(skill[m_BirthNow]);
	delay = skill[m_BirthNow]->Getlatency();
	name = specialskill->GetStateName();
}
//スキルのCSVを読み取る
void SkillManager::LoadCsvSkill(std::string& FileName, const int id) {
	
	std::ifstream file;
	std::stringstream popcom;

	file.open(FileName);
	popcom << file.rdbuf();
	file.close();

	std::string line;
	//アタックエリア用
	std::vector<std::vector<int> > MyVector;
	//タイマー用
	std::vector<std::vector<int> > MyTimer;

	while (std::getline(popcom, line)) {
		std::istringstream line_stream(line);
		std::string word;
		std::string word2;
		std::getline(line_stream, word, ',');
		if (word.find("//") == 0) {
			continue;
		}
		else if (word.find("ID") == 0) {
			std::getline(line_stream, word, ',');
			skill[id]->SetID(std::stoi(word));
		}
		else if (word.find("SkillType") == 0) {
			std::getline(line_stream, word, ',');
			if (std::stoi(word) == 0)
			{
				skill[id]->SetSkillType(SkillType::damege);
			}
			else if (std::stoi(word) == 1)
			{
				skill[id]->SetSkillType(SkillType::buff);
			}
			else if (std::stoi(word) == 2)
			{
				skill[id]->SetSkillType(SkillType::debuff);
			}
			else if (std::stoi(word) == 3)
			{
				skill[id]->SetSkillType(SkillType::max);
			}
		}
		else if (word.find("Delay") == 0) {
			std::getline(line_stream, word, ',');
			skill[id]->SetLatency(std::stoi(word));
			m_Delays[id] = std::stoi(word);
		}
		if (skill[id]->GetSkillType() == SkillType::damege) {
			if (word.find("Name") == 0) {
				std::getline(line_stream, word, ',');
				skill[id]->SetName(word);
			}
			else if (word.find("StateName") == 0) {
				std::getline(line_stream, word, ',');
				AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[id]);
				if (atkSkill != nullptr)
				{
					atkSkill->SetStateName(word);
				}
			}
			else if (word.find("TOKEN") == 0) {
				std::getline(line_stream, word, ',');
				AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[id]);
				if (atkSkill != nullptr)
				{
					atkSkill->SetPoisonToken(std::stoi(word));
				}
			}
			else if (word.find("InvocatingTime") == 0) {
				std::getline(line_stream, word, ',');
				skill[id]->SetInvocatingTime(std::stoi(word));
			}
			else if (word.find("restraintTime") == 0) {
				std::getline(line_stream, word, ',');
				skill[id]->SetRestraintTime(std::stoi(word));
			}
			else if (word.find("Rarity") == 0) {
				std::getline(line_stream, word, ',');
				skill[id]->SetRarity(std::stoi(word));
			}
			else if (word.find("PopRate") == 0) {
				std::getline(line_stream, word, ',');
				skill[id]->SetPopRate(std::stoi(word));
			}
			else if (word.find("Damege") == 0) {
				std::getline(line_stream, word, ',');
				AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[id]);
				if (atkSkill != nullptr)
				{
					atkSkill->SetDamege(std::stof(word));
				}
			}
			else if (word.find("DistanceX") == 0) {
				std::getline(line_stream, word, ',');
				AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[id]);
				if (atkSkill != nullptr)
				{
					atkSkill->SetDistanceX(std::stoi(word));
				}
			}
			else if (word.find("DistanceY") == 0) {
				std::getline(line_stream, word, ',');
				AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[id]);
				if (atkSkill != nullptr)
				{
					atkSkill->SetDistanceY(std::stoi(word));
				}
			}
			else if (word.find("AttackArea") == 0) {
				while (std::getline(line_stream, word)) {
					std::vector<int> row;

					for (char& x : word) {
						int X = x - '0';
						if (x != ' ')
							row.push_back(X);
					}
					MyVector.push_back(row);
				}
			}
			else if (word.find("AttackAreA") == 0) {
				while (std::getline(line_stream, word)) {
					std::vector<int> row;

					for (char& x : word) {
						int X = x - '0';
						if (x != ' ')
							row.push_back(X);
					}
					MyVector.push_back(row);
				}

				AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[id]);
				if (atkSkill != nullptr)
				{
					atkSkill->SetArea(MyVector);
				}
			}
			else if (word.find("Timer") == 0) {
				while (std::getline(line_stream, word)) {
					std::vector<int> row;
					stringstream ss;
					int X = 0;
					for (char& x : word) {
						ss << word;
						ss >> X;
						if (x != ' ')
							row.push_back(X);
					}
					MyTimer.push_back(row);
				}
			}
			else if (word.find("TimeR") == 0) {
				while (std::getline(line_stream, word)) {
					std::vector<int> row;
					stringstream ss;
					int X = 0;
					for (char& x : word) {
						ss << word;
						ss >> X;
						if (x != ' ')
							row.push_back(X);
					}
					MyTimer.push_back(row);
				}

				AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[id]);
				if (atkSkill != nullptr)
				{
					atkSkill->SetTimer(MyTimer);
				}
				break;
			}

		}
		else {
			if (word.find("StateName") == 0) {
				std::getline(line_stream, word, ',');
				SpecialSkill* specialSkill = dynamic_cast<SpecialSkill*>(skill[id]);
				if (specialSkill != nullptr)
				{
					specialSkill->SetStateName(word);
				}
			}
		}
	}

}

bool SkillManager::CreateSkill(int id) {

	std::string directory = "Resources/csv/chara/player/skill/Skill";

	std::stringstream ss;
	if (id >= 10) {
		ss << directory << id << ".csv";

	}
	else {
		ss << directory << "0" << id << ".csv";
	}
	std::string csv_ = ss.str();
	LoadCsvSkill(csv_, id);
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
void SkillManager::PushOnce2Deck(const int DeckNumber) {
	DeckUI* newdeckUi = nullptr;
	newdeckUi = new DeckUI();
	newdeckUi->Initialize();
	newdeckUi->InitState((int)deckui.size());
	newdeckUi->SetType((int)skill[DeckNumber]->GetSkillType());
	deckui.emplace_back(newdeckUi);
	m_DeckNum = (int)deckui.size();

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
	unique_ptr<DeckUI> newdeckui = make_unique<DeckUI>();
	newdeckui->Initialize();
	newdeckui->InitState(DeckCount);
	deckui.push_back(std::move(newdeckui));
	//手に入れたスキルのUIの更新
	for (auto i = 0; i < m_DeckDate.size(); i++) {
		deckui[i]->SetID(m_DeckDate[i]);
		deckui[i]->BirthIcon();
	}
	////手に入れたスキルのUIの更新
	//for (auto i = 0; i < m_DeckDate.size(); i++) {
	//	deckui[i]->SetType((int)skill[m_DeckDate[i]]->GetSkillType());
	//}
}
//リザルトに渡すデータ
void SkillManager::HandResultData(const int DeckID, vector<std::vector<int>>& area, int& DisX, int& DisY,int& Damage) {
	if (skill[DeckID]->GetSkillType() == SkillType::damege) {
		AttackSkill* atkSkill = dynamic_cast<AttackSkill*>(skill[DeckID]);
		area = atkSkill->GetArea();		//範囲
		//プレイヤーからの距離
		int l_DistanceX = {};
		int l_DistanceY = {};
		l_DistanceX = atkSkill->GetDistanceX();
		l_DistanceY = atkSkill->GetDistanceY();
		DisX = l_DistanceX;
		DisY = l_DistanceY; 
		Damage = (int)atkSkill->GetDamege();
	}
	else {
		return;
	}
}
//Text読み込み
void SkillManager::LoadText(const int Number) {
	skilltex[Number].baseSentence[0] = L"スキル：";
	skilltex[Number].baseSentence[1] = skilltex[Number].text_->GetSkillSentence(Number);
	skilltex[Number].baseSentence[2] = skilltex[Number].text_->GetSkillDamage(Number);
	skilltex[Number].text_->SetCreateSentence(skilltex[Number].baseSentence[0], skilltex[Number].baseSentence[1], skilltex[Number].baseSentence[2]);

}
//Text描画
void SkillManager::TextDraw(const int Number) {
	skilltex[Number].text_->Draw(dxcommon);
}