#include "ImageManager.h"

ImageManager* ImageManager::GetInstance()
{
	static ImageManager instans;
	return &instans;
}

//スプライトの読み込み
void ImageManager::StartLoad2D()
{
	IKESprite::LoadTexture(SKILLUI, L"Resources/2d/objicon/skill.png");
	IKESprite::LoadTexture(ACTIONUI, L"Resources/2d/objicon/action.png");
	IKESprite::LoadTexture(ENEMYHPUI, L"Resources/2d/objicon/BossHP.png");
	IKESprite::LoadTexture(HPNUMBERUI, L"Resources/2d/objicon/Num.png");
	IKESprite::LoadTexture(MAPSCREEN, L"Resources/2d/sceneback/mapscreen.jpg");
	IKESprite::LoadTexture(MAP_START, L"Resources/2d/objicon/map_00.png");
	IKESprite::LoadTexture(MAP_NORMAL, L"Resources/2d/objicon/map_01.png");
	IKESprite::LoadTexture(MAP_HEAL, L"Resources/2d/objicon/map_03.png");
	IKESprite::LoadTexture(MAP_BOSS, L"Resources/2d/objicon/map_02.png");
	IKESprite::LoadTexture(GAUGE, L"Resources/2d/effect/white1x1.png");

}

//非同期処理で読み込むスプライト
void ImageManager::SecondLoad2D()
{
}

//テクスチャの読み込み
void ImageManager::StartLoadTex2D()
{
	IKETexture::LoadTexture(MAGIC, L"Resources/2d/objicon/magic.png");

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
