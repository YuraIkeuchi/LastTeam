#include "EnemyBullet.h"
#include "Collision.h"
#include "Player.h"
#include "Helper.h"
#include <StagePanel.h>
#include <Easing.h>
#include <ImageManager.h>
EnemyBullet::EnemyBullet() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BULLET);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	m_Pannel.reset(new IKEObject3d());
	m_Pannel->Initialize();
	m_Pannel->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PANEL));
}
//������
bool EnemyBullet::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Rotation.y = 270.0f;
	m_Scale = { 0.0f,0.0f,0.0f };
	m_BaseScale = {};
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_AddSpeed = 1.0f;
	m_Alive = true;
	m_ThrowType = THROW_SET;
	m_AliveTimer = {};

	return true;
}
//��ԑJ��
/*CharaState��State���я��ɍ��킹��*/
void (EnemyBullet::* EnemyBullet::stateTable[])() = {
	&EnemyBullet::Throw,//������
};
//�X�V
void EnemyBullet::Update() {
	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[m_PolterType])();
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	Obj_SetParam();

	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
	Collide();		//�����蔻��

	m_PanelPos = {(-8.0f) + (2.0f * m_NowWidth),0.01f,(2.0f * m_NowHeight)};
	m_Pannel->SetPosition(m_PanelPos);
	m_Pannel->SetScale({2.0f,0.1f,2.0f});
	m_Pannel->SetColor({1.0f,0.3f,0.0f,1.0f});
	//m_Pannel->SetRotation({ 90.0f,0.0f,0.0f });
	m_Pannel->Update();
	//StagePanel::GetInstance()->SetCanonChange(m_NowWidth, m_NowHeight);
}
//�`��
void EnemyBullet::Draw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	if (m_ThrowType == THROW_PLAY) {
		m_Pannel->Draw();
	}
	IKEObject3d::PostDraw();
	Obj_Draw();
}
//ImGui�`��
void EnemyBullet::ImGuiDraw() {
	ImGui::Begin("Bullet");
	ImGui::Text("POSX:%f,POSZ:%f", m_PanelPos.x, m_PanelPos.z);
	ImGui::Text("NowHeight:%d,NowWidth:%d", m_NowHeight,m_NowWidth);
	ImGui::End();
}

//�����蔻��
bool EnemyBullet::Collide() {
	XMFLOAT3 l_PlayerPos = Player::GetInstance()->GetPosition();
	const float l_Damage = 0.5f;
	const float l_Radius = 0.2f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius) && (m_Alive)) {
		Player::GetInstance()->RecvDamage(5.0f);
		m_Alive = false;
		return true;
	}
	else {
		return false;
	}

	return false;
}
//�Ǐ]
void EnemyBullet::Throw() {
	const float l_AddFrame = 0.01f;
	const int l_BaseTimer = 40;
	const float l_AddCircle = 2.0f;
	//�e�̃}�X���擾����
	StagePanel::GetInstance()->SetPanelSearch(m_Object.get(), m_NowWidth, m_NowHeight);
	//�e�̃Z�b�g(���񂾂񕂂��ш��ӂ���悤�Ȋ���)
	if (m_ThrowType == THROW_SET) {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			m_ThrowType = THROW_INTER;

		}
		
		m_BaseScale = Ease(In, Cubic, m_Frame, m_BaseScale, 0.15f);
	}
	//�_�����������߂�
	else if (m_ThrowType == THROW_INTER) {
		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = {};
		mt19937 mt{ std::random_device{}() };
		m_ThrowTimer++;
		if (m_ThrowTimer == l_BaseTimer) {
			float l_Rot = {};
			int num = Helper::GetInstance()->GetRanNum(0, 2);
			if (num == DIR_STRAIGHT) {
				l_Rot = -90.0f;
			}
			else if (num == DIR_SLASHUP) {
				l_Rot = -45.0f;
			}
			else {
				l_Rot = -135.0f;
			}
			matRot = XMMatrixRotationY(XMConvertToRadians(l_Rot));
			move = XMVector3TransformNormal(move, matRot);
			m_Angle.x = move.m128_f32[0];
			m_Angle.y = move.m128_f32[2];
			m_ThrowTimer = 0;
			m_ThrowType = THROW_PLAY;
		}
	}
	//���ۂɑ_�����Ⴄ
	else {
		//�e�ɃX�s�[�h�����Z
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		if (Helper::GetInstance()->CheckNotValueRange(m_Position.z, 0.0f, 6.0f)) {		//���˂���
			m_Angle.y *= -1.0f;
		}
		if (Helper::GetInstance()->CheckNotValueRange(m_Position.x, -9.0f,10.0f)) {
			m_Alive = false;
		}
	}
}
