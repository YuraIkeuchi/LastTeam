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
	IKESprite::LoadTexture(PLASHINE, L"Resources/2d/effect/effect3.png");
	IKESprite::LoadTexture(SHINE_S, L"Resources/2d/effect/effect4.png");
	IKESprite::LoadTexture(SHINE_L, L"Resources/2d/effect/effect5.png");
	IKESprite::LoadTexture(SMOKE, L"Resources/2d/effect/smoke.png");
	IKESprite::LoadTexture(POWERUP, L"Resources/2d/effect/Up.png");
	IKESprite::LoadTexture(ENEMYHPUI, L"Resources/2d/objicon/BossHP.png");
	IKESprite::LoadTexture(POIZONCOVER, L"Resources/2d/objicon/Numbers/PoisonFrame.png");
	IKESprite::LoadTexture(HPNUMBERUI, L"Resources/2d/objicon/Numbers/Num.png");
	IKESprite::LoadTexture(POISONNUMBER, L"Resources/2d/objicon/Numbers/Num_Poison.png");
	IKESprite::LoadTexture(DAMAGENUMBER, L"Resources/2d/objicon/Numbers/Num_Damage.png");
	IKESprite::LoadTexture(HEALNUMBER, L"Resources/2d/objicon/Numbers/Num_Heal.png");
	IKESprite::LoadTexture(SLASH, L"Resources/2d/objicon/Numbers/slash.png");
	IKESprite::LoadTexture(MAPSCREEN, L"Resources/2d/sceneback/mapscreen.png");
	IKESprite::LoadTexture(MAPCOMMENT00, L"Resources/2d/objicon/Map/Comment_00.png");
	IKESprite::LoadTexture(MAPCOMMENT01, L"Resources/2d/objicon/Map/Comment_01.png");
	IKESprite::LoadTexture(MAPCOMMENT02, L"Resources/2d/objicon/Map/Comment_02.png");
	IKESprite::LoadTexture(MAPCOMMENT03, L"Resources/2d/objicon/Map/Comment_03.png");
	IKESprite::LoadTexture(MAPSTART, L"Resources/2d/objicon/Map/map_start.png");
	IKESprite::LoadTexture(MAPROAD, L"Resources/2d/objicon/Map/map_road.png");
	IKESprite::LoadTexture(MAP_CHARA, L"Resources/2d/objicon/Map/map_chara.png");
	IKESprite::LoadTexture(MAP_FRAME, L"Resources/2d/objicon/Map/Frame.png");
	IKESprite::LoadTexture(MAP_START, L"Resources/2d/objicon/Map/map_00.png");
	IKESprite::LoadTexture(MAP_NORMAL, L"Resources/2d/objicon/Map/map_01.png");
	IKESprite::LoadTexture(MAP_BOSS, L"Resources/2d/objicon/Map/map_02.png");
	IKESprite::LoadTexture(MAP_HEAL, L"Resources/2d/objicon/Map/map_03.png");
	IKESprite::LoadTexture(MAP_TUTORIAL, L"Resources/2d/objicon/Map/map_04.png");
	IKESprite::LoadTexture(MAP_CHEACK, L"Resources/2d/objicon/Map/map_cheack.png");
	IKESprite::LoadTexture(MAP_CHEACK_OK, L"Resources/2d/objicon/Map/T_OK.png");
	IKESprite::LoadTexture(MAP_CHEACK_NO, L"Resources/2d/objicon/Map/T_Cancel.png");
	IKESprite::LoadTexture(TUTORIAL_SKIPUI, L"Resources/2d/objicon/SKIP.png");
	IKESprite::LoadTexture(PASSIVE_ACTIVE, L"Resources/2d/objicon/Passive/Passive_Active.png");
	IKESprite::LoadTexture(PASSIVE_00, L"Resources/2d/objicon/Passive/Passive_00.png");
	IKESprite::LoadTexture(PASSIVE_01, L"Resources/2d/objicon/Passive/PASSIVE_01.png");
	IKESprite::LoadTexture(PASSIVE_02, L"Resources/2d/objicon/Passive/PASSIVE_02.png");
	IKESprite::LoadTexture(PASSIVE_03, L"Resources/2d/objicon/Passive/PASSIVE_03.png");
	IKESprite::LoadTexture(PASSIVE_04, L"Resources/2d/objicon/Passive/PASSIVE_04.png");
	IKESprite::LoadTexture(PASSIVE_05, L"Resources/2d/objicon/Passive/PASSIVE_05.png");
	IKESprite::LoadTexture(PASSIVE_06, L"Resources/2d/objicon/Passive/PASSIVE_06.png");
	IKESprite::LoadTexture(PASSIVE_07, L"Resources/2d/objicon/Passive/PASSIVE_07.png");
	IKESprite::LoadTexture(PASSIVE_08, L"Resources/2d/objicon/Passive/PASSIVE_08.png");
	IKESprite::LoadTexture(PASSIVE_09, L"Resources/2d/objicon/Passive/PASSIVE_09.png");
	IKESprite::LoadTexture(PASSIVE_10, L"Resources/2d/objicon/Passive/PASSIVE_10.png");
	IKESprite::LoadTexture(PASSIVE_11, L"Resources/2d/objicon/Passive/PASSIVE_11.png");
	IKESprite::LoadTexture(PASSIVE_12, L"Resources/2d/objicon/Passive/PASSIVE_12.png");
	IKESprite::LoadTexture(PASSIVE_13, L"Resources/2d/objicon/Passive/PASSIVE_13.png");
	IKESprite::LoadTexture(PASSIVE_14, L"Resources/2d/objicon/Passive/PASSIVE_14.png");
	IKESprite::LoadTexture(PASSIVE_FRAME, L"Resources/2d/objicon/Passive_Frame.png");
	IKESprite::LoadTexture(GAUGE, L"Resources/2d/effect/white1x1.png");
	IKESprite::LoadTexture(GAUGECOVER, L"Resources/2d/objicon/SkillGauge.png");
	IKESprite::LoadTexture(HANDSCOVER, L"Resources/2d/objicon/HandsCover.png");
	IKESprite::LoadTexture(SKILLCANCEL, L"Resources/2d/objicon/SkillCancel.png");
	IKESprite::LoadTexture(TITLETEXT, L"Resources/2d/sceneback/TitleText.png");
	IKESprite::LoadTexture(GAMEOVERBACK, L"Resources/2d/sceneback/Gameover.png");
	IKESprite::LoadTexture(GAMECLEARBACK, L"Resources/2d/sceneback/GameClear.png");
	IKESprite::LoadTexture(ONOMATO_00, L"Resources/2d/objicon/Onomatope/00_Foot.png");
	IKESprite::LoadTexture(ONOMATO_01, L"Resources/2d/objicon/Onomatope/01_Attack01.png");
	IKESprite::LoadTexture(ONOMATO_02, L"Resources/2d/objicon/Onomatope/02_BOSSSPAWN.png");
	IKESprite::LoadTexture(ONOMATO_03, L"Resources/2d/objicon/Onomatope/03_GAMEOVER.png");
	IKESprite::LoadTexture(ONOMATO_04, L"Resources/2d/objicon/Onomatope/04_Charged.png");
	IKESprite::LoadTexture(ONOMATO_05, L"Resources/2d/objicon/Onomatope/05_Counter.png");
	IKESprite::LoadTexture(ONOMATO_06, L"Resources/2d/objicon/Onomatope/06_Attack02.png");
	IKESprite::LoadTexture(ONOMATO_07, L"Resources/2d/objicon/Onomatope/07_Attack03.png");
	IKESprite::LoadTexture(ONOMATO_08, L"Resources/2d/objicon/Onomatope/08_Attack04.png");
	IKESprite::LoadTexture(ONOMATO_09, L"Resources/2d/objicon/Onomatope/09_Diffence.png");
	IKESprite::LoadTexture(RESULTBACKSCREEN, L"Resources/2d/sceneback/ResultSkill_BS.png");
	IKESprite::LoadTexture(RESULTATTACKAREA, L"Resources/2d/objicon/Areas/AttackArea.png");
	IKESprite::LoadTexture(RESULTNOWCHECK, L"Resources/2d/objicon/SkillCheackNow.png");
	IKESprite::LoadTexture(RESULTBACKCHECK, L"Resources/2d/objicon/SkillCheackBack.png");
	IKESprite::LoadTexture(RESULTREPORTBACK, L"Resources/2d/sceneback/ResultRepot.png");
	IKESprite::LoadTexture(RESULTREPORTATTACK, L"Resources/2d/objicon/AttackResult.png");
	IKESprite::LoadTexture(RESULTREPORTDEFFENCE, L"Resources/2d/objicon/DeffenceResult.png");
	IKESprite::LoadTexture(RESULTSKIP, L"Resources/2d/objicon/Skip_A.png");
	IKESprite::LoadTexture(STAGEBACK, L"Resources/2d/sceneback/1.png");
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
	IKESprite::LoadTexture(ATTACK_10, L"Resources/2d/objicon/AttackSKill/Skill10.png");
	IKESprite::LoadTexture(ATTACK_11, L"Resources/2d/objicon/AttackSKill/Skill11.png");
	IKESprite::LoadTexture(ATTACK_12, L"Resources/2d/objicon/AttackSKill/Skill12.png");
	IKESprite::LoadTexture(ATTACK_13, L"Resources/2d/objicon/AttackSKill/Skill13.png");
	IKESprite::LoadTexture(ATTACK_14, L"Resources/2d/objicon/AttackSKill/Skill14.png");
	IKESprite::LoadTexture(TUTORIAL_WINDOW, L"Resources/2d/objicon/Tutorial.png");
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
	IKETexture::LoadTexture(PANNELLINE, L"Resources/2d/objicon/Areas/AreaLine.png");
	IKETexture::LoadTexture(AREA, L"Resources/2d/objicon/Areas/DamageArea.png");
	IKETexture::LoadTexture(PLAYERAREA, L"Resources/2d/objicon/Areas/PlayerAttack.png");
	IKETexture::LoadTexture(POISONAREA, L"Resources/2d/objicon/Areas/PoizonAttack.png");
	IKETexture::LoadTexture(HEALAREA, L"Resources/2d/objicon/Areas/HealArea.png");
	IKETexture::LoadTexture(BUFFAREA, L"Resources/2d/objicon/Areas/Muscle.png");
	IKETexture::LoadTexture(HATENAAREA, L"Resources/2d/objicon/Areas/hatena.png");
	IKETexture::LoadTexture(ENEMYPREDICT, L"Resources/2d/objicon/Areas/Predict.png");
	IKETexture::LoadTexture(PLAYERPREDICT, L"Resources/2d/objicon/Areas/PlayerPredict.png");
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
	IKETexture::LoadTexture(ATTACK_TEX_10, L"Resources/2d/objicon/AttackSKill/Skill10.png");
	IKETexture::LoadTexture(ATTACK_TEX_11, L"Resources/2d/objicon/AttackSKill/Skill11.png");
	IKETexture::LoadTexture(ATTACK_TEX_12, L"Resources/2d/objicon/AttackSKill/Skill12.png");
	IKETexture::LoadTexture(ATTACK_TEX_13, L"Resources/2d/objicon/AttackSKill/Skill13.png");
	IKETexture::LoadTexture(ATTACK_TEX_14, L"Resources/2d/objicon/AttackSKill/Skill14.png");
	IKETexture::LoadTexture(BOOM_DIR, L"Resources/2d/effect/Up.png");
	IKETexture::LoadTexture(POISON_EFFECT, L"Resources/2d/effect/poison.png");
	IKETexture::LoadTexture(HEAL_DAMAGE, L"Resources/2d/effect/shine2.png");
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
	ParticleManager::LoadTexture(Plus, "Heal.png");

}
