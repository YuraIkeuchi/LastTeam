#include "SkillBase.h"

void SkillBase::Create(const std::string name, int ID, float latency, float invocatingTime, float restraintTime, int rarity, int popRate)
{
	this->name = name;
	this->ID = ID;
	this->latency = latency;
	this->invocatingTime = invocatingTime;
	this->restraintTime = restraintTime;
	this->rarity = rarity;
	this->popRate = popRate; 
}
