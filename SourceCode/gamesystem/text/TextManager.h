#pragma once
#include"DirectXCommon.h"
#include"VariableCommon.h"
#include"Font.h"
#include <map>
using namespace std;         //  名前空間指定
class TextManager
{
private:
	struct Word {
		wchar_t* FirstWord;//ˆês–Ú
		wchar_t* SecondWord;//“ñs–Ú
		wchar_t* ThirdWord;//ŽOs–Ú
	};
	struct Conversation
	{
		Font* FirstFont;
		Font* SecondFont;
		Font* ThirdFont;
	};

public:
	enum Name
	{
		NONE = 0,
		TITLE,
		MAP_01,
		MAP_BATTLE,
		MAP_HEAL,
		MAP_BOSS,
		TUTORIAL_START,
		TUTORIAL_GET,
		TUTORIAL_EXPLAIN,
		TUTORIAL_MARK,
		TUTORIAL_TEXT_ATTACK,
		TUTORIAL_TEXT_DAMAGE,
		TUTORIAL_ENEMYDESTROY,
		TUTORIAL_SKILL,
		TUTORIAL_END,
		RESULT,
	};

	static TextManager* GetInstance();

	//
	void Create(DirectXCommon* dxcomon);

	void Initialize(DirectXCommon* dxcomon);

	void Draw(DirectXCommon* dxcommon);

	void TestDraw(DirectXCommon* dxcommon);

	void Test();

	void SetAllColor(const XMVECTOR& color = { 1.f,1.f,1.f,1.f });

	void SetOnceColor(int row, const XMVECTOR& color = { 1.f,1.f,1.f,1.f });

	void SetConversation(Name name = NONE, const XMVECTOR& color = { 1.f,1.f,1.f,1.f });

	void NoneText();

	void GetWordSize(Word word);

	void SetRowPosition(float posX);
private:

	//
	void CreateWord(Name name, wchar_t* tex1, wchar_t* tex2 = L" ", wchar_t* tex3 = L" ");
	//
	Word SetWord(wchar_t* tex1, wchar_t* tex2, wchar_t* tex3);
	//
	Conversation CreateConversation(Word word);

	void CreateCon(Conversation con, Word word);
private:
	std::map<TextManager::Name, Word> wordlist_;

	Conversation conversation_ = {};
	Conversation old_conversation_ = {};

	Conversation bossconversation_ = {};

	XMVECTOR color_{ 1.f,1.f,1.f,1.f };
	size_t len[3];
	bool flag[3] = { true,true,true };
	bool next_f[3] = { false,false,false };

	bool testF = false;
	float time_ = 0.f;
	int length = 0;
	wchar_t* test;
	wchar_t* test1;
	wchar_t* test2;

	Name old = NONE;
};

