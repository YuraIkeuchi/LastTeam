#include "CanonEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
#include <StagePanel.h>
//fÇÝÝ
CanonEnemy::CanonEnemy() {
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
}
//ú»
bool CanonEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "hp")));
	m_MaxHP = m_HP;
	m_CheckPanel = true;
	return true;
}

void (CanonEnemy::* CanonEnemy::stateTable[])() = {
	&CanonEnemy::Inter,//®«ÌÔ
	&CanonEnemy::Attack,//®«ÌÔ
	&CanonEnemy::Teleport,//uÔÚ®
};

//s®
void CanonEnemy::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	//½è»è
	vector<AttackArea*> _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//½è»è
	PoisonState();//Å

	//GÌe
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Update();
		}
	}

	//áQ¨Ìí
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i] == nullptr) {
			continue;
		}

		if (!bullets[i]->GetAlive()) {
			bullets.erase(cbegin(bullets) + i);
		}
	}
}
//`æ
void CanonEnemy::Draw(DirectXCommon* dxCommon) {
	//GÌe
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Draw(dxCommon);
		}
	}
	Obj_Draw();
}
//ImGui`æ
void CanonEnemy::ImGui_Origin() {
	//GÌe
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->ImGuiDraw();
		}
	}
}
//Jú
void CanonEnemy::Finalize() {

}
//Ò@
void CanonEnemy::Inter() {
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, kIntervalMax);
	if (coolTimer == kIntervalMax) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		BirthBullet();
	}
}
//U
void CanonEnemy::Attack() {
	const int l_TargetTimer = 200;

	if (_CanonType == CANON_SET) {
		if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
			coolTimer = {};
			_CanonType = CANON_THROW;
		}
	}
	else if (_CanonType == CANON_THROW) {
		m_AttackCount++;
		BirthBullet();
		if (m_AttackCount != 2) {
			_CanonType = CANON_SET;
		}
		else {
			_CanonType = CANON_END;
		}
	}
	else {
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
		coolTimer = {};
		_CanonType = CANON_SET;
		StagePanel::GetInstance()->EnemyHitReset();
	}
}

//[v
void CanonEnemy::Teleport() {
	const int l_TargetTimer = 200;
	if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
		//m_Position = randPanelPos();
		_charaState = STATE_INTER;
		coolTimer = {};
		m_Position = StagePanel::GetInstance()->EnemySetPanel();
	}
}
//eÌ¶¬
void CanonEnemy::BirthBullet() {
		//áQ¨Ì­¶
		EnemyBullet* newbullet;
		newbullet = new EnemyBullet();
		newbullet->Initialize();

		newbullet->SetPolterType(TYPE_FOLLOW);
		newbullet->SetPosition({ m_Position.x,m_Position.y + 1.0f,m_Position.z });
		bullets.emplace_back(newbullet);
}