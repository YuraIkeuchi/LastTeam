#include "EnemyBullet.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Player.h"
#include "Helper.h"
#include <ParticleEmitter.h>
#include <Easing.h>
#include "GameStateManager.h"

EnemyBullet::EnemyBullet() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BULLET);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//������
bool EnemyBullet::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Rotation.y = 270.0f;
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_AddSpeed = 3.0f;
	m_Alive = true;
	m_AfterPos.y = 2.0f;
	m_ThrowType = THROW_SET;
	m_AliveTimer = {};

	return true;
}
//��ԑJ��
/*CharaState��State���я��ɍ��킹��*/
void (EnemyBullet::* EnemyBullet::stateTable[])() = {
	&EnemyBullet::Follow,//�Ǐ]
};
//�X�V
void EnemyBullet::Update() {
	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[m_PolterType])();
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	Obj_SetParam();

	Collide();
	Particle();
}
//�`��
void EnemyBullet::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui�`��
void EnemyBullet::ImGuiDraw() {
	ImGui::Begin("Polter");
	ImGui::Text("Timer:%d", m_AliveTimer);
	ImGui::Text("POSY:%f", m_Position.y);
	ImGui::End();
}

//�p�[�e�B�N��
void EnemyBullet::Particle() {
	XMFLOAT4 s_color = { 0.0f,0.4f,1.0f,1.0f };
	XMFLOAT4 s_color2 = { 0.4f,0.0f,1.0f,1.0f };
	XMFLOAT4 e_color = { 1.0f,1.0f,1.0f,1.0f };
	float s_scale = 0.3f;
	float e_scale = 0.0f;
	const int m_Life = 50;
	//if (m_Alive) {
	//	if (m_PolterType == TYPE_FOLLOW) {
	//		ParticleEmitter::GetInstance()->FireEffect(m_Life, m_Position, s_scale, e_scale, s_color, e_color);
	//	}
	//	else {
	//		ParticleEmitter::GetInstance()->FireEffect(m_Life, m_Position, s_scale, e_scale, s_color2, e_color);
	//	}
	//}
}

//�����蔻��
bool EnemyBullet::Collide() {
	auto player_data = GameStateManager::GetInstance()->GetPlayer().lock();
	XMFLOAT3 l_PlayerPos = player_data->GetPosition();
	const float l_Damage = 0.5f;
	const float l_Radius = 0.2f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius) && (m_Alive)) {
		player_data->RecvDamage(5.0f);
		m_Alive = false;
		return true;
	}
	else {
		return false;
	}

	return false;
}
//�Ǐ]
void EnemyBullet::Follow() {
	auto player_data = GameStateManager::GetInstance()->GetPlayer().lock();
	const float l_AddFrame = 0.01f;
	const int l_BaseTimer = 40;
	const float l_AddCircle = 2.0f;
	//�e�̃Z�b�g(���񂾂񕂂��ш��ӂ���悤�Ȋ���)
	if (m_ThrowType == THROW_SET) {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			m_ThrowType = THROW_INTER;

		}
		m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, m_AfterPos.y);
		m_Position = Helper::GetInstance()->CircleMove({ m_BasePos.x,m_Position.y,m_BasePos.z }, m_CircleScale, m_CircleSpeed);
	}
	//�_�����������߂�
	else if (m_ThrowType == THROW_INTER) {
		m_ThrowTimer++;
		if (m_ThrowTimer == l_BaseTimer + m_TargetTimer) {
			double sb, sbx, sbz;
			sbx = player_data->GetPosition().x - m_Position.x;
			sbz = player_data->GetPosition().z - m_Position.z;
			sb = sqrt(sbx * sbx + sbz * sbz);
			m_SpeedX = sbx / sb * 0.1f;
			m_SpeedZ = sbz / sb * 0.1f;
			m_ThrowTimer = 0;
			m_ThrowType = THROW_PLAY;
		}
		m_CircleSpeed += l_AddCircle;
		m_Position = Helper::GetInstance()->CircleMove({ m_BasePos.x,m_Position.y,m_BasePos.z }, m_CircleScale, m_CircleSpeed);
	}
	//���ۂɑ_�����Ⴄ
	else {
		//�v���C���[�ɃX�s�[�h���Z
		m_Position.x += (float)m_SpeedX;
		m_Position.z += (float)m_SpeedZ;

		if (Helper::GetInstance()->CheckNotValueRange(m_Position.x, -30.0f, 30.0f) || Helper::GetInstance()->CheckNotValueRange(m_Position.z, -30.0f, 30.0f)) {
			m_Alive = false;
		}
	}

}
