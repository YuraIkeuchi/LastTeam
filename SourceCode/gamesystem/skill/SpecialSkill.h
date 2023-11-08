#pragma once
#include "SkillBase.h"
#include <vector>
#include <string>

class SpecialSkill : public SkillBase
{
public:
	void ImGui_Origin()override;
	std::string GetStateName() { return StateName; }

	void SetStateName(const std::string name) { this->StateName = name; }
protected:
	std::string StateName = "NONE";
};

