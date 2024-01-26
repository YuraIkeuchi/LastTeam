#include "EnemyRock.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
#include <StagePanel.h>
#include <ParticleEmitter.h>
//モデル読み込み
EnemyRock::EnemyRock() {

	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::ROCK));
	magic.tex.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	magic.tex->TextureCreate();
	magic.tex->Initialize();
	magic.tex->SetRotation({ 90.0f,0.0f,0.0f });
	shake = make_unique<Shake>();
}
//初期化
bool EnemyRock::Initialize() {
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Scale = { 0.0f,0.0f,0.0f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/Rock.csv", "hp")));
	kIntervalMax = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/Rock.csv", "Interval")));;
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/Rock.csv", "Damage")));
	m_MaxHP = m_HP;
	m_CheckPanel = true;
	m_EnemyTag = "Rock";
	return true;
}
//ステータス初期化
void EnemyRock::InitState(const int width, const int height, const XMFLOAT3& pos) {
	magic.Pos = pos;
	m_NowWidth = width, m_NowHeight = height;
	m_Position = SetPannelPos(width, height);
	m_Position.y = 2.5f;
	m_BaseScale = 0.4f;
	m_ReturnPos = m_Position;
	m_AddDisolve = 2.0f;
	magic.Alive = false;
	magic.Frame = {};
	magic.Scale = {};
	magic.AfterScale = 0.15f;
	magic.State = {};
}


void (EnemyRock::* EnemyRock::stateTable[])() = {
	&EnemyRock::Inter,//生成
	&EnemyRock::Attack,//攻撃
	&EnemyRock::Close,//待機
};

//行動
void EnemyRock::Action() {

	(this->*stateTable[_charaState])();
	Obj_SetParam();
	//当たり判定
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//当たり判定
	RockCollide();//岩の当たり判定

	if (m_HP <= 0.0f && _charaState != STATE_SPECIAL) {
		Attack();
	}

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	//shadow_tex->SetPosition(m_ShadowPos);
	//shadow_tex->SetScale(m_ShadowScale);
	//shadow_tex->Update();
	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };

	magic.tex->SetPosition(magic.Pos);
	magic.tex->SetScale({ magic.Scale,magic.Scale,magic.Scale });
	magic.tex->Update();
}

