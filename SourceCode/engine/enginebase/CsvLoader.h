#pragma once
#include <any>
#include <string>

#include<fstream>
#include<string>
#include<sstream>
#include <vector>

#include "DirectXMath.h"
using namespace DirectX;
namespace LoadCSV
{
	//CSV読み込み
	//間接的にするやつ　この関数内で例外処理とかかける
	void LoadCsvParam(std::string FileName,std::string LoadName,std::any&p );

	//直接的にするやつ　コードが短くすむ・格納用の変数も必要ない
	std::any LoadCsvParam(std::string FileName, std::string LoadName);

	void LoadCsvParam_XMFLOAT3(std::string FileName, std::vector<XMFLOAT3>&obj,std::string LoadName);
	void LoadCsvParam_String(std::string FileName, std::vector<std::string>& obj, std::string LoadName);
	void LoadCsvParam_Float(std::string FileName, std::vector<float>& obj, std::string LoadName);
	void LoadCsvParam_Int(std::string FileName, std::vector<int>& obj, std::string LoadName);
}


