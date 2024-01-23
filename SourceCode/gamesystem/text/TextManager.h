#pragma once
#include"DirectXCommon.h"
#include"VariableCommon.h"
#include"Font.h"
#include <map>
using namespace std;         //  名前空間指定
enum LoadType {
	LOAD_TUTORIAL,
	LOAD_PASSIVE,
	LOAD_ATTACK,
};
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
		TUTORIAL_START,
		TUTORIAL_TASK,
		TUTORIAL_COUNTER,
		TUTORIAL_SKILL,
		TUTORIAL_CHOICE,
		TUTORIAL_END,
		RESULT,
		EXTRA,
	};

	enum PosType {
		FIRST_POS = 0,
		SECOND_POS,
		THIRD_POS,
		ALL_POS,
	};

	//
	void Create(DirectXCommon* dxcomon);

	void Initialize(DirectXCommon* dxcomon,const int LoadType);

	void Draw(DirectXCommon* dxcommon);

	void TestDraw(DirectXCommon* dxcommon);

	void Test();

	void SetAllColor(const XMVECTOR& color = { 1.f,1.f,1.f,1.f });

	void SetOnceColor(int row, const XMVECTOR& color = { 1.f,1.f,1.f,1.f });

	void SetConversation(Name name = NONE, const XMFLOAT2& pos = { 5.f,0.f }, const XMVECTOR& color = { 1.f,1.f,1.f,1.f });

	void NoneText();

	void GetWordSize(Word word);

	//void SetRowPosition(float posX);

	void SetCreateSentence(wchar_t* tex1, wchar_t* tex2 = L" ", wchar_t* tex3 = L" ");

	wchar_t* GetPasiveSentence(int ID) { return passiveSentence[ID]; };
	wchar_t* GetPassiveName(int ID) { return passiveName[ID]; };

	wchar_t* GetSkillSentence(int ID) { return skillSentence[ID]; };
	wchar_t* GetSkillDamage(int ID) { return skillDamage[ID]; };

private:

	//
	void CreateWord(Name name, wchar_t* tex1, wchar_t* tex2 = L" ", wchar_t* tex3 = L" ");
	//
	Word SetWord(wchar_t* tex1, wchar_t* tex2, wchar_t* tex3);

	void CreateCon(Conversation con, Word word);

	void CreatePassiveSentence(wchar_t* tex1);
	void CreatePassiveName(wchar_t* tex1);

	void CreateSkillSentence(wchar_t* tex1);
	void CreateSkillDamage(wchar_t* tex1);

private:
	std::map<TextManager::Name, Word> wordlist_;

	std::vector<wchar_t*> passiveSentence;
	std::vector<wchar_t*> passiveName;
	std::vector<wchar_t*> skillSentence;
	std::vector<wchar_t*> skillDamage;

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

