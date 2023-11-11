#include "EnemyBullet.h"
#include "Collision.h"
#include "Player.h"
#include "Helper.h"
#include <StagePanel.h>
#include <Easing.h>
EnemyBullet::EnemyBullet() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BULLET);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
}
//‰Šú‰»
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
//ó‘Ô‘JˆÚ
/*CharaState‚ÌState•À‚Ñ‡‚É‡‚í‚¹‚é*/
void (EnemyBullet::* EnemyBullet::stateTable[])() = {
	&EnemyBullet::Throw,//“Š‚°‚é
};
//XV
void EnemyBullet::Update() {
	//ó‘ÔˆÚs(charastate‚É‡‚í‚¹‚é)
	(this->*stateTable[m_PolterType])();
	//ƒ^ƒCƒv‚É‚æ‚Á‚ÄF‚ðˆê’U•Ï‚¦‚Ä‚é
	Obj_SetParam();

	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
	Collide();		//“–‚½‚è”»’è

	//’e‚Ìƒ}ƒX‚ðŽæ“¾‚·‚é
	//StagePanel::GetInstance()->SetCanonHit(m_Object.get(), m_NowWidth, m_NowHeight);
	//StagePanel::GetInstance()->SetCanonChange(m_NowWidth, m_NowHeight);
}
//•`‰æ
void EnemyBullet::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui•`‰æ
void EnemyBullet::ImGuiDraw() {
	ImGui::Begin("Bullet");
	ImGui::Text("NowHeight:%d,NowWidth:%d", m_NowHeight,m_NowWidth);
	ImGui::End();
}

//“–‚½‚è”»’è
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
//’Ç]
void EnemyBullet::Throw() {
	const float l_AddFrame = 0.01f;
	const int l_BaseTimer = 40;
	const float l_AddCircle = 2.0f;
	//’e‚ÌƒZƒbƒg(‚¾‚ñ‚¾‚ñ•‚‚©‚Ñˆ§‚Ó‚ª‚é‚æ‚¤‚ÈŠ´‚¶)
	if (m_ThrowType == THROW_SET) {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			m_ThrowType = THROW_INTER;

		}
		
		m_BaseScale = Ease(In, Cubic, m_Frame, m_BaseScale, 0.15f);
	}
	//‘_‚¤•ûŒü‚ðŒˆ‚ß‚é
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
	//ŽÀÛ‚É‘_‚Á‚¿‚á‚¤
	else {
		//’e‚ÉƒXƒs[ƒh‚ð‰ÁŽZ
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		if (Helper::GetInstance()->CheckNotValueRange(m_Position.z, 0.0f, 6.0f)) {		//”½ŽË‚·‚é
			m_Angle.y *= -1.0f;
		}
		if (Helper::GetInstance()->CheckNotValueRange(m_Position.x, -30.0f, 30.0f)) {
			m_Alive = false;
		}
	}

}
