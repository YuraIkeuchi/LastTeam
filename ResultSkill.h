#pragma once
#include"IKESprite.h"
#include <vector>
#include <memory>
#include <ActionUI.h>
#include <Passive.h>
class ResultSkill {
public:
	ResultSkill();
	~ResultSkill();

	void Initialize();
	void Update();
	void Draw();


	void GetNotDeckNumber(std::vector<int>& notDeck);
	void SetPassiveId(std::vector<int>& gotPassives);


	void SetIsVisible(const bool isVisible) {
		this->isVisible = isVisible;
		isStart = true;
	}
	bool GetIsVisible() { return isVisible; }

private:
	void RandSkill();


private:
	bool isVisible = false;
	bool isStart = true;
	std::unique_ptr<IKESprite> backScreen;
	std::list<std::unique_ptr<ActionUI>> choiceSkills;
	std::list<std::unique_ptr<Passive>> choicePassives;

	std::vector<int> GetPassives;
	XMFLOAT2 BasePos[2] = { {320.f,360.0f},{960.f,360.f} };
};

