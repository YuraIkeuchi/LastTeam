#pragma once
#include "DirectXMath.h"
#include <string>

class SkillBase
{
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="ID">ID</param>
	/// <param name="latency">待機時間</param>
	/// <param name="invocatingTime">発動時間</param>
	/// <param name="restraintTime">拘束時間</param>
	/// <param name="rarity">レアリティ</param>
	/// <param name="popRate">出現ランク</param>
	void Create(const std::string name, int ID, float latency = 0.0f, float invocatingTime = 0.0f, float restraintTime = 0.0f, int rarity = 1, int popRate = 1);

protected:
	//スキル名
	std::string name = "NoName";
	//ID
	int ID = 0;
	//アイコン
	
	//待機時間
	float latency = 0.0f;
	//発動時間
	float invocatingTime = 0.0f;
	//拘束時間
	float restraintTime = 0.0f;
	//レアリティ
	int rarity = 1;
	//出現ランク
	int popRate = 1;
};

