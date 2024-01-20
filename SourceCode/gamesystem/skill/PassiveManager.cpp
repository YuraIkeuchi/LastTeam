#include "PassiveManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

PassiveManager* PassiveManager::GetInstance() {
	static PassiveManager instance;
	return &instance;
}

void PassiveManager::Initialize(DirectXCommon* dxCommon) {
	dxcommon = dxCommon;
	passiveTexts.resize(15);
	for (int i = 0; i < 15;i++) {
		passiveTexts[i].text_ = make_unique<TextManager>();
		passiveTexts[i].text_->Initialize(dxcommon, LOAD_PASSIVE);
		passiveTexts[i].text_->SetConversation(TextManager::RESULT, { -200.0f,80.0f });
	}
}
//Text読み込み
void PassiveManager::LoadText(const int Number) {
	passiveTexts[Number].baseSentence[0] = L"パッシブ：";
	passiveTexts[Number].baseSentence[1] = passiveTexts[Number].text_->GetPassiveName(Number);
	passiveTexts[Number].baseSentence[2] = passiveTexts[Number].text_->GetPasiveSentence(Number);
	passiveTexts[Number].text_->SetCreateSentence(passiveTexts[Number].baseSentence[0], passiveTexts[Number].baseSentence[1], passiveTexts[Number].baseSentence[2]);

}

//Text描画
void PassiveManager::TextDraw(const int Number) {
	passiveTexts[Number].text_->Draw(dxcommon);
}