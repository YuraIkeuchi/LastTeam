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
		SKILLUI,
		ACTIONUI,
		ENEMYHPUI,
		HPNUMBERUI,
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
		TITLEBACK,
		TITLETEXT,
		GAMEOVERBACK,
		GAMECLEARBACK,
		ONOMATO_00,
		RESULTBACKSCREEN,
		RESULTATTACKAREA,
	};
	//テクスチャの名前
	enum tex2dName
	{
		MAGIC,
		CHARGE,
		SHADOW,
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
