﻿#include "TextManager.h"


void TextManager::Create(DirectXCommon* dxcomon)
{
	conversation_.FirstFont = new Font();
	conversation_.SecondFont = new Font();
	conversation_.ThirdFont = new Font();
	conversation_.FirstFont->LoadFont(dxcomon);
	conversation_.SecondFont->LoadFont(dxcomon);
	conversation_.ThirdFont->LoadFont(dxcomon);
	conversation_.FirstFont->SetColor(color_);
	conversation_.SecondFont->SetColor(color_);
	conversation_.ThirdFont->SetColor(color_);
	old_conversation_.FirstFont = new Font();
	old_conversation_.SecondFont = new Font();
	old_conversation_.ThirdFont = new Font();
	old_conversation_.FirstFont->LoadFont(dxcomon);
	old_conversation_.SecondFont->LoadFont(dxcomon);
	old_conversation_.ThirdFont->LoadFont(dxcomon);
	old_conversation_.FirstFont->SetColor(color_);
	old_conversation_.SecondFont->SetColor(color_);
	old_conversation_.ThirdFont->SetColor(color_);
}


//初期化
void TextManager::Initialize(DirectXCommon* dxcomon)
{
	//ワード追加
	CreateWord(NONE, L"");
	CreateWord(TITLE, L"Bボタン:ゲーム");
	CreateWord(MAP_01, L"始めようここから！");
	CreateWord(MAP_BATTLE, L"バトルカードいっぱい！");
	CreateWord(MAP_PASSIVE, L"パッシブが欲しいあなたへ！");
	CreateWord(MAP_BOSS, L"手強そうだ！");
	CreateWord(TUTORIAL_START, L"ここでは戦い方を教える", L"まずはパッドかスティックで移動してみろ");
	CreateWord(TUTORIAL_GET, L"ステージに何か出てきたぞ、取ってみろ");
	CreateWord(TUTORIAL_EXPLAIN, L"下にカードが出ただろう", L"そのカードに応じてプレイヤーは動ける");
	CreateWord(TUTORIAL_MARK, L"赤いエリアが出てきただろう", L"それが攻撃エリアだと思ってくれればいい");
	CreateWord(TUTORIAL_TEXT_ATTACK, L"エリアに敵を収めて",L"Bボタンで攻撃してみろ");
	CreateWord(TUTORIAL_TEXT_DAMAGE, L"よし、よくできた", L"じゃあ敵を倒してみよう");
	CreateWord(TUTORIAL_SKILL, L"敵を倒すとスキルが手に入るぞ", L"スキルは攻撃やパッシブなどがあるぞ");
	CreateWord(TUTORIAL_CHOICE, L"下の3つのスキルから", L"1つ選択してみよう");
	CreateWord(TUTORIAL_END, L"手に入れたスキルを駆使して",L"敵を倒そう!");
	CreateWord(RESULT, L"スキル効果");

	CreatePassiveSentence(L"リロードが早くなるぞ");
	CreatePassiveSentence(L"全回復、体力増加");
	CreatePassiveSentence(L"リロードしても消えないぞ");
	CreatePassiveSentence(L"毒の時間が長いぞ");
	CreatePassiveSentence(L"毒の威力が上がるぞ");
	CreatePassiveSentence(L"ドレイン威力が上がるぞ");
	CreatePassiveSentence(L"リロードしたら攻撃だ");

	CreateSkillSentence(L"ドドドな近距離攻撃");
	CreateSkillDamage(L"威力:15");
	CreateSkillSentence(L"ドゴンな近中距離攻撃");
	CreateSkillDamage(L"威力:8");
	CreateSkillSentence(L"バコン!な一列攻撃");
	CreateSkillDamage(L"威力:5");
	CreateSkillSentence(L"ザクッな近距離攻撃");
	CreateSkillDamage(L"威力:5");
	CreateSkillSentence(L"少し強い近中距離攻撃");
	CreateSkillDamage(L"威力:10");
	CreateSkillSentence(L"少し強い中遠距離攻撃");
	CreateSkillDamage(L"威力:10");
	CreateSkillSentence(L"ドドドな遠距離攻撃");
	CreateSkillDamage(L"威力:15");
	CreateSkillSentence(L"イヤーナ毒攻撃だ");
	CreateSkillDamage(L"威力:3");
	CreateSkillSentence(L"回復できる攻撃だ");
	CreateSkillDamage(L"威力:9");
	CreateSkillSentence(L"次の攻撃を強く!");
	CreateSkillDamage(L"威力:次のスキル2倍");

	//ラスボス
	//コンヴァージョン初期化
	Create(dxcomon);
}
//描画?
void TextManager::Draw(DirectXCommon* dxcommon)
{
	//コンヴァージョン.フォントドローする
	conversation_.FirstFont->Draw(dxcommon);
	conversation_.SecondFont->Draw(dxcommon);
	conversation_.ThirdFont->Draw(dxcommon);
	Font::PostDraw(dxcommon);
}

