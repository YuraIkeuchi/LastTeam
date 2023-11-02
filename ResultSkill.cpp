#include "ResultSkill.h"
#include "Helper.h"
#include "ImageManager.h"
#include <Input.h>

ResultSkill::ResultSkill() {

}

ResultSkill::~ResultSkill() {
}

void ResultSkill::Initialize() {
	backScreen = IKESprite::Create(ImageManager::FEED, { 0.f,0.f }, { 0.f,0.f, 0.f, 0.3f });
	backScreen->SetSize({ 1280.f,720.f });
	wchar_t* sample = L"よくたおした！こんやはカツドンだ！";
	font = std::make_unique<Font>(sample, XMFLOAT2{300.f,100.f});

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
	font->Draw();
	Font::PostDraw();

}

void ResultSkill::InDeck(std::vector<int>& Deck) {
	std::vector<int> itr = Deck;
	for (ResultUI& resultUI : choiceSkills) {
		if (!resultUI.isSkill) { continue; }
		itr.push_back(resultUI.ID);
	}
	Deck.resize(itr.size());
	for (int i = 0; i < Deck.size(); i++) {
		Deck[i] = itr[i];
	}
}

void ResultSkill::InPassive(std::vector<int>& Passive) {
	std::vector<int> itr = Passive;
	for (ResultUI& resultUI : choiceSkills) {
		if (resultUI.isSkill) { continue; }
		itr.push_back(resultUI.ID);
	}
	Passive.resize(itr.size());
	for (int i = 0; i < Passive.size(); i++) {
		Passive[i] = itr[i];
	}
}

void ResultSkill::CreateResult(std::vector<int>& notDeck, std::vector<int>& notPassives) {
	if (isStart) { return; }
	vector<int> noDeck = notDeck;
	vector<int> noPassive = notPassives;
	if(noDeck.size()==0|| noPassive.size()==0){
		isStart = true;
		return;
	}

	//スキル
	ResultUI resultUI= CreateUI(true, noDeck[0], BasePos[nowPos]);
	choiceSkills.push_back(std::move(resultUI));
	//パッシブ
	ResultUI passiveUI = CreateUI(false, noPassive[0], BasePos[nowPos]);
	choiceSkills.push_back(std::move(passiveUI));
	if (noPassive.size() > 1) {
		ResultUI passiveUI2 = CreateUI(false, noPassive[1], BasePos[nowPos]);
		choiceSkills.push_back(std::move(passiveUI2));
	}

	isStart = true;
}

ResultSkill::ResultUI ResultSkill::CreateUI(bool isSkill, int id, XMFLOAT2 pos) {
	ResultUI resultUI;
	resultUI.ID = id;
	resultUI.position = pos;
	resultUI.isSkill = isSkill;
	if (resultUI.isSkill) {
		resultUI.icon = IKESprite::Create(ImageManager::ACTIONUI, { 0.0f,0.0f });
		resultUI.number = make_unique<DrawNumber>();
		resultUI.number->Initialize();
		resultUI.number->SetNumber(resultUI.ID);
		resultUI.number->SetPosition(resultUI.position);
	} else {
		resultUI.icon = IKESprite::Create(ImageManager::PASSIVE_01 + resultUI.ID, { 0.0f,0.0f });
	}
	resultUI.icon->SetAnchorPoint({ 0.5f,0.5f });
	resultUI.icon->SetSize({ 128.f,128.f });
	resultUI.icon->SetPosition(resultUI.position);
	
	nowPos++;
	return resultUI;
}
