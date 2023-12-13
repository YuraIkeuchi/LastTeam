#pragma once
#include"IKESprite.h"
#include"DrawNumber.h"
#include <vector>
#include <Feed.h>
class ResultReport {
public:
	ResultReport();
	~ResultReport();

	void Initialize();
	void Update();
	void Draw(DirectXCommon* dxCommon);

	void SetScore(int score) { this->score = score; }

	void SetDealtDamage(int damage) { dealtDamage = damage; }
	void SetTakenDamage(int damage) { takenDamage = damage; }

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

	void SmokeInit(XMFLOAT2 pos);
	void SmokeUpdate();
	void DamageIntNum(int num,vector<int>& nums);
private:
	struct SmokeEffect {
		std::unique_ptr<IKESprite> tex = nullptr;
		XMFLOAT2 pos = { };
		XMFLOAT2 size = { 64.f,64.f };
		float frame = 0.f;
		float kFrameMax = 45.f;
		bool isVanish = false;
	};
	struct ShineEffect {
		unique_ptr<IKESprite> tex;
		float frame = 0.f;
		float frameA = 0.f;
		float kFrame = 20.f;
		float angle = 0.f;
		float dia = 0.f;
		XMFLOAT2 position = { 0.f,0.f };
		XMFLOAT2 size = { 32.f,32.f };
		bool isVanish = false;
	};
private:
	std::unique_ptr<IKESprite> backScreen = nullptr;
	std::unique_ptr<IKESprite> rate = nullptr;
	std::unique_ptr<IKESprite> skip = nullptr;
	std::list<ShineEffect> shines;
	
	int score = 0;
	int counter = 0;
	bool isFinish = false;
	std::list<SmokeEffect> smokes;
	bool isFirstNum = false;
	bool isSecondNum = false;
	std::unique_ptr<Feed> feed;
	bool m_Feed = false;
	bool m_FeedStart = false;
	bool m_FeedEnd = false;

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

	int dealtDamage = 0;
	int takenDamage = 0;

	std::vector<int> dealNum = {};
	std::vector<int> takeNum = {};

	XMFLOAT2 margin[4] = { XMFLOAT2(-100.f,-30.f),
						   XMFLOAT2(-50.f,0.f),
						   XMFLOAT2(50.f,0.f),
						   XMFLOAT2(100.f,-30.f)};
	float skip_alpha = 0.f;
};