void TextManager::TestDraw(DirectXCommon* dxcommon)
{

	length = 0;
	test = conversation_.FirstFont->GetString();
	test1 = conversation_.SecondFont->GetString();
	test2 = conversation_.ThirdFont->GetString();


	len[0] = wcslen(test);
	len[1] = wcslen(test1);
	len[2] = wcslen(test2);


	conversation_.FirstFont->TestSet(test, len[0], flag[0], next_f[0]);
	conversation_.FirstFont->Draw(dxcommon);
	if (next_f[0] == true) {
		conversation_.SecondFont->TestSet(test1, len[1], flag[1], next_f[1]);
		conversation_.SecondFont->Draw(dxcommon);
	}
	if (next_f[1] == true) {
		conversation_.ThirdFont->TestSet(test2, len[2], flag[2], next_f[2]);
		conversation_.ThirdFont->Draw(dxcommon);
	}

	Font::PostDraw(dxcommon);
}

void TextManager::Test()
{
	wchar_t* test = conversation_.FirstFont->GetString();
	wchar_t* test1 = conversation_.SecondFont->GetString();
	wchar_t* test2 = conversation_.ThirdFont->GetString();

	conversation_.FirstFont->SetString(test);
	conversation_.SecondFont->SetString(test1);
	conversation_.ThirdFont->SetString(test2);
}

void TextManager::SetAllColor(const XMVECTOR& color)
{
	conversation_.FirstFont->SetColor(color);
	conversation_.SecondFont->SetColor(color);
	conversation_.ThirdFont->SetColor(color);
}

void TextManager::SetOnceColor(int row, const XMVECTOR& color)
{
	assert(row < 3);
	if (row == 0) {
		conversation_.FirstFont->SetColor(color);
	}
	else if (row == 1) {
		conversation_.SecondFont->SetColor(color);
	}
	else if (row == 2) {
		conversation_.ThirdFont->SetColor(color);
	}
}

//名前から文字列を呼び出しセットする
void TextManager::SetConversation(Name name, const XMFLOAT2& pos, const XMVECTOR& color)
{
	XMFLOAT2 l_FirstPos = pos;
	XMFLOAT2 l_SecondPos = { pos.x,pos.y - 40.0f };
	XMFLOAT2 l_ThirdPos = { pos.x,pos.y - 80.0f };
	std::map<TextManager::Name, Word>::iterator itr = wordlist_.find(name);

	if (old != itr->first) {
		for (int i = 0; i < 3; i++) {
			flag[i] = true;
			next_f[i] = false;
		}
	}

	old = itr->first;

	GetWordSize(itr->second);

	CreateCon(conversation_, itr->second);

	conversation_.FirstFont->SetColor(color);
	conversation_.SecondFont->SetColor(color);
	conversation_.ThirdFont->SetColor(color);

	//フォントのあれこれ
	conversation_.FirstFont->SetPos(l_FirstPos);
	conversation_.SecondFont->SetPos(l_SecondPos);
	conversation_.ThirdFont->SetPos(l_ThirdPos);
}

void TextManager::SetCreateSentence(wchar_t* tex1, wchar_t* tex2, wchar_t* tex3) {
	
	CreateWord(EXTRA, tex1, tex2, tex3);

	std::map<TextManager::Name, Word>::iterator itr = wordlist_.find(EXTRA);


	for (int i = 0; i < 3; i++) {
			flag[i] = true;
			next_f[i] = false;
	}

	GetWordSize(itr->second);

	CreateCon(conversation_, itr->second);

}

//名前と文字列セットで保存
void TextManager::CreateWord(Name name, wchar_t* tex1, wchar_t* tex2, wchar_t* tex3)
{
	Word temp = SetWord(tex1, tex2, tex3);

	wordlist_.insert(std::make_pair(name, temp));
}
//
//void TextManager::SetRowPosition(float posX)
//{
//	//フォントのあれこれ
//	conversation_.FirstFont->SetPos({ posX,kFirstRowPos.y });
//	conversation_.SecondFont->SetPos({ posX, kSecondRowPos.y });
//	conversation_.ThirdFont->SetPos({ posX,kThirdRowPos.y });
//}

void TextManager::GetWordSize(Word word)
{
	len[0] = wcslen(word.FirstWord);
	len[1] = wcslen(word.SecondWord);
	len[2] = wcslen(word.ThirdWord);
}

//文字列保存
TextManager::Word TextManager::SetWord(wchar_t* tex1, wchar_t* tex2, wchar_t* tex3)
{
	Word word = {};
	word.FirstWord = tex1;
	word.SecondWord = tex2;
	word.ThirdWord = tex3;
	return word;
}
void TextManager::NoneText()
{
	std::map<TextManager::Name, Word>::iterator itr = wordlist_.find(NONE);

	CreateCon(conversation_, itr->second);
}

void TextManager::CreateCon(Conversation con, Word word)
{
	con.FirstFont->SetString(word.FirstWord);
	con.SecondFont->SetString(word.SecondWord);
	con.ThirdFont->SetString(word.ThirdWord);
}

void TextManager::CreatePassiveSentence(wchar_t* tex1) {
	passiveSentence.emplace_back(tex1);
}

void TextManager::CreateSkillSentence(wchar_t* tex1) {
	skillSentence.emplace_back(tex1);
}

void TextManager::CreateSkillDamage(wchar_t* tex1) {
	skillDamage.emplace_back(tex1);
}
