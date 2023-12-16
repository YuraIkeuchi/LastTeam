#include "Passive.h"
#include "ImageManager.h"
Passive::Passive(int id, XMFLOAT2 pos, XMFLOAT2 size) {
	this->pos = pos;
	this->size = size;
	CreatePassive(id);

	Initialize();
}

Passive::~Passive() {
}

void Passive::Initialize() {
	icon = IKESprite::Create(ImageManager::PASSIVE_00 + spriteNum, pos);
	icon->SetSize(size);

}

void Passive::Draw() {
	IKESprite::PreDraw();
	icon->Draw();
	IKESprite::PostDraw();
}

void Passive::LoadCsvPassive(std::string& FileName) {
	std::ifstream file;
	std::stringstream popcom;

	file.open(FileName);
	popcom << file.rdbuf();
	file.close();

	std::string line;
	while (std::getline(popcom, line)) {
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}else if (word.find("ID") == 0) {
			std::getline(line_stream, word, ',');
			id = std::stoi(word);
		} else if (word.find("ICON") == 0) {
			std::getline(line_stream, word, ',');
			spriteNum = std::stoi(word);
		} else if (word.find("ABILITY") == 0) {
			std::getline(line_stream, word, ',');
			ability = ABILITY(std::stoi(word));
		} else if (word.find("DIAMETER") == 0) {
			std::getline(line_stream, word, ',');
			diameter = std::stof(word);
			break;
		}
	}
}

bool Passive::CreatePassive(int id) {

	string directory = "Resources/csv/chara/player/passive/Passive";

	std::stringstream ss;
	if (id >= 10) {
		ss << directory << id << ".csv";
	} else {
		ss << directory << "0" << id << ".csv";
	}
	std::string csv_ = ss.str();
	LoadCsvPassive(csv_);
	return true;
}
