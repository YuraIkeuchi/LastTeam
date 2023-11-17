#include "TackleEnemy.h"
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
TackleEnemy::TackleEnemy() {
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
bool TackleEnemy::Initialize() {

	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/TackleEnemy.csv", "hp")));
	m_MaxHP = m_HP;
	StagePanel::GetInstance()->EnemyHitReset();
	m_ShadowScale = { 0.05f,0.05f,0.05f };
	return true;
}

void (TackleEnemy::* TackleEnemy::stateTable[])() = {
	&TackleEnemy::Inter,//動きの合間
	&TackleEnemy::Attack,//動きの合間
};

//行動
void TackleEnemy::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//当たり判定
	PoisonState();//毒

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	shadow_tex->SetPosition(m_ShadowPos);
	shadow_tex->SetScale(m_ShadowScale);
	shadow_tex->Update();
}
//描画
void TackleEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	shadow_tex->Draw();
	IKETexture::PostDraw();
	Obj_Draw();
}
//ImGui描画
void TackleEnemy::ImGui_Origin() {
	ImGui::Begin("TackleEnemy");
	ImGui::Text("Height:%d,Width:%d", m_NowHeight, m_NowWidth);
	ImGui::Text("Timer:%d", m_PoisonTimer);
	ImGui::End();
}
//開放
void TackleEnemy::Finalize() {

}
//追従
//void NormalEnemy::Follow() {
//	Helper::GetInstance()->FollowMove(m_Position, Player::GetInstance()->GetPosition(), 0.05f);
//}

void TackleEnemy::Inter() {
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, kIntervalMax);
	if (coolTimer == kIntervalMax) {
		_charaState = STATE_ATTACK;
		coolTimer = 0;
	}
}

void TackleEnemy::Attack() {
	m_Position.x -= 0.4f;
	if (m_Position.x < -10.0f) {
		XMFLOAT3 l_RandPos = {};
		l_RandPos = StagePanel::GetInstance()->EnemySetPanel();
		_charaState = STATE_INTER;
		m_Position = l_RandPos;
	}
}

void TackleEnemy::Standby() {
}
