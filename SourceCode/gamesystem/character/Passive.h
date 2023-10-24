#pragma once
#include"IKESprite.h"
#include<memory>

#include <string>
#include "CsvLoader.h"
#include <sstream>

using namespace std;         //  名前空間指定

class Passive {
public:
	enum STATUS {
		NONE = 0,
		HP_UP,
		HP_DOWN,
		ATTACK_UP,
	};
	Passive(int id);
	~Passive();

private:

	bool CreatePassive(int id);


private:

	int id = 0;
	std::unique_ptr<IKESprite> icon = nullptr;
	int spriteNum = 0;
	int status = NONE;
	//倍率
	float diameter = 1.0f;
};

