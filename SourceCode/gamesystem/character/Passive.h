#pragma once
#include"IKESprite.h"
#include<memory>

#include"VariableCommon.h"
#include "CsvLoader.h"

using namespace std;         //  –¼‘O‹óŠÔŽw’è

class Passive {
public:
	enum class ABILITY: int{
		NONE = 0,
		RELOAD_UP,
		HP_UP,
		RELOAD_LOCK,
		MAX_ABILITY
	};
	Passive(int id, XMFLOAT2 pos = {0.f,0.f});
	~Passive();

	void Initialize();
	void Update();
	void Draw();

	float GetDiameter() { return diameter; }
	ABILITY GetAbility() { return ability; }
private:
	void LoadCsvPassive(std::string& FileName);

	bool CreatePassive(int id);
private:
	int id = 0;
	std::unique_ptr<IKESprite> icon = nullptr;
	int spriteNum = 0;
	//
	XMFLOAT2 pos = {};
	//
	ABILITY ability = ABILITY::NONE;
	//”{—¦
	float diameter = 1.0f;
};

