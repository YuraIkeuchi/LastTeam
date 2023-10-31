#include "ResultSkill.h"
#include "Helper.h"

ResultSkill::ResultSkill() {
}

ResultSkill::~ResultSkill() {
}

void ResultSkill::Initialize() {


}

void ResultSkill::Update() {

	for (std::unique_ptr<ActionUI>& skill:choiceSkills) {
		skill->Update();
	}

}

void ResultSkill::Draw() {
}

void ResultSkill::GetNotDeckNumber(std::vector<int>& notDeck) {
	std::vector<int>l_Deck = notDeck;
	int num = l_Deck[0];

	//アクションUIのセット
	std::unique_ptr<ActionUI> actUi = nullptr;
	actUi = std::make_unique<ActionUI>();
	actUi->Initialize();
	actUi->SetID(num);
	choiceSkills.push_back(std::move(actUi));
}

void ResultSkill::SetPassiveId(std::vector<int>& gotPassives) {
	GetPassives = gotPassives;
}

void ResultSkill::RandSkill() {
	if (!isStart) { return; }
	//パッシブ最大数（後で絶対変えろ）
	int n = Helper::GetInstance()->GetRanNum(1,3);
	std::unique_ptr<Passive> passive;


}
