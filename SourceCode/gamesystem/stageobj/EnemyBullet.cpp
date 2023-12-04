#include "EnemyBullet.h"
#include "Collision.h"
#include "Player.h"
#include "Helper.h"
#include <StagePanel.h>
#include <Easing.h>
#include <ImageManager.h>
#include "GameStateManager.h"

EnemyBullet::EnemyBullet() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BULLET);
	m_Object = make_unique<IKEObject3d>();
	m_Object->Initialize();
	m_Object->SetModel(m_Model);

	panels.tex.reset(new IKETexture(ImageManager::AREA, {}, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	panels.tex->TextureCreate();
	panels.tex->Initialize();
	float baseScale = PANEL_SIZE * 0.1f;
	panels.tex->SetScale({ baseScale,baseScale,baseScale });
	panels.tex->SetRotation({ 90.0f,0.0f,0.0f });

	shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });
}
//初期化
bool EnemyBullet::Initialize() {
	m_Position = { 0.0f,0.0f,0.0f };
	m_Rotation.y = 270.0f;
	m_Scale = { 0.0f,0.0f,0.0f };
	m_BaseScale = {};
	m_Color = { 0.6f,0.1f,0.1f,1.0f };
	m_AddSpeed = 1.0f;
	m_Alive = true;
	m_ThrowType = THROW_SET;
	m_AliveTimer = {};
	m_ShadowScale = { 0.05f,0.05f,0.05f };
	panels.position = {};
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/CanonEnemy.csv", "BULLET_DAMAGE")));
	panels.color = { 1.f,1.f,1.f,1.f };
	return true;
}
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (EnemyBullet::* EnemyBullet::stateTable[])() = {
	&EnemyBullet::Throw,//投げる
};
//更新
void EnemyBullet::Update() {
	//状態移行(charastateに合わせる)
	(this->*stateTable[m_PolterType])();
	//タイプによって色を一旦変えてる
	Obj_SetParam();

	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
	Collide();		//当たり判定

	panels.position = {(PANEL_SIZE * m_NowWidth) - (PANEL_HEIGHT * PANEL_SIZE) ,0.011f,(PANEL_SIZE * m_NowHeight)};
	panels.tex->SetPosition(panels.position);
	panels.tex->SetColor({1.0f,0.3f,0.0f,1.0f});
	panels.tex->Update();

	m_ShadowPos = { m_Position.x,0.11f,m_Position.z };
	shadow_tex->SetPosition(m_ShadowPos);
	shadow_tex->SetScale(m_ShadowScale);
	shadow_tex->Update();
}
//描画
void EnemyBullet::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	if (m_NowWidth <= 3) {
		panels.tex->Draw();
	}
	shadow_tex->Draw();
	IKETexture::PostDraw();
	Obj_Draw();
}
//ImGui描画
void EnemyBullet::ImGuiDraw() {
}

//当たり判定
bool EnemyBullet::Collide() {
	XMFLOAT3 l_PlayerPos = player->GetPosition();
	const float l_Damage = 0.5f;
	const float l_Radius = 0.15f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius) && (m_Alive)) {
		player->RecvDamage(m_Damage,"NORMAL");
		m_Alive = false;
		return true;
	}
	else {
		return false;
	}

	return false;
}
//追従
void EnemyBullet::Throw() {
	const float l_AddFrame = 0.1f;
	const int l_BaseTimer = 40;
	const float l_AddCircle = 2.0f;
	//弾のマスを取得する
	StagePanel::GetInstance()->SetPanelSearch(m_Object.get(), m_NowWidth, m_NowHeight);
	//弾のセット(だんだん浮かび逢ふがるような感じ)
	if (m_ThrowType == THROW_SET) {
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			m_ThrowType = THROW_INTER;
		}
		
		m_BaseScale = Ease(In, Cubic, m_Frame, m_BaseScale, 0.10f);
	}
	//狙う方向を決める
	else if (m_ThrowType == THROW_INTER) {
		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = {};
		mt19937 mt{ std::random_device{}() };
		m_ThrowTimer++;
		if (m_ThrowTimer == l_BaseTimer) {
			float l_Rot = {};
			if (m_ShotDir == DIR_STRAIGHT) {
				l_Rot = -90.0f;
			}
			else if (m_ShotDir == DIR_SLASHUP) {
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
		m_Rotation.x += 5.0f;
		//弾にスピードを加算
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		if (Helper::CheckNotValueRange(m_Position.z, 0.0f, 4.5f)) {		//反射する
			m_Angle.y *= -1.0f;
		}
		if (Helper::CheckNotValueRange(m_Position.x, -6.0f,10.0f)) {
			m_Alive = false;
		}
	}
}
