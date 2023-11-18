#include "ImageManager.h"

ImageManager* ImageManager::GetInstance()
{
	static ImageManager instans;
	return &instans;
}

//スプライトの読み込み
void ImageManager::StartLoad2D()
{
	IKESprite::LoadTexture(FEED, L"Resources/2d/effect/white1x1.png");
	IKESprite::LoadTexture(SHINE, L"Resources/2d/effect/shine.png");
	IKESprite::LoadTexture(SKILLUI, L"Resources/2d/objicon/skill.png");
	IKESprite::LoadTexture(ENEMYHPUI, L"Resources/2d/objicon/BossHP.png");
	IKESprite::LoadTexture(HPNUMBERUI, L"Resources/2d/objicon/Num.png");
	IKESprite::LoadTexture(SLASH, L"Resources/2d/objicon/slash.png");
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
	IKESprite::LoadTexture(PASSIVE_01, L"Resources/2d/objicon/Passive/Passive_01.png");
	IKESprite::LoadTexture(PASSIVE_02, L"Resources/2d/objicon/Passive/Passive_02.png");
	IKESprite::LoadTexture(PASSIVE_03, L"Resources/2d/objicon/Passive/Passive_03.png");
	IKESprite::LoadTexture(PASSIVE_04, L"Resources/2d/objicon/Passive/Passive_04.png");
	IKESprite::LoadTexture(PASSIVE_05, L"Resources/2d/objicon/Passive/Passive_05.png");
	IKESprite::LoadTexture(PASSIVE_06, L"Resources/2d/objicon/Passive/Passive_06.png");
	IKESprite::LoadTexture(PASSIVE_07, L"Resources/2d/objicon/Passive/Passive_07.png");
	IKESprite::LoadTexture(PASSIVE_FRAME, L"Resources/2d/objicon/Passive_Frame.png");
	IKESprite::LoadTexture(GAUGE, L"Resources/2d/effect/white1x1.png");
	IKESprite::LoadTexture(GAUGECOVER, L"Resources/2d/objicon/SkillGauge.png");
	IKESprite::LoadTexture(HANDSCOVER, L"Resources/2d/objicon/HandsCover.png");
	IKESprite::LoadTexture(TITLETEXT, L"Resources/2d/sceneback/TitleText.png");
	IKESprite::LoadTexture(GAMEOVERBACK, L"Resources/2d/sceneback/Gameover.png");
	IKESprite::LoadTexture(GAMECLEARBACK, L"Resources/2d/sceneback/GameClear.png");
	IKESprite::LoadTexture(ONOMATO_00, L"Resources/2d/objicon/Onomatope/00_Foot.png");
	IKESprite::LoadTexture(ONOMATO_01, L"Resources/2d/objicon/Onomatope/01_Attack01.png");

	IKESprite::LoadTexture(RESULTBACKSCREEN, L"Resources/2d/sceneback/ResultSkill_BS.png");
	IKESprite::LoadTexture(RESULTATTACKAREA, L"Resources/2d/objicon/AttackArea.png");
	IKESprite::LoadTexture(ATTACK_0, L"Resources/2d/objicon/AttackSKill/Skill0.png");
	IKESprite::LoadTexture(ATTACK_1, L"Resources/2d/objicon/AttackSKill/Skill1.png");
	IKESprite::LoadTexture(ATTACK_2, L"Resources/2d/objicon/AttackSKill/Skill2.png");
	IKESprite::LoadTexture(ATTACK_3, L"Resources/2d/objicon/AttackSKill/Skill3.png");
	IKESprite::LoadTexture(ATTACK_4, L"Resources/2d/objicon/AttackSKill/Skill4.png");
	IKESprite::LoadTexture(ATTACK_5, L"Resources/2d/objicon/AttackSKill/Skill5.png");
	IKESprite::LoadTexture(ATTACK_6, L"Resources/2d/objicon/AttackSKill/Skill6.png");
	IKESprite::LoadTexture(ATTACK_7, L"Resources/2d/objicon/AttackSKill/Skill7.png");
	IKESprite::LoadTexture(ATTACK_8, L"Resources/2d/objicon/AttackSKill/Skill8.png");
	IKESprite::LoadTexture(ATTACK_9, L"Resources/2d/objicon/AttackSKill/Skill9.png");
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
	IKETexture::LoadTexture(SHADOW, L"Resources/2d/objicon/shadow.png");
	IKETexture::LoadTexture(SHADOW2, L"Resources/2d/objicon/shadow2.png");
	IKETexture::LoadTexture(AREA, L"Resources/2d/objicon/DamageArea.png"); 
	IKETexture::LoadTexture(PLAYERAREA, L"Resources/2d/objicon/PlayerAttack.png");
	IKETexture::LoadTexture(POISONAREA, L"Resources/2d/objicon/PoizonAttack.png");
	IKETexture::LoadTexture(ENEMYPREDICT, L"Resources/2d/objicon/Predict.png");
	IKETexture::LoadTexture(PLAYERPREDICT, L"Resources/2d/objicon/PlayerPredict.png");
	IKETexture::LoadTexture(ATTACK_TEX_0, L"Resources/2d/objicon/AttackSKill/Skill0.png");
	IKETexture::LoadTexture(ATTACK_TEX_1, L"Resources/2d/objicon/AttackSKill/Skill1.png");
	IKETexture::LoadTexture(ATTACK_TEX_2, L"Resources/2d/objicon/AttackSKill/Skill2.png");
	IKETexture::LoadTexture(ATTACK_TEX_3, L"Resources/2d/objicon/AttackSKill/Skill3.png");
	IKETexture::LoadTexture(ATTACK_TEX_4, L"Resources/2d/objicon/AttackSKill/Skill4.png");
	IKETexture::LoadTexture(ATTACK_TEX_5, L"Resources/2d/objicon/AttackSKill/Skill5.png");
	IKETexture::LoadTexture(ATTACK_TEX_6, L"Resources/2d/objicon/AttackSKill/Skill6.png");
	IKETexture::LoadTexture(ATTACK_TEX_7, L"Resources/2d/objicon/AttackSKill/Skill7.png");
	IKETexture::LoadTexture(ATTACK_TEX_8, L"Resources/2d/objicon/AttackSKill/Skill8.png");
	IKETexture::LoadTexture(ATTACK_TEX_9, L"Resources/2d/objicon/AttackSKill/Skill9.png");
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
