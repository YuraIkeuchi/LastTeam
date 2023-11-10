#include "SkillBase.h"
#include "imgui.h"
void SkillBase::Create(const std::string name, int ID, int latency, int invocatingTime, int restraintTime, int rarity, int popRate)
{
	this->name = name;
	this->ID = ID;
	this->latency = latency;
	this->invocatingTime = invocatingTime;
	this->restraintTime = restraintTime;
	this->rarity = rarity;
	this->popRate = popRate;
}

void SkillBase::ImGuiDraw() {
	ImGui_Origin();
}

