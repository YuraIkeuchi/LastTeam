#pragma once
#include "SkillBase.h"
#include <vector>
#include <string>

class AttackSkill : public SkillBase
{
public:
	void ImGui_Origin()override;
	float GetDamege() { return damege; }
	std::vector<std::vector<int>> GetArea() { return area; }
	std::vector<std::vector<int>> GetTimer() { return timer; }
	const int GetDistanceX() { return distanceX; }
	const int GetDistanceY() { return distanceY; }
	std::string GetStateName() { return StateName; }
	const int GetPoisonToken() { return m_PoisonToken; }

	void SetDamege(const float damege) { this->damege = damege; }
	void SetArea(std::vector<std::vector<int>> area) { this->area = area; }
	void SetTimer(std::vector<std::vector<int>> timer) { this->timer = timer; }
	//”O‚Ì‚½‚ß
	void SetDistanceX(const int distanceX) { this->distanceX = distanceX; }
	void SetDistanceY(const int distanceY) { this->distanceY = distanceY; }
	void SetStateName(const std::string name) { this->StateName = name; }
	void SetPoisonToken(const int token) { this->m_PoisonToken = token; }
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
	int m_PoisonToken = {};

	std::string StateName = "NONE";
	std::vector<std::vector<int>> area;
	std::vector < std::vector<int>> timer;
};

