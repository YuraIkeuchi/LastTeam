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
	//for (HaveUI& resultUI : haveSkills) {
	//	if (resultUI.isSkill) {
	//		resultUI.number->Update();
	//	}
	//}

	//for (HaveUI& PassiveUI : havePassive) {
	//	if (PassiveUI.isSkill) {
	//		PassiveUI.number->Update();
	//	}
	//}
}

void HaveResultSkill::Draw() {

	//IKESprite::PreDraw();
	//backScreen->Draw();
	//selectFrame->Draw();
	//for (HaveUI& resultUI : haveSkills) {
	//	resultUI.icon->Draw();
	//	if (resultUI.isSkill) {
	//		resultUI.number->Update();
	//	}
	//}

	//for (HaveUI& PassiveUI : havePassive) {
	//	PassiveUI.icon->Draw();
	//	if (PassiveUI.isSkill) {
	//		PassiveUI.number->Update();
	//	}
	//}

	//IKESprite::PostDraw();
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
	}
}
//持っているパッシブ
void HaveResultSkill::HavePassiveSkill(std::vector<int> Passive,
	int PassiveSize) {
	havePassive.resize(PassiveSize);
	for (auto i = 0; i < havePassive.size(); i++) {
		havePassive[i].ID = Passive[i];
	}
}
