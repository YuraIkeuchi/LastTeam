#pragma once
#include"IKESprite.h"
#include <vector>
#include <memory>
#include <ActionUI.h>
#include <Passive.h>
#include "DrawNumber.h"

class ResultSkill {
public:
	ResultSkill();
	~ResultSkill();

	void Initialize();
	void Update();
	void Draw();


	void CreateResult(std::vector<int>& notDeck,
		std::vector<int>& notPassives);


	bool GetIsVisible() { return isStart; }
private:

	struct ResultUI{
		std::unique_ptr<IKESprite> icon;
		std::unique_ptr<DrawNumber> number;
		XMFLOAT2 position = { 640.f,320.f };
		XMFLOAT2 size = { 128.f,128.f };
		int ID = -1;
		bool isSkill = true;


	};
private:
	bool isStart = false;


	std::unique_ptr<IKESprite> backScreen;
	std::list<ResultUI> choiceSkills;

	XMFLOAT2 BasePos[2] = { {320.f,360.0f},{960.f,360.f} };
};

