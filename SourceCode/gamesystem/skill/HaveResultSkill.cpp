#include "HaveResultSkill.h"
#include "Helper.h"
#include "ImageManager.h"
#include <Input.h>
#include <Easing.h>

HaveResultSkill::HaveResultSkill() {

}

HaveResultSkill::~HaveResultSkill() {
}

void HaveResultSkill::Initialize() {
	backScreen = IKESprite::Create(ImageManager::FEED, { 0.f,0.f }, { 0.f,0.f, 0.f, 0.5f });
	backScreen->SetSize({ 1280.f,720.f });
	selectFrame = IKESprite::Create(ImageManager::PASSIVE_FRAME, { 200.f,200.f });
	selectFrame->SetAnchorPoint({ 0.5f,0.5f });
}

void HaveResultSkill::Update() {
	for (HaveUI& resultUI : haveSkills) {
		if (resultUI.isSkill) {
			resultUI.number->Update();
		}
	}

	for (HaveUI& PassiveUI : havePassive) {
		if (PassiveUI.isSkill) {
			PassiveUI.number->Update();
		}
	}
}

void HaveResultSkill::Draw() {

	IKESprite::PreDraw();
	backScreen->Draw();
	//selectFrame->Draw();
	for (HaveUI& resultUI : haveSkills) {
		resultUI.icon->Draw();
		if (resultUI.isSkill) {
			resultUI.number->Draw();
		}
	}

	for (HaveUI& PassiveUI : havePassive) {
		PassiveUI.icon->Draw();
	}

	IKESprite::PostDraw();
}
void HaveResultSkill::ImGuiDraw() {
	if (haveSkills.empty()) { return; }
	if (havePassive.empty()) { return; }
	ImGui::Begin("Have");
	for (auto i = 0; i < haveSkills.size(); i++) {
		ImGui::Text("HaveID[%d]:%d", i, haveSkills[i].ID);
	}
	for (auto i = 0; i < havePassive.size(); i++) {
		ImGui::Text("PassiveHaveID[%d]:%d", i, havePassive[i].ID);
	}
	ImGui::End();
}

//持っているスキルの検索
void HaveResultSkill::HaveAttackSkill(std::vector<int> Deck,
	int DeckSize) {
	haveSkills.resize(DeckSize);
	for (auto i = 0; i < haveSkills.size(); i++) {
		haveSkills[i].ID = Deck[i];
		CreateAttackSkill(i, haveSkills[i].ID);
	}
}
//持っているパッシブ
void HaveResultSkill::HavePassiveSkill(std::vector<int> Passive,
	int PassiveSize) {
	havePassive.resize(PassiveSize);
	for (auto i = 0; i < havePassive.size(); i++) {
		havePassive[i].ID = Passive[i];
		CreatePassiveSkill(i, havePassive[i].ID);
	}
}
//攻撃スキルの表示
void HaveResultSkill::CreateAttackSkill(int num,int id) {
	XMFLOAT2 l_BasePos = { 200.0f,500.0f };
	haveSkills[num].position = { l_BasePos.x + (num * 100.0f),l_BasePos.y };
	haveSkills[num].icon = IKESprite::Create(ImageManager::ACTIONUI, { 0.0f,0.0f });
	haveSkills[num].icon->SetSize({ 64.0f,64.0f });
	haveSkills[num].icon->SetAnchorPoint({ 0.5f,0.5f });
	haveSkills[num].icon->SetPosition(haveSkills[num].position);
	haveSkills[num].number = make_unique<DrawNumber>();
	haveSkills[num].number->Initialize();
	haveSkills[num].number->SetNumber(haveSkills[num].ID);
	haveSkills[num].number->SetPosition(haveSkills[num].position);
}
//パッシブスキルの表示
void HaveResultSkill::CreatePassiveSkill(int num, int id) {
	XMFLOAT2 l_BasePos = { 200.0f,500.0f };
	havePassive[num].position = { l_BasePos.x + ((num + (int)haveSkills.size()) * 100.0f),l_BasePos.y };
	havePassive[num].icon = IKESprite::Create(ImageManager::PASSIVE_01 + havePassive[num].ID, {0.0f,0.0f});
	havePassive[num].icon->SetSize({ 64.0f,64.0f });
	havePassive[num].icon->SetAnchorPoint({ 0.5f,0.5f });
	havePassive[num].icon->SetPosition(havePassive[num].position);
	havePassive[num].number = make_unique<DrawNumber>();
	havePassive[num].number->Initialize();
	havePassive[num].number->SetNumber(havePassive[num].ID);
	havePassive[num].number->SetPosition(havePassive[num].position);
}