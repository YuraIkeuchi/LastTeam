#pragma once
#include"IKESprite.h"
#include"DrawNumber.h"
#include <vector>
class ResultReport {
public:
	ResultReport();
	~ResultReport();

	void Initialize();
	void Update();
	void Draw(DirectXCommon* dxCommon);

	void SetIsFinish(bool flag) { isFinish = flag; }
	bool GetIsFinish() { return isFinish; }
private:
	enum State {
		INIT,
		SCORE,
		STAMP,
		FINISH,
	}state=INIT;
	static void(ResultReport::* stateTable[])();

	void InitUpdate();
	void ScoreUpdate();
	void StampUpdate();
	void FinishUpdate();

	void DamageIntNum(int num,vector<int>& nums);
private:
	std::unique_ptr<IKESprite> backScreen = nullptr;
	std::unique_ptr<IKESprite> rate = nullptr;
	std::unique_ptr<IKESprite> skip = nullptr;

	bool isFinish = false;

	static const int DAMAGEMAX = 3;
	std::unique_ptr<DrawNumber> damage_dealt[DAMAGEMAX];//ó^É_ÉÅ
	std::unique_ptr<DrawNumber> damage_taken[DAMAGEMAX];//îÌÉ_ÉÅ

	float frameInit = 0.f;
	float kFrameInitMax = 5.f;

	int count = 10;
	float numFrames[2] = {};
	float kFrameScoreMax = 60.f;
	float frameStamp = 0.f;
	float kFrameStampMax = 60.0f;

	int dealtDamage = 130;
	int takenDamage = 8;

	std::vector<int> dealNum = {};
	std::vector<int> takeNum = {};

	float skip_alpha = 0.f;
};