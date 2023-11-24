#pragma once
#include <map>
#include <string>
#include "IKESprite.h"
#include "IKETexture.h"
#include "ParticleManager.h"

//2Dの読み込み処理
class ImageManager final
{
private:
	ImageManager() = default;
	~ImageManager() = default;
	ImageManager(const ImageManager& r) = default;
	ImageManager& operator=(const ImageManager& r) = default;

public:
	//スプライトの名前
	enum texName
	{
		FEED,
		SHINE,
		POWERUP,
		ENEMYHPUI,
		HPNUMBERUI,
		DAMAGENUMBER,
		SLASH,
		MAPSCREEN,
		MAPROAD,
		MAP_CHARA,
		MAP_FRAME,
		MAP_START,
		MAP_NORMAL,
		MAP_BOSS,
		MAP_HEAL,
		MAP_TUTORIAL,
		MAP_CHEACK,
		PASSIVE_01,
		PASSIVE_02,
		PASSIVE_03,
		PASSIVE_04,
		PASSIVE_05,
		PASSIVE_06,
		PASSIVE_07,
		PASSIVE_FRAME,
		GAUGE,
		GAUGECOVER,
		HANDSCOVER,
		TITLETEXT,
		GAMEOVERBACK,
		GAMECLEARBACK,
		ONOMATO_00,
		ONOMATO_01,
		ONOMATO_02,
		ONOMATO_03,
		ONOMATO_04,
		RESULTBACKSCREEN,
		RESULTATTACKAREA,
		STAGEBACK,
		ATTACK_0,
		ATTACK_1,
		ATTACK_2,
		ATTACK_3,
		ATTACK_4,
		ATTACK_5,
		ATTACK_6,
		ATTACK_7,
		ATTACK_8,
		ATTACK_9,
		TUTORIAL_WINDOW,
	};
	//テクスチャの名前
	enum tex2dName
	{
		MAGIC,
		CHARGE,
		SHADOW,
		SHADOW2,
		AREA,
		PLAYERAREA,
		POISONAREA,
		ENEMYPREDICT,
		PLAYERPREDICT,
		ATTACK_TEX_0,
		ATTACK_TEX_1,
		ATTACK_TEX_2,
		ATTACK_TEX_3,
		ATTACK_TEX_4,
		ATTACK_TEX_5,
		ATTACK_TEX_6,
		ATTACK_TEX_7,
		ATTACK_TEX_8,
		ATTACK_TEX_9,
	};
	enum particleName
	{
		Normal,
		Smoke,
		Heal,
	};

public:
	static ImageManager* GetInstance(); //取得用
	void StartLoad2D();
	void SecondLoad2D();
	void StartLoadTex2D();
	void SecondLoadTex2D();
	void LoadParticle();
};
