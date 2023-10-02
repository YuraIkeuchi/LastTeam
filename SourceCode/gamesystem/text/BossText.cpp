#include "BossText.h"
#include "Helper.h"
#include "ImageManager.h"
#include "Easing.h"
BossText::BossText() {
}

void BossText::Initialize(DirectXCommon* dxCommon) {
	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::AISATU);
}

void BossText::SelectText(TextManager::Name_Second name) {
	text_->SetSecondConversation(name);
}
void BossText::SelectText(TextManager::Name_First name, const XMVECTOR& color) {
	text_->SetConversation(name,color);
}

void BossText::SelectText(TextManager::Name_Cap name, const XMVECTOR& color)
{
	text_->SetCapConversation(name);
}

void BossText::SelectText(TextManager::Name_Fourth name) {
	text_->SetFourthConversation(name);
}
void BossText::SelectText(TextManager::Name_CameraBoss name, const XMVECTOR& color) {
	text_->SetCameraBossConversation(name);
}
void BossText::SelectText(TextManager::Name_Last name, const XMVECTOR& color) {
	text_->SetLastConversation(name);
}
void BossText::SpriteDraw(DirectXCommon* dxCommon) {

	text_->TestDraw(dxCommon);
}

void BossText::Display()
{

}

void BossText::ChangeColor(int row, const XMVECTOR& color) {
	text_->SetOnceColor(row, color);
}

void BossText::ChangeColor(const XMVECTOR& color) {
	text_->SetAllColor(color);

}
