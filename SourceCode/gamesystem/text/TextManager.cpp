#include "TextManager.h"

const XMFLOAT2 kFirstRowPos{ 350.f,380.f };
const XMFLOAT2 kSecondRowPos{ 350.f,420.f };
const XMFLOAT2 kThirdRowPos{ 350.f, 460.f };


TextManager* TextManager::GetInstance() {
	static TextManager instance;
	return &instance;
}

void TextManager::Create() {
	conversation_.FirstFont = make_unique<Font>(m_word.FirstWord, kFirstRowPos, m_Color);
	conversation_.SecondFont = make_unique<Font>(m_word.SecondWord, kFirstRowPos, m_Color);
	conversation_.ThirdFont = make_unique<Font>(m_word.ThirdWord, kFirstRowPos, m_Color);
}


//初期化
void TextManager::Initialize() {
	//ワード追加
	CreateWord(FIRST, L"タイトルだよ", L"Bボタンをおせば", L"ゲームシーンだよ");
	//コンヴァージョン初期化
	Create();

	//フォントのあれこれ
	conversation_.FirstFont->SetPos(kFirstRowPos);
	conversation_.SecondFont->SetPos(kSecondRowPos);
	conversation_.ThirdFont->SetPos(kThirdRowPos);
}
//描画?
void TextManager::Draw() {
	//コンヴァージョン.フォントドローする
	conversation_.FirstFont->Draw();
	conversation_.SecondFont->Draw();
	conversation_.ThirdFont->Draw();
	Font::PostDraw();
}

void TextManager::FlowDraw() {

	if (conversation_.FirstFont->FlowText()) {
		if (conversation_.SecondFont->FlowText()) {
			if (conversation_.ThirdFont->FlowText()) {
				if (!change) {
					conversation_.FirstFont->SetString(L"サイゴまでミタナ");
					conversation_.SecondFont->SetString(L"オレはオマエを");
					conversation_.ThirdFont->SetString(L"ユルさない");

					change = true;
				}
			}
			conversation_.ThirdFont->Draw();
		}
		conversation_.SecondFont->Draw();
	}
	conversation_.FirstFont->Draw();

	Font::PostDraw();
}


void TextManager::SetAllColor(const XMVECTOR& color) {
	conversation_.FirstFont->SetColor(color);
	conversation_.SecondFont->SetColor(color);
	conversation_.ThirdFont->SetColor(color);
}

void TextManager::SetOnceColor(int row, const XMVECTOR& color) {
	assert(row < 3);
	if (row == 0) {
		conversation_.FirstFont->SetColor(color);
	} else if (row == 1) {
		conversation_.SecondFont->SetColor(color);
	} else if (row == 2) {
		conversation_.ThirdFont->SetColor(color);
	}
}

//名前から文字列を呼び出しセットする
void TextManager::SetConversation(TITLE name, const XMVECTOR& color) {
	std::map<TextManager::TITLE, Word>::iterator itr = wordlist_.find(name);

	if (old != itr->first) {
		for (int i = 0; i < 3; i++) {
			flag[i] = true;
			next_f[i] = false;
		}
	}

	old = itr->first;

	CreateCon(conversation_, itr->second);

	conversation_.FirstFont->SetColor(color);
	conversation_.SecondFont->SetColor(color);
	conversation_.ThirdFont->SetColor(color);

}

//名前と文字列セットで保存
void TextManager::CreateWord(TITLE name, wchar_t* tex1, wchar_t* tex2, wchar_t* tex3) {
	Word temp = SetWord(tex1, tex2, tex3);

	wordlist_.insert(std::make_pair(name, temp));
}

void TextManager::SetRowPosition(float posX) {
	//フォントのあれこれ
	conversation_.FirstFont->SetPos({ posX,kFirstRowPos.y });
	conversation_.SecondFont->SetPos({ posX, kSecondRowPos.y });
	conversation_.ThirdFont->SetPos({ posX,kThirdRowPos.y });
}


//文字列保存
TextManager::Word TextManager::SetWord(wchar_t* tex1, wchar_t* tex2, wchar_t* tex3) {
	Word word = {};
	word.FirstWord = tex1;
	word.SecondWord = tex2;
	word.ThirdWord = tex3;
	return word;
}
void TextManager::NoneText() {
	std::map<TextManager::TITLE, Word>::iterator itr = wordlist_.find(FIRST);

	CreateCon(conversation_, itr->second);
}
//文字列呼び出し
//TextManager::Conversation TextManager::CreateConversation(Word word)
//{
//	/*Conversation* temp = {};
//	temp->FirstFont->SetString(word.FirstWord);
//	temp->SecondFont->SetString(word.SecondWord);
//	temp->ThirdFont->SetString(word.ThirdWord);
//	return temp;*/
//}

void TextManager::CreateCon(Conversation& con, Word word) {
	con.FirstFont->SetString(word.FirstWord);
	con.SecondFont->SetString(word.SecondWord);
	con.ThirdFont->SetString(word.ThirdWord);
}
