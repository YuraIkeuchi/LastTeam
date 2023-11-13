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
//モデル読み込み
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

	shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });
}
//初期化
bool CanonEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/CanonEnemy.csv", "hp")));
	m_MaxHP = m_HP;
	m_CheckPanel = true;
	m_ShadowScale = { 0.05f,0.05f,0.05f };
	return true;
}

void (CanonEnemy::* CanonEnemy::stateTable[])() = {
	&CanonEnemy::Inter,//動きの合間
	&CanonEnemy::Attack,//動きの合間
	&CanonEnemy::Teleport,//瞬間移動
};

//行動
void CanonEnemy::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	//当たり判定
	vector<AttackArea*> _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//当たり判定
	PoisonState();//毒

	//敵の弾
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Update();
		}
	}

	//障害物の削除
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

//描画
void CanonEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	shadow_tex->Draw();
	IKETexture::PostDraw();
	//敵の弾
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Draw(dxCommon);
		}
	}
	Obj_Draw();
}
//ImGui描画
void CanonEnemy::ImGui_Origin() {
	//敵の弾
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->ImGuiDraw();
		}
	}
}
//開放
void CanonEnemy::Finalize() {

}
//待機
void CanonEnemy::Inter() {
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, kIntervalMax);
	if (coolTimer == kIntervalMax) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
		BirthBullet();
	}
}
//攻撃
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

//ワープ
void CanonEnemy::Teleport() {
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
//弾の生成
void CanonEnemy::BirthBullet() {
		//障害物の発生
		EnemyBullet* newbullet;
		newbullet = new EnemyBullet();
		newbullet->Initialize();

		newbullet->SetPolterType(TYPE_FOLLOW);
		newbullet->SetPosition({ m_Position.x,m_Position.y + 1.0f,m_Position.z });
		bullets.emplace_back(newbullet);
}