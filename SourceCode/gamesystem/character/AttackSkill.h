#pragma once
#include "SkillBase.h"
class AttackSkill : public SkillBase
{
public:
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="ID">ID</param>
	/// <param name="latency">待機時間</param>
	/// <param name="invocatingTime">発動時間</param>
	/// <param name="restraintTime">拘束時間</param>
	/// <param name="rarity">レアリティ―</param>
	/// <param name="popRate">出現ランク</param>
	/// <param name="damege">ダメージ</param>
	/// <param name="range">範囲</param>
	/// <param name="distance">距離</param>
	void Create(
		const std::string name,
		int ID,
		float latency = 0.0f,
		float invocatingTime = 0.0f,
		float restraintTime = 0.0f,
		int rarity = 1,
		int popRate = 1,
		float damege = 0.0f,
		XMFLOAT2 range = { 1.0f,1.0f },
		XMFLOAT2 distance = { 1.0f, 0.0f });

	void ImGui_Origin()override;


	XMFLOAT2 GetDistance() { return distance; }
	XMFLOAT2 GetRange() { return range; }


protected:
	
	//範囲
	XMFLOAT2 range = { 1.0f,1.0f };
	//距離 ※プレイヤーの位置から範囲の一番左上基準
	XMFLOAT2 distance = { 0.0f, 1.0f };

	//スキル固有割合
	float skillRation = 0.0f;
	
	// deta = 000000000000000;
	
	//吸収攻撃
	bool adsorption = false;
	//自傷攻撃
	bool selfDamege = false;
};

