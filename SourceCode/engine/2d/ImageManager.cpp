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
	IKESprite::LoadTexture(VIGNETTE, L"Resources/2d/sceneback/vignette.png");
	IKESprite::LoadTexture(SHINE, L"Resources/2d/effect/effect7.png");
	IKESprite::LoadTexture(PLASHINE, L"Resources/2d/effect/effect3.png");
	IKESprite::LoadTexture(SHINE_S, L"Resources/2d/effect/effect4.png");
	IKESprite::LoadTexture(SHINE_L, L"Resources/2d/effect/effect5.png");
	IKESprite::LoadTexture(SMOKE, L"Resources/2d/effect/smoke.png");
	IKESprite::LoadTexture(POWERUP, L"Resources/2d/effect/Up.png");
	IKESprite::LoadTexture(ENEMYHPUI, L"Resources/2d/objicon/BossHP.png");
	IKESprite::LoadTexture(POIZONCOVER, L"Resources/2d/objicon/Numbers/PoisonFrame.png");
	IKESprite::LoadTexture(SHIELDCOVER, L"Resources/2d/objicon/Numbers/ShieldFrame.png");
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
	IKESprite::LoadTexture(MAP_SKILL_CLOSE, L"Resources/2d/objicon/Map/close_01.png");
	IKESprite::LoadTexture(MAP_BOSS_CLOSE, L"Resources/2d/objicon/Map/close_02.png");
	IKESprite::LoadTexture(MAP_PASSIVE_CLOSE, L"Resources/2d/objicon/Map/close_03.png");
	IKESprite::LoadTexture(MAP_TUTORIAL_CLOSE, L"Resources/2d/objicon/Map/close_04.png");
	IKESprite::LoadTexture(MAP_CHEACK, L"Resources/2d/objicon/Map/map_cheack.png");
	IKESprite::LoadTexture(MAP_CHEACK_OK, L"Resources/2d/objicon/Map/T_OK.png");
	IKESprite::LoadTexture(MAP_CHEACK_NO, L"Resources/2d/objicon/Map/T_Cancel.png");
	IKESprite::LoadTexture(MAP_SELECT, L"Resources/2d/objicon/Map/Map_Select.png");
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
	IKESprite::LoadTexture(GAUGECOVER, L"Resources/2d/objicon/SkillGauge.png");
	IKESprite::LoadTexture(HANDSCOVER, L"Resources/2d/objicon/HandsCover.png");
	IKESprite::LoadTexture(SKILLCANCEL, L"Resources/2d/objicon/SkillCancel.png");
	IKESprite::LoadTexture(TITLETEXT, L"Resources/2d/sceneback/TitleText.png");
	IKESprite::LoadTexture(GAMEOVERBACK, L"Resources/2d/sceneback/Gameover.png");
	IKESprite::LoadTexture(GAMEOVER_SELECT, L"Resources/2d/sceneback/OverSelect.png");
	IKESprite::LoadTexture(GAMEOVER_STICK, L"Resources/2d/objicon/Select.png");
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
	IKESprite::LoadTexture(ONOMATO_10, L"Resources/2d/objicon/Onomatope/10_pachipachi.png");
	IKESprite::LoadTexture(ONOMATO_11, L"Resources/2d/objicon/Onomatope/11_pafu.png");
	IKESprite::LoadTexture(RESULTBACKSCREEN, L"Resources/2d/sceneback/ResultSkill_BS.png");
	IKESprite::LoadTexture(RESULTATTACKAREA, L"Resources/2d/objicon/Areas/AttackArea.png");
	IKESprite::LoadTexture(RESULTNOWCHECK, L"Resources/2d/objicon/SkillCheackNow.png");
	IKESprite::LoadTexture(RESULTBACKCHECK, L"Resources/2d/objicon/SkillCheackBack.png");
	IKESprite::LoadTexture(RESULTREPORTBACK, L"Resources/2d/sceneback/ResultRepot.png");
	IKESprite::LoadTexture(RESULTREPORTATTACK, L"Resources/2d/objicon/AttackResult.png");
	IKESprite::LoadTexture(RESULTREPORTDEFFENCE, L"Resources/2d/objicon/DeffenceResult.png");
	IKESprite::LoadTexture(RESULTSKIP, L"Resources/2d/objicon/Skip_A.png");
	IKESprite::LoadTexture(RESULTNEXT, L"Resources/2d/objicon/Next.png");
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
	IKESprite::LoadTexture(ATTACK_15, L"Resources/2d/objicon/AttackSKill/Skill15.png");
	IKESprite::LoadTexture(ATTACK_16, L"Resources/2d/objicon/AttackSKill/Skill16.png");
	IKESprite::LoadTexture(ATTACK_17, L"Resources/2d/objicon/AttackSKill/Skill17.png");
	IKESprite::LoadTexture(ATTACK_18, L"Resources/2d/objicon/AttackSKill/Skill18.png");
	IKESprite::LoadTexture(ATTACK_19, L"Resources/2d/objicon/AttackSKill/Skill19.png");
	IKESprite::LoadTexture(ATTACK_20, L"Resources/2d/objicon/AttackSKill/Skill20.png");
	IKESprite::LoadTexture(ATTACK_21, L"Resources/2d/objicon/AttackSKill/Skill21.png");
	IKESprite::LoadTexture(ATTACK_22, L"Resources/2d/objicon/AttackSKill/Skill22.png");
	IKESprite::LoadTexture(ATTACK_23, L"Resources/2d/objicon/AttackSKill/Skill23.png");
	IKESprite::LoadTexture(ATTACK_24, L"Resources/2d/objicon/AttackSKill/Skill24.png");
	IKESprite::LoadTexture(ATTACK_25, L"Resources/2d/objicon/AttackSKill/Skill25.png");
	IKESprite::LoadTexture(ATTACK_26, L"Resources/2d/objicon/AttackSKill/Skill26.png");
	IKESprite::LoadTexture(ATTACK_27, L"Resources/2d/objicon/AttackSKill/Skill27.png");
	IKESprite::LoadTexture(ATTACK_28, L"Resources/2d/objicon/AttackSKill/Skill28.png");
	IKESprite::LoadTexture(ATTACK_29, L"Resources/2d/objicon/AttackSKill/Skill29.png");
	IKESprite::LoadTexture(ATTACK_BACK, L"Resources/2d/objicon/AttackSKill/BackCard.png");

	IKESprite::LoadTexture(TUTORIAL_WINDOW, L"Resources/2d/objicon/Tutorial.png");
	IKESprite::LoadTexture(TUTORIAL_TASK, L"Resources/2d/objicon/TutorialTask.png");
	IKESprite::LoadTexture(TUTORIAL_CHECK, L"Resources/2d/objicon/TutorialCheck.png");
	IKESprite::LoadTexture(SYUUTYUU, L"Resources/2d/sceneback/syutyu.png");
	IKESprite::LoadTexture(PLAYERPREDICT2D, L"Resources/2d/objicon/Areas/PlayerPredict.png");
	IKESprite::LoadTexture(DECKSKILLTOP, L"Resources/2d/objicon/DeckSkillT.png");
	IKESprite::LoadTexture(PICKSKILLTOP, L"Resources/2d/objicon/PickSkillT.png");

	IKESprite::LoadTexture(DECKDELETESHEET, L"Resources/2d/objicon/DeleteCheck.png");
	IKESprite::LoadTexture(DECKDELETEOK, L"Resources/2d/objicon/DeleteYes.png");
	IKESprite::LoadTexture(DECKDELETENO, L"Resources/2d/objicon/DeleteNo.png");
	IKESprite::LoadTexture(SHIELD, L"Resources/2d/objicon/Shield.png");
	IKESprite::LoadTexture(NODEATH, L"Resources/2d/objicon/nocontinue.png");
	IKESprite::LoadTexture(NODAMAGE, L"Resources/2d/objicon/nodamage.png");
	IKESprite::LoadTexture(HPGauge, L"Resources/2d/objicon/HPGauge.png");
	IKESprite::LoadTexture(HPGauge_W, L"Resources/2d/objicon/HPGauge_W.png");
	IKESprite::LoadTexture(HPCover, L"Resources/2d/objicon/HP_Gage.png");
	IKESprite::LoadTexture(SAVE, L"Resources/2d/sceneback/Save.png");
	IKESprite::LoadTexture(EXPRE, L"Resources/2d/objicon/Expra.png");
	IKESprite::LoadTexture(DELAY, L"Resources/2d/objicon/delay.png");
	IKESprite::LoadTexture(STICK, L"Resources/2d/objicon/SkillGauge2.png");
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
	IKETexture::LoadTexture(CHARGE2, L"Resources/2d/objicon/impact2.png");
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
	IKETexture::LoadTexture(NEOPLAYERPREDICT, L"Resources/2d/objicon/Areas/NeoPlayerPredict.png");

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
	IKETexture::LoadTexture(ATTACK_TEX_15, L"Resources/2d/objicon/AttackSKill/Skill15.png");
	IKETexture::LoadTexture(ATTACK_TEX_16, L"Resources/2d/objicon/AttackSKill/Skill16.png");
	IKETexture::LoadTexture(ATTACK_TEX_17, L"Resources/2d/objicon/AttackSKill/Skill17.png");
	IKETexture::LoadTexture(ATTACK_TEX_18, L"Resources/2d/objicon/AttackSKill/Skill18.png");
	IKETexture::LoadTexture(ATTACK_TEX_19, L"Resources/2d/objicon/AttackSKill/Skill19.png");
	IKETexture::LoadTexture(ATTACK_TEX_20, L"Resources/2d/objicon/AttackSKill/Skill20.png");
	IKETexture::LoadTexture(ATTACK_TEX_21, L"Resources/2d/objicon/AttackSKill/Skill21.png");
	IKETexture::LoadTexture(ATTACK_TEX_22, L"Resources/2d/objicon/AttackSKill/Skill22.png");
	IKETexture::LoadTexture(ATTACK_TEX_23, L"Resources/2d/objicon/AttackSKill/Skill23.png");
	IKETexture::LoadTexture(ATTACK_TEX_24, L"Resources/2d/objicon/AttackSKill/Skill24.png");
	IKETexture::LoadTexture(ATTACK_TEX_25, L"Resources/2d/objicon/AttackSKill/Skill25.png");
	IKETexture::LoadTexture(ATTACK_TEX_26, L"Resources/2d/objicon/AttackSKill/Skill26.png");
	IKETexture::LoadTexture(ATTACK_TEX_27, L"Resources/2d/objicon/AttackSKill/Skill27.png");
	IKETexture::LoadTexture(ATTACK_TEX_28, L"Resources/2d/objicon/AttackSKill/Skill28.png");
	IKETexture::LoadTexture(ATTACK_TEX_29, L"Resources/2d/objicon/AttackSKill/Skill29.png");

	IKETexture::LoadTexture(BOOM_DIR, L"Resources/2d/effect/Dir.png");
	IKETexture::LoadTexture(POISON_EFFECT, L"Resources/2d/effect/poison.png");
	IKETexture::LoadTexture(HEAL_DAMAGE, L"Resources/2d/effect/shine2.png");
	IKETexture::LoadTexture(COUNTER, L"Resources/2d/effect/Counter.png");
	IKETexture::LoadTexture(COUNTER_TWO, L"Resources/2d/effect/effect6.png");
	IKETexture::LoadTexture(CHANTING_HEAL, L"Resources/2d/objicon/ChantingHeal.png");
	IKETexture::LoadTexture(CHANTING_HEAL, L"Resources/2d/objicon/ChantingHeal.png");
	IKETexture::LoadTexture(BOM, L"Resources/2d/effect/effect5.png");
	IKETexture::LoadTexture(BOM2, L"Resources/2d/effect/Bom.png");
	IKETexture::LoadTexture(RELOADEFF, L"Resources/2d/effect/reload.png");
	IKETexture::LoadTexture(SHIELD_TEX, L"Resources/2d/objicon/Shield_Icon.png");
	IKETexture::LoadTexture(POWER_TEX, L"Resources/2d/effect/Up.png");
	IKETexture::LoadTexture(ABSEFFECT, L"Resources/2d/effect/effect1.png");
	IKETexture::LoadTexture(STUN, L"Resources/2d/effect/stun.png");
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
