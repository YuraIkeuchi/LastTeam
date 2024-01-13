#pragma once
#include"IKESprite.h"
#include<memory>
#include<list>
#include "VariableCommon.h"


enum OnomatoPattern {
	Foot,
	BossSpawn,
	GameOver,
	AttackCharge,
	Counter,
	Attack01,
	Attack02,
	Attack03,
	Refrain,
	Guard
};
class Onomatope {
public:
	Onomatope();
	~Onomatope();

	void Update();

	void Draw();

	void AddOnomato(OnomatoPattern patten, XMFLOAT2 basePos, float delay = 1.f);
private:
	struct OnomatoStruct {
		std::unique_ptr<IKESprite> Tex;
		bool isFinish = false;
		XMFLOAT2 basePos = { 0.f,0.f };
		float frame = 0.0f;
		float kFrameMax = 10.f;
		int pattern = -1;
		float delayFrame = 0.f;
		float kDelayFrameMax = 1.f;
		float vanishFrame = 0.f;
		float kVanishFrameMax = 20.0f;
	};
	//ä÷êîÉ|ÉCÉìÉ^
	static void(Onomatope::* updateTable[])(OnomatoStruct& onomato);

	void FootUpdate(OnomatoStruct& onomato);
	void BurnUpdate(OnomatoStruct& onomato);
	void BossSpawnUpdate(OnomatoStruct& onomato);
	void GameOverUpdate(OnomatoStruct& onomato);
	void AttackChargeUpdate(OnomatoStruct& onomato);
	void CounterUpdate(OnomatoStruct& onomato);
	void LittleBurnUpdate(OnomatoStruct& onomato);
	void StoneUpdate(OnomatoStruct& onomato);
	void RefrainUpdate(OnomatoStruct& onomato);
	void GuardUpdate(OnomatoStruct& onomato);
private:
	std::list<OnomatoStruct> onomatoList;
	std::unique_ptr<IKESprite> Huu;

};