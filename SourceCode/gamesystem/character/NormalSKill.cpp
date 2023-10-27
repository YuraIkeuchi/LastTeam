#include "NormalSkill.h"
#include "imgui.h"
void NormalSkill::Create(const std::string name, int ID, int latency, int invocatingTime, int restraintTime, int rarity, int popRate, float damege, XMFLOAT2 range, XMFLOAT2 distance)
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

void NormalSkill::ImGui_Origin() {
	ImGui::Begin("Normal");
	ImGui::Text("ID:%d", ID);
	ImGui::Text("Birth:%d", m_Birth);
	ImGui::End();
}