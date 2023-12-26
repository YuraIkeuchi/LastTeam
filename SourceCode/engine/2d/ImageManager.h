﻿#pragma once
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
		VIGNETTE,
		SHINE,
		PLASHINE,
		SHINE_S,
		SHINE_L,
		SMOKE,
		POWERUP,
		ENEMYHPUI,
		POIZONCOVER,
		HPNUMBERUI,
		DAMAGENUMBER,
		HEALNUMBER,
		POISONNUMBER,
		SLASH,
		MAPSCREEN,
		MAPCOMMENT00,
		MAPCOMMENT01,
		MAPCOMMENT02,
		MAPCOMMENT03,
		MAPSTART,
		MAPROAD,
		MAP_CHARA,
		MAP_FRAME,
		MAP_START,
		MAP_NORMAL,
		MAP_BOSS,
		MAP_HEAL,
		MAP_TUTORIAL,
		MAP_CHEACK,
		MAP_CHEACK_OK,
		MAP_CHEACK_NO,
		TUTORIAL_SKIPUI,
		PASSIVE_ACTIVE,
		PASSIVE_00,
		PASSIVE_01,
		PASSIVE_02,
		PASSIVE_03,
		PASSIVE_04,
		PASSIVE_05,
		PASSIVE_06,
		PASSIVE_07,
		PASSIVE_08,
		PASSIVE_09,
		PASSIVE_10,
		PASSIVE_11,
		PASSIVE_12,
		PASSIVE_13,
		PASSIVE_14,
		PASSIVE_FRAME,
		GAUGE,
		GAUGECOVER,
		HANDSCOVER,
		SKILLCANCEL,
		TITLETEXT,
		GAMEOVERBACK,
		GAMECLEARBACK,
		ONOMATO_00,
		ONOMATO_01,
		ONOMATO_02,
		ONOMATO_03,
		ONOMATO_04,
		ONOMATO_05,
		ONOMATO_06,
		ONOMATO_07,
		ONOMATO_08,
		ONOMATO_09,
		RESULTBACKSCREEN,
		RESULTNOWCHECK,
		RESULTBACKCHECK,
		RESULTATTACKAREA,
		RESULTREPORTBACK,
		RESULTREPORTATTACK,
		RESULTREPORTDEFFENCE,
		RESULTSKIP,
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
		ATTACK_10,
		ATTACK_11,
		ATTACK_12,
		ATTACK_13,
		ATTACK_14,
		ATTACK_15,
		ATTACK_16,
		ATTACK_17,
		ATTACK_18,
		ATTACK_19,
		ATTACK_20,
		ATTACK_21,
		ATTACK_22,
		ATTACK_23,
		TUTORIAL_WINDOW,
		TUTORIAL_TASK,
		TUTORIAL_CHECK,
	};
	//テクスチャの名前
	enum tex2dName
	{
		MAGIC,
		CHARGE,
		SHADOW,
		SHADOW2,
		PANNELLINE,
		AREA,
		PLAYERAREA,
		POISONAREA,
		HEALAREA,
		BUFFAREA,
		HATENAAREA,
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
		ATTACK_TEX_10,
		ATTACK_TEX_11,
		ATTACK_TEX_12,
		ATTACK_TEX_13,
		ATTACK_TEX_14,
		ATTACK_TEX_15,
		ATTACK_TEX_16,
		ATTACK_TEX_17,
		ATTACK_TEX_18,
		ATTACK_TEX_19,
		ATTACK_TEX_20,
		ATTACK_TEX_21,
		ATTACK_TEX_22,
		ATTACK_TEX_23,
		BOOM_DIR,
		POISON_EFFECT,
		HEAL_DAMAGE,
	};
	enum particleName
	{
		Normal,
		Smoke,
		Heal,
		Plus,
	};

public:
	static ImageManager* GetInstance(); //取得用
	void StartLoad2D();
	void SecondLoad2D();
	void StartLoadTex2D();
	void SecondLoadTex2D();
	void LoadParticle();
};
