#pragma once
#include"IKESprite.h"
#include<memory>

#include"VariableCommon.h"
#include "CsvLoader.h"

using namespace std;         //  名前空間指定

class Passive {
public:
	enum class ABILITY: int{
		NONE = 0,
		RELOAD_UP,
		SPEED_UP,
		HP_UP,
		ATTACK_UP,
		MAX_ABILITY
	};
	Passive(int id, XMFLOAT2 pos = {0.f,0.f});
	~Passive();

	void Initialize();
	void Update();
	void Draw();
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
	ABILITY status = ABILITY::NONE;
	//倍率
	float diameter = 1.0f;
};

