#include "Passive.h"

Passive::Passive(int id) {
}

Passive::~Passive() {
}

bool Passive::CreatePassive(int id) {

	string directory = "Resources/csv/chara/player/Passive";

	std::stringstream ss;
	if (id > 10) {
		ss << directory << id << ".csv";
	} else {
		ss << directory << "0" << id << ".csv";
	}
	std::string csv_ = ss.str();
	spriteNum = std::any_cast<int>(LoadCSV::LoadCsvParam(csv_, "icon"));

	return true;
}
