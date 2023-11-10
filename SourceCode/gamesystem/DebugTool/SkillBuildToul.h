#pragma once

#include "Singleton/Singleton.h"

enum class TestType
{
	kNone = -1,
	kBase,
	kAttack,
};

class SkillBuildToul : 
	public Singleton<SkillBuildToul>
{
public:
	friend class Singleton<SkillBuildToul>; // インスタンス作成を許可


	void BuildToul();

private:

	TestType type = TestType::kNone;

	//待機時間
	int latency_ = 0;
	//発動時間
	float invocating_time_ = 0.0f;
	//拘束時間
	float restraint_time_ = 0.0f;
	//レアリティ
	int rarity_ = 1;
	//出現ランク
	int pop_Rate_ = 1;

};