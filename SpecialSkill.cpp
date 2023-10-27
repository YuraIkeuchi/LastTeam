#include "SpecialSkill.h"

float SpecialSkill::HPAbsorption(const float damege, const float ratio)
{
	return damege * ratio;
}

float SpecialSkill::SelfScratch(const float damege, const float ratio)
{
	return damege * ratio;
}
