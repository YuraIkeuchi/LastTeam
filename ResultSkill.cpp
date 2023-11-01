#include "ResultSkill.h"
#include "Helper.h"
#include "ImageManager.h"

ResultSkill::ResultSkill() {

}

ResultSkill::~ResultSkill() {
}

void ResultSkill::Initialize() {
	backScreen = IKESprite::Create(ImageManager::FEED, { 0.f,0.f }, { 0.f,0.f, 0.f, 0.1f });
	backScreen->SetSize({ 1280.f,720.f });



}

void ResultSkill::Update() {

	for (ResultUI& resultUI : choiceSkills) {
		if (resultUI.isSkill) {
			resultUI.number->Update();
		}
	}
}

void ResultSkill::Draw() {
	if (!isStart) { return; }

	IKESprite::PreDraw();
	backScreen->Draw();
	for (ResultUI& resultUI:choiceSkills) {
		resultUI.icon->Draw();
		if (resultUI.isSkill) {
			resultUI.number->Draw();
		}
	}
	IKESprite::PostDraw();

}

void ResultSkill::CreateResult(std::vector<int>& notDeck, std::vector<int>& notPassives) {
	if (isStart) { return; }
	vector<int> noDeck = notDeck;
	vector<int> noPassive = notPassives;

	ResultUI resultUI;
	resultUI.ID = noDeck[0];
	resultUI.position = BasePos[0];
	resultUI.icon= IKESprite::Create(ImageManager::ACTIONUI, { 0.0f,0.0f });
	resultUI.icon->SetAnchorPoint({ 0.5f,0.5f });
	resultUI.icon->SetPosition(resultUI.position);
	resultUI.isSkill = true;
	if (resultUI.isSkill) {
		resultUI.number = make_unique<DrawNumber>();
		resultUI.number->Initialize();
		resultUI.number->SetNumber(resultUI.ID);
		resultUI.number->SetPosition(resultUI.position);
	}

	choiceSkills.push_back(std::move(resultUI));

	ResultUI passiveUI;
	passiveUI.ID = noPassive[0];
	passiveUI.position = BasePos[1];
	passiveUI.icon = IKESprite::Create(ImageManager::PASSIVE_01 + passiveUI.ID, { 0.0f,0.0f });
	passiveUI.icon->SetAnchorPoint({ 0.5f,0.5f });
	passiveUI.icon->SetPosition(passiveUI.position);
	passiveUI.isSkill = false;

	choiceSkills.push_back(std::move(passiveUI));

	isStart = true;
}
