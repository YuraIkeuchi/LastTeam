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

//モデル読み込み
EnemyRock::EnemyRock() {

	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::BULLET));
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
	m_EnemyTag = "EnemyRock";
	return true;
}
//ステータス初期化
void EnemyRock::InitState(const int width, const int height) {
	m_NowWidth = width, m_NowHeight = height;
	m_Position = SetPannelPos(width, height);
	m_Position.y = 2.0f;
}


void (EnemyRock::* EnemyRock::stateTable[])() = {
	&EnemyRock::Inter,//生成
	&EnemyRock::Attack,//攻撃
	&EnemyRock::Close,//待機
};

//行動
void EnemyRock::Action() {

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
void EnemyRock::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	UIDraw();
	Obj_Draw();
}
//ImGui描画
void EnemyRock::ImGui_Origin() {
	
}
//開放
void EnemyRock::Finalize() {

}
//待機
void EnemyRock::Inter() {
	const float l_AddFrame = 1 / 30.0f;
	const float l_AfterScale = 0.2f;
	if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
		m_Frame = {};
		_charaState = STATE_ATTACK;
	}
	m_BaseScale = Ease(In, Cubic, m_Frame, m_BaseScale, l_AfterScale);
}
//攻撃
void EnemyRock::Attack() {
	m_AddPower -= m_Gravity;
	if (Helper::CheckMax(m_Position.y, 0.1f, m_AddPower)) {
		_charaState = STATE_SPECIAL;
		m_Position.y = 0.1f;
	}
}

void EnemyRock::Close()
{
	const float l_AddFrame = 1 / 30.0f;
	if (Helper::CheckMin(m_Timer, kIntervalMax, 1)) {
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
		}
		else {
			m_BaseScale = Ease(In, Cubic, m_Frame, m_BaseScale, 0.0f);
		}
	}

	StagePanel::GetInstance()->ClosePanel(m_Object.get(), m_Alive);
}
