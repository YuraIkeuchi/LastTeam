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

private:
	void RandSkill();


private:
	bool isVisible = false;
	bool isStart = false;
	std::unique_ptr<IKESprite> backScreen;
	std::list<std::unique_ptr<ActionUI>> choiceSkills;
	std::list<std::unique_ptr<Passive>> choicePassives;

	std::vector<int> GetPassives;

};

