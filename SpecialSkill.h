#pragma once
class SpecialSkill
{

public:
	/// <summary>
	/// HP吸収攻撃
	/// </summary>
	/// <param name="damege">ダメージ</param>
	/// <param name="ratio">吸収割合</param>
	/// <returns>吸収量</returns>
	float HPAbsorption(const float damege, const float ratio);
	
	/// <summary>
	/// 自傷ダメージ
	/// </summary>
	/// <param name="damege">ダメージ</param>
	/// <param name="ratio">自傷割合</param>
	/// <returns>自傷量</returns>
	float SelfScratch(const float damege, const float ratio);



};

