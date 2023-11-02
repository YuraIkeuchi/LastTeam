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
	friend class Singleton<SkillBuildToul>; // �C���X�^���X�쐬������


	void BuildToul();

private:

	TestType type = TestType::kNone;

	//�ҋ@����
	int latency_ = 0;
	//��������
	float invocating_time_ = 0.0f;
	//�S������
	float restraint_time_ = 0.0f;
	//���A���e�B
	int rarity_ = 1;
	//�o�������N
	int pop_Rate_ = 1;

};