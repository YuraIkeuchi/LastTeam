#include "EnemyBullet.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Player.h"
#include "Helper.h"
#include <ParticleEmitter.h>
#include <Easing.h>
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
	&EnemyBullet::Follow,//�Ǐ]
};
//�X�V
void EnemyBullet::Update() {
	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[m_PolterType])();
	//�^�C�v�ɂ���ĐF����U�ς��Ă�
	Obj_SetParam();

	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
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
void EnemyBullet::Follow() {
	const float l_AddFrame = 0.01f;
	const int l_BaseTimer = 40;
	const float l_AddCircle = 2.0f;
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
		//�v���C���[�ɃX�s�[�h���Z
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		if (Helper::GetInstance()->CheckNotValueRange(m_Position.x, -30.0f, 30.0f)) {
			m_Alive = false;
		}
	}

}
