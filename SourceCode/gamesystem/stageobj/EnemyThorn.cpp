#include "EnemyThorn.h"
#include "ModelManager.h"
#include <StagePanel.h>
#include <Helper.h>
#include <Easing.h>
EnemyThorn::EnemyThorn(const string& name) {
	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	if (name == "ICE") {
		m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::THORN));
	}
	else {
		m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::BOSS_ARM));
	}
	Initialize();
}
bool EnemyThorn::Initialize() {
	m_Position = {};
	m_Rotation = {};
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Scale = { 0.4f,0.25f,0.4f };
	return true;
}
//初期化
void EnemyThorn::InitState(const int width, const int height) {
	m_Position = SetPanelPos(width, height);
	m_Position.y = -0.5f;
	m_NowWidth = width, m_NowHeight = height;
	m_Alive = true;
	m_Hit = false;
	m_AliveTimer = {};
}
//更新
void EnemyThorn::Update() {
	Move();
	Obj_SetParam();
	Collide();
}
//描画
void EnemyThorn::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui
void EnemyThorn::ImGuiDraw() {

}
//動き
void EnemyThorn::Move() {
	const int l_TargetTimer = 3;
	static float addFrame = 1.f / 15.f;
	if (_ThornState == THORN_UP) {
		if (Helper::FrameCheck(m_Frame, addFrame)) {
			_ThornState = THORN_STOP;
			m_Frame = {};
			/// <summary>
			///	音入れ(突き刺し音希望)
			/// </summary>
			if (m_Sound) {
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Damage.wav", 0.02f);
				m_Sound = false;
			}
		}
		m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 0.4f);
	}
	else if (_ThornState == THORN_STOP) {
		if (Helper::CheckMin(m_AliveTimer, l_TargetTimer, 1)) {
			_ThornState = THORN_END;
			m_AliveTimer = false;
		}
	}
	else {
		if (Helper::FrameCheck(m_Frame, addFrame)) {
			m_Alive = false;
		}
		m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 0.0f);
	}
}
//当たり判定
void EnemyThorn::Collide() {
	if (_ThornState != THORN_STOP) { return; }
	if (m_Hit) { return; }

	int l_PlayerWidth = player->GetNowWidth();
	int l_PlayerHeight = player->GetNowHeight();
	const float l_Damage = 10.0f;
	if (m_NowWidth == l_PlayerWidth && m_NowHeight == l_PlayerHeight) {
		m_Hit = true;
		player->RecvDamage(m_Damage);
	}
}
//パネルの位置に置く
XMFLOAT3 EnemyThorn::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}