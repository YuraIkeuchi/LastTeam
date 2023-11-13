#include "BossEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
#include <StagePanel.h>
//ƒ‚ƒfƒ‹“Ç‚İ‚İ
BossEnemy::BossEnemy() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	m_Object->SetLightEffect(false);
	//HPII
	hptex = IKESprite::Create(ImageManager::ENEMYHPUI, { 0.0f,0.0f });

	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i] = make_unique<DrawNumber>();
		_drawnumber[i]->Initialize();
	}

	shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });
}
//‰Šú‰»
bool BossEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 0.0f,1.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy.csv", "hp")));
	m_MaxHP = m_HP;
	m_CheckPanel = true;
	m_ShadowScale = { 0.05f,0.05f,0.05f };
	return true;
}
//ó‘Ô‘JˆÚ
void (BossEnemy::* BossEnemy::stateTable[])() = {
	&BossEnemy::Inter,//“®‚«‚Ì‡ŠÔ
	&BossEnemy::Attack,//“®‚«‚Ì‡ŠÔ
	&BossEnemy::Teleport,//uŠÔˆÚ“®
};
//UŒ‚‘JˆÚ
void (BossEnemy::* BossEnemy::attackTable[])() = {
	&BossEnemy::BulletAttack,//’e‚ğ‘Å‚ÂUŒ‚
};

//s“®
void BossEnemy::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	//“–‚½‚è”»’è
	vector<AttackArea*> _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//“–‚½‚è”»’è
	PoisonState();//“Å

	//“G‚Ì’e
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Update();
		}
	}

	//áŠQ•¨‚Ìíœ
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i] == nullptr) {
			continue;
		}

		if (!bullets[i]->GetAlive()) {
			bullets.erase(cbegin(bullets) + i);
		}
	}

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	shadow_tex->SetPosition(m_ShadowPos);
	shadow_tex->SetScale(m_ShadowScale);
	shadow_tex->Update();
}

//•`‰æ
void BossEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	shadow_tex->Draw();
	IKETexture::PostDraw();
	//“G‚Ì’e
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Draw(dxCommon);
		}
	}
	Obj_Draw();
}
//ImGui•`‰æ
void BossEnemy::ImGui_Origin() {
	//“G‚Ì’e
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->ImGuiDraw();
		}
	}
}
//ŠJ•ú
void BossEnemy::Finalize() {

}
//‘Ò‹@
void BossEnemy::Inter() {
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, kIntervalMax);
	if (coolTimer == kIntervalMax) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
	}
}
//UŒ‚
void BossEnemy::Attack() {
	(this->*attackTable[_AttackState])();
}

//ƒ[ƒv
void BossEnemy::Teleport() {
	const int l_TargetTimer = 200;
	XMFLOAT3 l_RandPos = {};
	l_RandPos = StagePanel::GetInstance()->EnemySetPanel();
	if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
		//m_Position = randPanelPos();
		_charaState = STATE_INTER;
		coolTimer = {};
		m_Position = l_RandPos;
		StagePanel::GetInstance()->EnemyHitReset();
	}
}
//’e‚Ì¶¬
void BossEnemy::BirthBullet() {
	//áŠQ•¨‚Ì”­¶
	EnemyBullet* newbullet;
	newbullet = new EnemyBullet();
	newbullet->Initialize();

	newbullet->SetPolterType(TYPE_FOLLOW);
	newbullet->SetPosition({ m_Position.x,m_Position.y + 1.0f,m_Position.z });
	bullets.emplace_back(newbullet);
}
//UŒ‚‘JˆÚ
//’e
void BossEnemy::BulletAttack() {
	const int l_TargetTimer = 150;

	if (_BossType == Boss_SET) {
		if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
			coolTimer = {};
			_BossType = Boss_THROW;
		}
	}
	else if (_BossType == Boss_THROW) {
		m_AttackCount++;
		BirthBullet();
		if (m_AttackCount != 5) {
			_BossType = Boss_SET;
		}
		else {
			_BossType = Boss_END;
		}
	}
	else {
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
		coolTimer = {};
		_BossType = Boss_SET;
		StagePanel::GetInstance()->EnemyHitReset();
	}
}