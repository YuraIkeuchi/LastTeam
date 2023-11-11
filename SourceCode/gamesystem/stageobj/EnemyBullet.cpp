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
//初期化
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
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (EnemyBullet::* EnemyBullet::stateTable[])() = {
	&EnemyBullet::Follow,//追従
};
//更新
void EnemyBullet::Update() {
	//状態移行(charastateに合わせる)
	(this->*stateTable[m_PolterType])();
	//タイプによって色を一旦変えてる
	Obj_SetParam();

	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
	Collide();
	Particle();
}
//描画
void EnemyBullet::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui描画
void EnemyBullet::ImGuiDraw() {
	ImGui::Begin("Polter");
	ImGui::Text("Timer:%d", m_AliveTimer);
	ImGui::Text("POSY:%f", m_Position.y);
	ImGui::End();
}

//パーティクル
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

//当たり判定
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
//追従
void EnemyBullet::Follow() {
	const float l_AddFrame = 0.01f;
	const int l_BaseTimer = 40;
	const float l_AddCircle = 2.0f;
	//弾のセット(だんだん浮かび逢ふがるような感じ)
	if (m_ThrowType == THROW_SET) {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			m_ThrowType = THROW_INTER;

		}
		
		m_BaseScale = Ease(In, Cubic, m_Frame, m_BaseScale, 0.15f);
	}
	//狙う方向を決める
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
	//実際に狙っちゃう
	else {
		//プレイヤーにスピード加算
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		if (Helper::GetInstance()->CheckNotValueRange(m_Position.x, -30.0f, 30.0f)) {
			m_Alive = false;
		}
	}

}