//描画
void EnemyRock::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	magic.tex->Draw();
	if (!m_Alive) { return; }
	if (_charaState == STATE_SPECIAL) {
		UIDraw();
	}
	if (m_Color.w != 0.0f && _RockState == ROCK_BIRTH)
	Obj_Draw();
}
//ImGui描画
void EnemyRock::ImGui_Origin() {
	ImGui::Begin("RockEnemy");
	ImGui::Text("AddDisolve:%f", m_AddDisolve);
	ImGui::Text("ScaleX:%f", m_Scale.x);
	ImGui::Text("ShakePosX:%f", m_ShakePos.x);
	ImGui::Text("MagPosX:%f", magic.Pos.x);
	ImGui::Text("Scale:%f", magic.Scale);
	ImGui::Text("RockState:%d", _RockState);
	ImGui::End();
}
//開放
void EnemyRock::Finalize() {

}
//待機
void EnemyRock::Inter() {
	const float l_AddFrame = 1 / 20.0f;
	const float l_AddDisolve = 0.05f;
	if (_RockState == ROCK_MAGIC) {
		if (Helper::FrameCheck(magic.Frame, l_AddFrame)) {
			_RockState = ROCK_FOLLOW;
			magic.Frame = {};
		}

		magic.Scale = Ease(In, Cubic, magic.Frame, magic.Scale, magic.AfterScale);
	}
	else if (_RockState == ROCK_FOLLOW) {
		if (Helper::FrameCheck(magic.Frame, l_AddFrame)) {
			_RockState = ROCK_BIRTH;
			magic.Frame = {};
		}

		magic.Pos = { Ease(In,Cubic,magic.Frame,magic.Pos.x,m_Position.x),
		magic.Pos.y,
		Ease(In,Cubic,magic.Frame,magic.Pos.z,m_Position.z) };
	}
	else {
		RockParticle();
		if (Helper::CheckMax(m_AddDisolve, 0.0f, -l_AddDisolve)) {
			shake->SetShakeStart(true);
			shake->ShakePos(m_ShakePos.x, 1, -1, 15, 30);
			shake->ShakePos(m_ShakePos.y, 1, -1, 15, 30);
			m_Position.x += m_ShakePos.x;
			m_Position.y += m_ShakePos.y;
			if (!shake->GetShakeStart()) {
				m_ShakePos = { 0.0f,0.0f,0.0f };
				m_Frame = {};
				_charaState = STATE_ATTACK;
				m_Position = m_ReturnPos;
			}
			magic.AfterScale = {};
		}
	}
}
//攻撃
void EnemyRock::Attack() {
	const float l_AddFrame = 1 / 30.0f;
	if (Helper::FrameCheck(magic.Frame, l_AddFrame)) {
		if (Helper::CheckMin(m_Timer, 20, 1)) {
			m_AddPower -= m_Gravity;
			if (Helper::CheckMax(m_Position.y, 0.1f, m_AddPower)) {
				/// <summary>
				///	音入れ(岩が落ちた重点音みたいな音(ドンッみたいな感じ))
				/// </summary>
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/heavyRockCollapse.wav", 0.01f);
				BirthParticle();
				_charaState = STATE_SPECIAL;
				StagePanel::GetInstance()->SetClose(m_NowWidth, m_NowHeight, true);
				m_Position.y = 0.1f;
			}
		}
	}
	else {
		magic.Scale = Ease(In, Cubic, magic.Frame, magic.Scale, magic.AfterScale);
	}
}

void EnemyRock::Close()
{
	const float l_AddFrame = 1 / 30.0f;
	if (Helper::CheckMin(m_Timer, kIntervalMax, 1)) {
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			StagePanel::GetInstance()->SetClose(m_NowWidth, m_NowHeight, false);
			StagePanel::GetInstance()->SetRock(m_NowWidth, m_NowHeight, false);
			m_Alive = false;
		}
		else {
			m_BaseScale = Ease(In, Cubic, m_Frame, m_BaseScale, 0.0f);
			//StagePanel::GetInstance()->ClosePanel(m_Object.get(), m_Alive);
		}
	}

}

bool EnemyRock::RockCollide() {
	if (_charaState != STATE_ATTACK) { return false; }
	if (m_Position.y >= 0.2f) { return false; }
	if (m_Hit) { return false; }
	XMFLOAT3 l_PlayerPos = player->GetPosition();
	const float l_Damage = 0.5f;
	const float l_Radius = 0.15f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius) && (m_Alive)) {
		player->RecvDamage(m_Damage, "BOUND");
		m_Hit = true;
		return true;
	}
	else {
		return false;
	}

	return false;
}

void EnemyRock::BirthParticle() {
	int l_life = 20;
	const float s_Scale = 1.5f;
	const float e_Scale = 0.0f;
	const XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	for (int i = 1; i < 7; i++) {
		ParticleEmitter::GetInstance()->SmokeEffect(l_life,m_Position, s_Scale, e_Scale, color, color, i);
	}
}

void EnemyRock::RockParticle() {
	int l_life = 50;
	const float s_Scale = 0.4f;
	const float e_Scale = 0.0f;
	const XMFLOAT4 color = { 1.0f,0.8f,0.0f,1.0f };
	m_ParticleTimer++;
	if (m_ParticleTimer == 5) {
		ParticleEmitter::GetInstance()->RockEffect(l_life, m_Position, s_Scale, e_Scale, color, color);
		m_ParticleTimer = {};
	}
}
//クリアシーンの更新
void EnemyRock::ClearAction() {
	m_AddDisolve = {};
	Obj_SetParam();
}
//ゲームオーバーシーンの更新
void EnemyRock::GameOverAction() {

}