#include "TextManager.h"

const XMFLOAT2 kFirstRowPos{ 350.f,380.f };
const XMFLOAT2 kSecondRowPos{ 350.f,420.f };
const XMFLOAT2 kThirdRowPos{ 350.f, 460.f };


TextManager* TextManager::GetInstance() {
	static TextManager instance;
	return &instance;
}

//初期化
void TextManager::Initialize() {
	//ワード追加
	CreateWord(TITLE_01, L"タイトルだよ");
	CreateWord(TITLE_02, L"Bボタンをおせば");
	CreateWord(TITLE_03, L"ゲームシーンだよ");

	
}


wchar_t* TextManager::SearchText(TextManager::TEXT text) {
	
	return 	wordLists[text];
}

void TextManager::CreateWord(TextManager::TEXT text, wchar_t* word) {

	wordLists[text] = word;
}