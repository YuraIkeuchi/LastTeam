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
	CreateWord(NONE, L"");
	CreateWord(TITLE_01, L"Bボタン:ゲーム");
	CreateWord(TITLE_02, L"SPACEキー:マップ");
	CreateWord(TITLE_03, L"Xボタン:チュートリアル");
	CreateWord(MAP_01, L"はじめようここから！");
	CreateWord(MAP_BATTLE, L"バトルカードいっぱい！");
	CreateWord(MAP_HEAL, L"回復したいあなたへ！");
	CreateWord(MAP_BOSS, L"てごわそうだ！");
	CreateWord(TUTORIAL_START, L"ここでは戦い方を教える");
	CreateWord(TUTORIAL_MOVE, L"まずは方向パッドで移動してみろ");
	CreateWord(TUTORIAL_GET, L"ステージに何か出てきたぞ、取ってみろ");
	CreateWord(TUTORIAL_EXPLAIN1, L"下にカードが出ただろう");
	CreateWord(TUTORIAL_EXPLAIN2, L"そのカードに応じてプレイヤーは動ける");
	CreateWord(TUTORIAL_MARK1, L"赤いエリアが出てきただろう");
	CreateWord(TUTORIAL_MARK2, L"それが攻撃エリアだと思ってくれればいい");
	CreateWord(TUTORIAL_TEXT_ATTACK, L"エリアに敵を収めてAボタンで攻撃してみろ");
	CreateWord(TUTORIAL_TEXT_DAMAGE, L"よし、よくできた");
	CreateWord(TUTORIAL_ENEMYKNOCK, L"じゃあ敵を倒してみよう");
	CreateWord(TUTORIAL_SKILL, L"敵を倒すとスキルが手に入るぞ");
	CreateWord(TUTORIAL_SKILL2, L"スキルは攻撃やパッシブなどいろんな物があるぞ");
	CreateWord(TUTORIAL_SKILL3, L"手に入れたスキルを駆使して");
	CreateWord(TUTORIAL_END, L"敵を倒そう!");
}


wchar_t* TextManager::SearchText(TextManager::TEXT text) {
	
	return 	wordLists[text];
}

void TextManager::CreateWord(TextManager::TEXT text, wchar_t* word) {

	wordLists[text] = word;
}