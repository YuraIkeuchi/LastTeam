#include "ResultSkill.h"
#include "Helper.h"
#include "ImageManager.h"

ResultSkill::ResultSkill() {

	backScreen = IKESprite::Create(ImageManager::FEED, { 0.f,0.f }, { 0.f,0.f, 0.f, 0.1f });
	backScreen->SetSize({1280.f,720.f});
}

ResultSkill::~ResultSkill() {
}

void ResultSkill::Initialize() {


}

void ResultSkill::Update() {
	RandSkill();
	for (std::unique_ptr<ActionUI>& skill : choiceSkills) {
		skill->Update();
	}

}

void ResultSkill::Draw() {

	IKESprite::PreDraw();
	backScreen->Draw();
	IKESprite::PostDraw();

	for (std::unique_ptr<ActionUI>& skill : choiceSkills) {
		skill->Draw();
	}

	for (std::unique_ptr<Passive>& passive : choicePassives) {
		passive->Draw();
	}
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
	int r_num = Helper::GetInstance()->GetRanNum(1, 2);
	std::unique_ptr<Passive> passive;
	passive = std::make_unique<Passive>(1, XMFLOAT2(360.f, 240.f), XMFLOAT2{256.f,256.f});
	choicePassives.push_back(std::move(passive));
	isStart = false;

}
