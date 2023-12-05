#pragma once
#include"IKESprite.h"
#include"DrawNumber.h"

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
	std::unique_ptr<IKESprite> backScreen = nullptr;
	std::unique_ptr<IKESprite> rate = nullptr;
	bool isFinish = false;

	static const int DAMAGEMAX = 3;
	std::unique_ptr<DrawNumber> damage_dealt[DAMAGEMAX];//ó^É_ÉÅ
	std::unique_ptr<DrawNumber> damage_taken[DAMAGEMAX];//îÌÉ_ÉÅ

	float numFrames[2] = {};
	float frame = 0.f;
	float kFrameMax = 30.0f;

};