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

	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::BULLET));

	shockWaveTex.reset(new IKETexture(ImageManager::CHARGE, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shockWaveTex->TextureCreate();
	shockWaveTex->Initialize();
	shockWaveTex->SetRotation({ 90.0f,0.0f,0.0f });

	/*shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });*/

}
//初期化
bool Bomb::Initialize() {
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.0f,0.0f,0.0f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/Bomb.csv", "hp")));
	kIntervalMax = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/Bomb.csv", "Interval")));;
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/Bomb.csv", "Damage")));
	m_MaxHP = m_HP;
	m_CheckPanel = true;
	m_EnemyTag = "Bomb";
	m_ShadowScale = { 0.05f,0.05f,0.05f };
	m_BaseScale = {};
	_BombState = BOMB_SET;
	return true;
}

void (Bomb::* Bomb::stateTable[])() = {
	&Bomb::Inter,//動きの合間
	&Bomb::Attack,//動きの合間
	&Bomb::ShockWave,
};

//行動
void Bomb::Action() {

	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	//当たり判定
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//当たり判定

	if (m_HP <= 0.0f && _charaState != STATE_SPECIAL) {
		Attack();
	}

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	//shadow_tex->SetPosition(m_ShadowPos);
	//shadow_tex->SetScale(m_ShadowScale);
	//shadow_tex->Update();
	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
}

//描画
void Bomb::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	if (m_HealDamage) { healdamage_tex->Draw(); }
	if (_charaState == STATE_SPECIAL) {
		//衝撃波の描画
		shockWaveTex->Draw();
	}
	IKETexture::PostDraw();
	UIDraw();
	Obj_Draw();
	BaseBackDraw(dxCommon);

}
//ImGui描画
void Bomb::ImGui_Origin() {
	ImGui::Begin("Bomb");
	ImGui::Text("Scale.x:%f", m_Scale.x);
	ImGui::Text("PosY:%f", m_Position.y);
	ImGui::Text("Frame:%f", m_Frame);
	ImGui::End();
}
//開放
void Bomb::Finalize() {

}
//待機
void Bomb::Inter() {
	const float l_AddFrame = 1 / 30.0f;
	const float l_AfterScale = 0.2f;

	if (_BombState == BOMB_SET) {
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			_BombState = BOMB_THROW;
		}
		m_BaseScale = Ease(In, Cubic, m_Frame, m_BaseScale, l_AfterScale);
	}
	else {
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 1.0f;
			//衝撃波出してる間は時間進まないように
			//制限時間
			coolTimer++;
			coolTimer = clamp(coolTimer, 0, kIntervalMax);
			//時間切れ
			if (coolTimer == kIntervalMax) {
				coolTimer = 0;
				_charaState = STATE_ATTACK;
			}

			m_AddAngle = Helper::Lerp(10.0f, 30.0f, coolTimer, kIntervalMax);		//線形補間でチャージを表してる
				//sin波によって上下に動く
			m_SinAngle += m_AddAngle;
			m_SinAngle2 = m_SinAngle * (3.14f / 180.0f);
			m_Color.y = (sin(m_SinAngle2) * 0.5f + 0.5f);
			m_Color.z = (sin(m_SinAngle2) * 0.5f + 0.5f);
		}

		m_Position = { Ease(In,Cubic,m_Frame,m_Position.x,m_TargetPos.x),
		Ease(In,Cubic,m_Frame,m_Position.y,0.0f),
		Ease(In,Cubic,m_Frame,m_Position.z,m_TargetPos.z), };
	}
}
//攻撃
void Bomb::Attack() {
	if (m_HP <= 0) {
		//敵全体にダメージ
		GameStateManager::GetInstance()->SetIsBombDamage(true);
		m_shockWaveColor = { 0.0f,0.0f,1.0f,1.0f };
	}
	else {
		//プレイヤーにダメージ
		player->RecvDamage(m_Damage, "NORMAL");
		m_shockWaveColor = { 1.0f,0.0f,0.0f,1.0f };
	}
	//setcolor
	_charaState = STATE_SPECIAL;
}

void Bomb::ShockWave()
{
	shockWaveTex->SetPosition(m_Position.x, m_Position.y + 0.2f, m_Position.z);
	shockWaveTex->SetScale(m_shockWaveScale);
	shockWaveTex->SetColor(m_shockWaveColor);
	shockWaveTex->Update();

	m_shockWaveScale.x += m_addShockWaveScale;
	m_shockWaveScale.y += m_addShockWaveScale;
	m_shockWaveScale.z += m_addShockWaveScale;

	m_shockWaveTimer++;
	m_shockWaveTimer = clamp(m_shockWaveTimer, 0, m_maxShockWaveTimer);
	//時間切れ
	if (m_shockWaveTimer == m_maxShockWaveTimer) {
		m_Alive = false;
		m_shockWaveTimer = 0;
	}
	//衝撃波を出す
	//出し終わったらm_aliveをfalseに
}
//クリアシーンの更新
void Bomb::ClearAction() {
	m_AddDisolve = {};
	Obj_SetParam();
}
//ゲームオーバーシーンの更新
void Bomb::GameOverAction() {

}