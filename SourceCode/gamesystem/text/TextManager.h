#pragma once
#include"DirectXCommon.h"
#include"VariableCommon.h"
#include"Font.h"
#include <map>


class TextManager
{
private:
	TextManager() = default;
	~TextManager() = default;
	TextManager(const TextManager& r) = default;
	TextManager& operator=(const TextManager& r) = default;

public:
	enum TEXT
	{
		TITLE_01,
		TITLE_02,
		TITLE_03,
		MAP_01,
		MAP_BATTLE,
		MAP_HEAL,
		MAP_BOSS,

	};

	static TextManager* GetInstance();

	void Initialize();
	wchar_t* SearchText(TextManager::TEXT text);
private:
	void CreateWord(TextManager::TEXT text, wchar_t* word);

private:
	std::map<TextManager::TEXT, wchar_t*> wordLists;
};

