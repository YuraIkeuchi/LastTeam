#include "Bomb.h"
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
Bomb::Bomb() {
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
	kIntervalMax = 450;
}
//初期化
bool Bomb::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy.csv", "hp")));
	m_MaxHP = m_HP;
	m_CheckPanel = true;
	m_ShadowScale = { 0.05f,0.05f,0.05f };
	return true;
}

void (Bomb::* Bomb::stateTable[])() = {
	&Bomb::Inter,//動きの合間
	&Bomb::Attack,//動きの合間
};

//行動
void Bomb::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	//当たり判定
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//当たり判定

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	shadow_tex->SetPosition(m_ShadowPos);
	shadow_tex->SetScale(m_ShadowScale);
	shadow_tex->Update();
}

//描画
void Bomb::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	shadow_tex->Draw();
	IKETexture::PostDraw();
	UIDraw();
	Obj_Draw();
}
//ImGui描画
void Bomb::ImGui_Origin() {

}
//開放
void Bomb::Finalize() {

}
//待機
void Bomb::Inter() {
	//制限時間
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, kIntervalMax);
	//時間切れ
	if (coolTimer == kIntervalMax) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
	}
}
//攻撃
void Bomb::Attack() {
	//プレイヤーにダメージ
	player->RecvDamage(10, "NORMAL");
}
