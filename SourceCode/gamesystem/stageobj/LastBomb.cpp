#include "LastBomb.h"
#include "Collision.h"
#include "Player.h"
#include "Helper.h"
#include <StagePanel.h>
#include <Easing.h>
#include <ImageManager.h>
#include "GameStateManager.h"

LastBomb::LastBomb() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BULLET);
	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//������
bool LastBomb::Initialize() {
	m_Alive = false;
	m_Rotation.y = 270.0f;
	m_Scale = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/SupportEnemy.csv", "BULLET_DAMAGE")));
	return true;
}

void LastBomb::InitState(const XMFLOAT3& pos) {
	m_Position = pos;
	m_Scale = { 0.0f,0.0f,0.0f };
	m_AfterPos = { m_Position.x - 1.5f,m_Position.y,m_Position.z };
	if (m_BirthCount == 0) {
		m_AfterPos.x = -1.5f;
	}
	m_AddPower = {};
	m_BirthCount++;
	m_BaseScale = {};
	m_Alive = true;
	m_Frame = {};
	_BombState = BOMB_SET;
}
//�X�V
void LastBomb::Update() {
	Throw();
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	Obj_SetParam();
	if (m_BirthCount == 4) {
		m_BirthCount = {};
	}
	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
	Collide();		//�����蔻��
	//m_Rotation.y += 20.0f;
}
//�`��
void LastBomb::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui�`��
void LastBomb::ImGuiDraw() {
}

//�����蔻��
bool LastBomb::Collide() {
	if (m_Hit) { return false; }
	if (!m_Alive) { return false; }
	XMFLOAT3 l_PlayerPos = player->GetPosition();
	const float l_Damage = 0.5f;
	const float l_Radius = 0.15f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius)) {
		player->RecvDamage(m_Damage, "NORMAL");
		m_Hit = true;
		return true;
	}
	else {
		return false;
	}

	return false;
}
//�Ǐ]
void LastBomb::Throw() {
	if (!m_Alive) { return; }
	const int l_BaseTimer = 40;
	const float l_AddFrame = 1 / 30.0f;
	const float l_AfterScale = 0.2f;

	//�e�̃}�X���擾����
	StagePanel::GetInstance()->SetPanelSearch(m_Object.get(), m_NowWidth, m_NowHeight);
	//�e�̃Z�b�g(���񂾂񕂂��ш��ӂ���悤�Ȋ���)
	if (_BombState == BOMB_SET) {
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			_BombState = BOMB_THROW;
			m_Jump = true;
			m_AddPower = 0.3f;
		}
		m_BaseScale = Ease(In, Cubic, m_Frame, m_BaseScale, l_AfterScale);
	}
	//���ۂɑ_�����Ⴄ
	else {
		if (m_Jump) {
			m_AddPower -= m_Gravity;
			if (Helper::CheckMax(m_Position.y, 0.1f, m_AddPower)) {
				m_AddPower = {};
				m_Jump = false;
				m_Position.y = 0.1f;
			}
		}

		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 1.0f;
		}
		else {
			m_Position.x = Ease(In, Cubic, m_Frame, m_Position.x, m_AfterPos.x);
		}
	}
}
