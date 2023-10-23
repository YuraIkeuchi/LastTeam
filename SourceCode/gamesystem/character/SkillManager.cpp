#include "SkillManager.h"

SkillManager* SkillManager::GetInstance()
{
	static SkillManager instance;

	return &instance;
}

void SkillManager::Initialize()
{
	skillA.Create(nameA, 1);
	skillB.Create(nameB, 2, 0.0f, 0.0f, 0.0f, 1, 1);
	skillC.Create(nameC, 3, 0.0f, 0.0f, 0.0f, 1, 1);
}
