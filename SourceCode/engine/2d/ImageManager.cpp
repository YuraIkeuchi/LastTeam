﻿#include "ImageManager.h"

ImageManager* ImageManager::GetInstance()
{
	static ImageManager instans;
	return &instans;
}

//スプライトの読み込み
void ImageManager::StartLoad2D()
{
	IKESprite::LoadTexture(FEED, L"Resources/2d/effect/white1x1.png");
	IKESprite::LoadTexture(SKILLUI, L"Resources/2d/objicon/skill.png");
	IKESprite::LoadTexture(ACTIONUI, L"Resources/2d/objicon/action.png");
	IKESprite::LoadTexture(ENEMYHPUI, L"Resources/2d/objicon/BossHP.png");
	IKESprite::LoadTexture(HPNUMBERUI, L"Resources/2d/objicon/Num.png");
	IKESprite::LoadTexture(MAPSCREEN, L"Resources/2d/sceneback/mapscreen.png");
	IKESprite::LoadTexture(MAPROAD, L"Resources/2d/objicon/map_road.png");
	IKESprite::LoadTexture(MAP_CHARA, L"Resources/2d/objicon/map_chara.png");
	IKESprite::LoadTexture(MAP_FRAME, L"Resources/2d/objicon/Frame.png");
	IKESprite::LoadTexture(MAP_START, L"Resources/2d/objicon/map_00.png");
	IKESprite::LoadTexture(MAP_NORMAL, L"Resources/2d/objicon/map_01.png");
	IKESprite::LoadTexture(MAP_BOSS, L"Resources/2d/objicon/map_02.png");
	IKESprite::LoadTexture(MAP_HEAL, L"Resources/2d/objicon/map_03.png");
	IKESprite::LoadTexture(MAP_TUTORIAL, L"Resources/2d/objicon/map_04.png");
	IKESprite::LoadTexture(MAP_CHEACK, L"Resources/2d/objicon/map_cheack.png");
	IKESprite::LoadTexture(PASSIVE_01, L"Resources/2d/objicon/Passive_01.png");
	IKESprite::LoadTexture(PASSIVE_02, L"Resources/2d/objicon/Passive_02.png");
	IKESprite::LoadTexture(PASSIVE_03, L"Resources/2d/objicon/Passive_03.png");
	IKESprite::LoadTexture(PASSIVE_04, L"Resources/2d/objicon/Passive_04.png");
	IKESprite::LoadTexture(PASSIVE_05, L"Resources/2d/objicon/Passive_05.png");
	IKESprite::LoadTexture(PASSIVE_FRAME, L"Resources/2d/objicon/Passive_Frame.png");
	IKESprite::LoadTexture(GAUGE, L"Resources/2d/effect/white1x1.png");
	IKESprite::LoadTexture(TITLEBACK, L"Resources/2d/sceneback/TitleBack.png");
	IKESprite::LoadTexture(TITLETEXT, L"Resources/2d/sceneback/TitleText.png");
	IKESprite::LoadTexture(GAMEOVERBACK, L"Resources/2d/sceneback/Gameover.png");
}

//非同期処理で読み込むスプライト
void ImageManager::SecondLoad2D()
{
}

//テクスチャの読み込み
void ImageManager::StartLoadTex2D()
{
	IKETexture::LoadTexture(MAGIC, L"Resources/2d/objicon/magic.png");
	IKETexture::LoadTexture(CHARGE, L"Resources/2d/objicon/impact.png");
}

//非同期処理でのテクスチャの読み込み
void ImageManager::SecondLoadTex2D()
{
}

void ImageManager::LoadParticle()
{
	ParticleManager::LoadTexture(Normal, "effect1.png");
	ParticleManager::LoadTexture(Smoke, "hootEffect.png");
	ParticleManager::LoadTexture(Heal, "effect2.png");
}
