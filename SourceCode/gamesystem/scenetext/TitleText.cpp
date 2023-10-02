#include "TitleText.h"
TitleText::TitleText() {
};
//初期化
void TitleText::Initialize(DirectXCommon* dxCommon) {
	text_ = make_unique<TextManager>();
	text_->Initialize(dxCommon);
	text_->SetConversation(TextManager::FIRST);
}
//テキストの選択
void TitleText::SelectText(TextManager::TITLE name) {
	text_->SetConversation(name);
}
//色変更
void TitleText::ChangeColor(int row, const XMVECTOR& color) {
	text_->SetOnceColor(row, color);
}
//すべての色変更
void TitleText::ChangeAllColor(const XMVECTOR& color) {
	text_->SetAllColor(color);
}

void TitleText::SpriteDraw(DirectXCommon* dxCommon) {
	text_->TestDraw(dxCommon);
}