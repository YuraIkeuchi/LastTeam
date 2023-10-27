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
		MAP_CHARA,
		MAP_FRAME,
		MAP_START,
		MAP_NORMAL,
		MAP_HEAL,
		MAP_BOSS,
		PASSIVE_01,
		PASSIVE_02,
		PASSIVE_03,
		GAUGE,
	};
	//テクスチャの名前
	enum tex2dName
	{
		MAGIC,
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
