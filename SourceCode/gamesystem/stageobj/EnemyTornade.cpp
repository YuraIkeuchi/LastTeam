#include "EnemyTornade.h"
#include "ModelManager.h"
#include <StagePanel.h>
#include <Helper.h>
#include <Easing.h>
EnemyTornade::EnemyTornade() {
	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::TORNADO));
	Initialize();
}
bool EnemyTornade::Initialize() {
	m_Position = {};
	m_Rotation = {};
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Scale = { 0.0f,0.0f,0.0f };
	return true;
}
//������
void EnemyTornade::InitState(const int width, const int height) {
	m_Position = SetPanelPos(width, height);
	m_Position.y = -0.5f;
	m_NowWidth = width, m_NowHeight = height;
	m_Alive = true;
	m_Hit = false;
	m_AliveTimer = {};
}
//�X�V
void EnemyTornade::Update() {
	Move();
	Obj_SetParam();
	Collide();
}
//�`��
void EnemyTornade::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui
void EnemyTornade::ImGuiDraw() {

}
//����
void EnemyTornade::Move() {
	const int l_TargetTimer = 3;
	static float addFrame = 1.f / 15.f;
	if (_ThornState == THORN_UP) {
		if (Helper::FrameCheck(m_Frame, addFrame)) {
			
			_ThornState = THORN_STOP;
			m_Frame = {};
			/// <summary>
			///	������(��������])
			/// </summary>
			if (m_Sound) {
				Audio::GetInstance()->PlayWave("Resources/Sound/SE/Damage.wav", 0.02f);
				m_Sound = false;
			}
		}

		m_Scale = { Ease(In,Cubic,m_Frame,m_Scale.x,0.5f),
		Ease(In,Cubic,m_Frame,m_Scale.y,2.0f),
		Ease(In,Cubic,m_Frame,m_Scale.z,0.5f) };
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

	m_Rotation.y += 40.0f;
}
//�����蔻��
void EnemyTornade::Collide() {
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
//�p�l���̈ʒu�ɒu��
XMFLOAT3 EnemyTornade::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}