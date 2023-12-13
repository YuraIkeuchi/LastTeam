#include "Sickle.h"
#include "ModelManager.h"
#include <StagePanel.h>
#include <Helper.h>
#include <Easing.h>
Sickle::Sickle() {
	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::SICKLE));
	Initialize();
}
bool Sickle::Initialize() {
	m_Position = {};
	m_Rotation.y = 90.0f;
	m_Color = { 1.0f,1.0f,1.0f,0.0f };
	m_Scale = { 0.7f,0.7f,0.7f };
	return true;
}
//初期化
void Sickle::InitState(const int width, const int height, const XMFLOAT3 Pos) {
	m_Position = Pos;
	m_Position.y = 2.0f;
	m_NowWidth = width, m_NowHeight = height;
	m_Alive = true;
	m_Hit = false;
	m_AliveTimer = {};
}
//更新
void Sickle::Update() {
	Move();
	Obj_SetParam();
	Collide();
}
//描画
void Sickle::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui
void Sickle::ImGuiDraw() {

}
//動き
void Sickle::Move() {
	const int l_TargetTimer = 3;
	static float addFrame = 1.f / 15.f;
	if (_ThornState == THORN_UP) {
		if (Helper::FrameCheck(m_Frame, addFrame)) {
			_ThornState = THORN_STOP;
			m_Frame = {};
		}
		m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 1.0f);
	}
	else if (_ThornState == THORN_STOP) {
		if (Helper::FrameCheck(m_Frame, addFrame)) {
			if (Helper::CheckMin(m_AliveTimer, l_TargetTimer, 1)) {
				_ThornState = THORN_END;
				m_AliveTimer = {};
				m_Frame = {};
			}
		}
		m_Rotation.x = Ease(In, Cubic, m_Frame, m_Rotation.x, -450.0f);
		m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 0.5f);
	}
	else {
		if (Helper::FrameCheck(m_Frame, addFrame)) {
			m_Alive = false;
		}
		m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 0.0f);
	}
}
//当たり判定
void Sickle::Collide() {
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
XMFLOAT3 Sickle::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}