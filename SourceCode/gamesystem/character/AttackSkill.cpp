#include "AttackSkill.h"
#include "imgui.h"
void AttackSkill::Create(const std::string name, int ID, int latency, int invocatingTime, int restraintTime, int rarity, int popRate, float damege, XMFLOAT2 range, XMFLOAT2 distance)
{
	this->name = name;
	this->ID = ID;
	this->latency = latency;
	this->invocatingTime = invocatingTime;
	this->restraintTime = restraintTime;
	this->rarity = rarity;
	this->popRate = popRate;
	this->damege = damege;
	this->range = range;
	this->distance = distance;
}

void AttackSkill::ImGui_Origin() {
	ImGui::Begin("Attack");
	ImGui::Text("ID:%d", ID);
	ImGui::Text("Damage:%f", damege);
	ImGui::End();
}