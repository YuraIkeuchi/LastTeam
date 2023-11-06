#pragma once
#include "SkillBase.h"
#include <vector>

class AttackSkill : public SkillBase
{
public:
	void ImGui_Origin()override;
	float GetDamege() { return damege; }
	std::vector<std::vector<int>> GetArea() { return area; }
	int GetDistanceX() { return distanceX; }
	int GetDistanceY() { return distanceY; }

	void SetDamege(const float damege) { this->damege = damege; }
	void SetArea(std::vector<std::vector<int>> area) { this->area = area; }
	//”O‚Ì‚½‚ß
	void SetDistanceX(const int distanceX) { this->distanceX = distanceX; }
	void SetDistanceY(const int distanceY) { this->distanceY = distanceY; }
protected:
	float damege = 0.0f;

	//ƒXƒLƒ‹ŒÅ—LŠ„‡
	float skillRation = 0.0f;
	
	// deta = 000000000000000;
	
	//‹zûUŒ‚
	bool adsorption = false;
	//©UŒ‚
	bool selfDamege = false;
	int distanceX = 0;
	int distanceY = 0;
	std::vector<std::vector<int>> area;
};

