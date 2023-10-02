#include "CsvLoader.h"

void LoadCSV::LoadCsvParam(std::string FileName, std::string LoadName, std::any& p)
{
	std::string line;
	std::stringstream popcom;
	std::ifstream file;
	
	file.open(FileName);

	popcom << file.rdbuf();
	
	while (std::getline(popcom, line))
	{
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}
		if (word.find(LoadName) == 0)
		{
			std::getline(line_stream, word, ',');
			p =(std::atof(word.c_str()));
			
			break;
		}
	}

	file.close();
}

std::any LoadCSV::LoadCsvParam(std::string FileName, std::string LoadName)
{
	std::string line;
	std::stringstream popcom;
	std::ifstream file;

	std::string name1 = "Resources/";
	std::string name2 = "csv/";
	std::string openFileName = name1 + name2 + FileName;

	std::any param;

	file.open(FileName);

	popcom << file.rdbuf();

	file.close();
	while (std::getline(popcom, line))
	{
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}
		if (word.find(LoadName) == 0)
		{
			std::getline(line_stream, word, ',');
			param = (std::atof(word.c_str()));

			break;
		}
	}

	return param;
}


void LoadCSV::LoadCsvParam_XMFLOAT3(std::string FileName,std::vector<XMFLOAT3>&obj, std::string LoadName)
{
	std::string line;
	std::stringstream popcom;
	std::ifstream file;

	file.open(FileName);

	popcom << file.rdbuf();

	file.close();

	std::vector<XMFLOAT3>l_obj(obj.size());
	for (int i = 0; i < obj.size(); i++) {
		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}
			if (word.find(LoadName) == 0)
			{
				std::getline(line_stream, word, ',');
				float x = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float y = static_cast<float>(std::atof(word.c_str()));

				std::getline(line_stream, word, ',');
				float z = static_cast<float>(std::atof(word.c_str()));

				l_obj[i] = {x, y, z};
				break;
			}
		}
	}
	for (int i = 0; i <obj.size(); i++) {
		obj[i] = l_obj[i];
	}
}


void LoadCSV::LoadCsvParam_String(std::string FileName, std::vector<std::string>& obj, std::string LoadName)
{
	std::string line;
	std::stringstream popcom;
	std::ifstream file;

	file.open(FileName);

	popcom << file.rdbuf();

	file.close();

	std::vector<std::string>l_obj(obj.size());
	for (int i = 0; i < obj.size(); i++) {
		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}
			if (word.find(LoadName) == 0)
			{
				std::getline(line_stream, word, ',');
				l_obj[i] = word;
				break;
			}
		}
	}
	for (int i = 0; i < obj.size(); i++) {
		obj[i] = l_obj[i];
	}
}


void LoadCSV::LoadCsvParam_Float(std::string FileName, std::vector<float>& obj, std::string LoadName) {
	std::string line;
	std::stringstream popcom;
	std::ifstream file;

	file.open(FileName);

	popcom << file.rdbuf();

	file.close();

	std::vector<float>l_obj(obj.size());
	for (int i = 0; i < obj.size(); i++) {
		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}
			if (word.find(LoadName) == 0)
			{

				std::getline(line_stream, word, ',');
				float variable = (float)std::atof(word.c_str());
				l_obj[i] = variable;
				break;
			}
		}
	}
	for (int i = 0; i < obj.size(); i++) {

		obj[i] = l_obj[i];
	}
}

void LoadCSV::LoadCsvParam_Int(std::string FileName, std::vector<int>& obj, std::string LoadName) {
	std::string line;
	std::stringstream popcom;
	std::ifstream file;

	file.open(FileName);

	popcom << file.rdbuf();

	file.close();

	std::vector<int>l_obj(obj.size());
	for (int i = 0; i < obj.size(); i++) {
		while (std::getline(popcom, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			std::getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}
			if (word.find(LoadName) == 0)
			{

				std::getline(line_stream, word, ',');
				int variable = (int)std::atof(word.c_str());
				l_obj[i] = variable;
				break;
			}
		}
	}
	for (int i = 0; i < obj.size(); i++) {

		obj[i] = l_obj[i];
	}
}