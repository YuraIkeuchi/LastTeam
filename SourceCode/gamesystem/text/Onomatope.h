#pragma once
#include"IKESprite.h"
#include<memory>
#include<list>
#include "VariableCommon.h"


enum OnomatoPattern {
	Foot,
	Attack01,
	BossSpawn,
	GameOver,
	AttackCharge,
	

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
	};
	//ä÷êîÉ|ÉCÉìÉ^
	static void(Onomatope::* updateTable[])(OnomatoStruct& onomato);

	void FootUpdate(OnomatoStruct& onomato);
	void BurnUpdate(OnomatoStruct& onomato);
	void BossSpawnUpdate(OnomatoStruct& onomato);
	void GameOverUpdate(OnomatoStruct& onomato);
	void AttackChargeUpdate(OnomatoStruct& onomato);

private:
	std::list<OnomatoStruct> onomatoList;

};