#include "EnemyBullet.h"
#include "Collision.h"
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
	//予測
	predictArea = std::make_unique<PredictArea>("ENEMY");
	predictArea->Initialize();

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
	m_AddSpeed = 1.7f;
	m_Alive = false;
	m_ThrowType = THROW_SET;
	m_ShadowScale = { 0.05f,0.05f,0.05f };
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/CanonEnemy.csv", "BULLET_DAMAGE")));
	return true;
}

void EnemyBullet::InitState(const XMFLOAT3& pos, const int ShotDir) {
	m_Position = pos;
	m_ShotDir = ShotDir;
	m_Rotation.y = 270.0f;
	m_Scale = { 0.0f,0.0f,0.0f };
	m_BaseScale = {};
	m_Color = { 0.6f,0.1f,0.1f,1.0f };
	m_AddSpeed = 1.7f;
	m_Alive = true;
	m_ThrowType = THROW_SET;
	m_ThrowTimer = {};
	rebirth = 1;
	Obj_SetParam();
	//弾のマスを取得する
	StagePanel::GetInstance()->SetPanelSearch(m_Object.get(), m_NowWidth, m_NowHeight);
}

//更新
void EnemyBullet::Update() {
	Throw();
	//タイプによって色を一旦変えてる
	Obj_SetParam();
	predictArea->Update();

	m_Scale = { m_BaseScale,m_BaseScale,m_BaseScale };
	Collide();		//当たり判定

	m_ShadowPos = { m_Position.x,0.11f,m_Position.z };
	shadow_tex->SetPosition(m_ShadowPos);
	shadow_tex->SetScale(m_ShadowScale);
	shadow_tex->Update();
}
//描画
void EnemyBullet::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	shadow_tex->Draw();
	predictArea->Draw(dxCommon);
	IKETexture::PostDraw();
	Obj_Draw();
}
//ImGui描画
void EnemyBullet::ImGuiDraw() {
	ImGui::Begin("Bullet");
	ImGui::Text("Alive:%d", m_Alive);
	ImGui::Text("ShotDir:%d", m_ShotDir);
	ImGui::Text("Scale:%f", m_BaseScale);
	ImGui::Text("PosX:%f", m_Position.x);
	ImGui::End();
}

//当たり判定
bool EnemyBullet::Collide() {
	XMFLOAT3 l_PlayerPos = player->GetPosition();
	const float l_Damage = 0.5f;
	const float l_Radius = 0.15f;
	if (Collision::CircleCollision(m_Position.x, m_Position.z, l_Radius, l_PlayerPos.x, l_PlayerPos.z, l_Radius) && (m_Alive)) {
		player->RecvDamage(m_Damage, "NORMAL");
		m_Alive = false;
		return true;
	} else {
		return false;
	}

	return false;
}
//追従
void EnemyBullet::Throw() {
	const float l_AddFrame = 0.5f;
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

		m_BaseScale = Ease(In, Cubic, m_Frame, m_BaseScale, 0.1f);
	}
	//狙う方向を決める
	else if (m_ThrowType == THROW_INTER) {
		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = {};
		mt19937 mt{ std::random_device{}() };
		m_ThrowTimer++;
		if (m_ThrowTimer == m_TargetTimer) {
			float l_Rot = {};
			if (m_ShotDir == DIR_STRAIGHT) {
				l_Rot = -90.0f;
			} else if (m_ShotDir == DIR_SLASHUP) {
				l_Rot = -45.0f;
			} else {
				l_Rot = -135.0f;
			}
			matRot = XMMatrixRotationY(XMConvertToRadians(l_Rot));
			move = XMVector3TransformNormal(move, matRot);
			m_Angle.x = move.m128_f32[0];
			m_Angle.y = move.m128_f32[2];
			m_ThrowTimer = 0;
			m_ThrowType = THROW_READY;
			nextHeightPanel = m_NowHeight;
		}
	} else if (m_ThrowType == THROW_READY) {
		int nextWidthPanel = m_NowWidth - (nextPredict + 1);
		if (nextWidthPanel < 0) {
			m_ThrowType = THROW_PLAY;
			nextPredict = 0;
			predictFrame = 0.f;
			/// <summary>
			/// ここに発射音（音入れ）
			/// </summary>
			return;
		}

		if (Helper::FrameCheck(predictFrame, 1.f / 5.0f)) {
			if (m_ShotDir == DIR_SLASHUP) {
				nextHeightPanel += 1 * rebirth;

				if (nextHeightPanel > 3) {
					nextHeightPanel = 2;
					rebirth = -1;
				}

				if (nextHeightPanel < 0) {
					nextHeightPanel = 1;
					rebirth = 1;
				}
			} else if (m_ShotDir == DIR_SLASHDOWN) {
				nextHeightPanel -= 1 * rebirth;
				if (nextHeightPanel < 0) {
					nextHeightPanel = 1;
					rebirth = -1;
				}
				if (nextHeightPanel > 3) {
					nextHeightPanel = 2;
					rebirth = 1;
				}
			}
			predictArea->VersePredict(nextWidthPanel, nextHeightPanel);
			nextPredict++;
			predictFrame = 0.f;
		}
	}
	//実際に狙っちゃう
	else {
		predictArea->VanishPredict(m_NowWidth, m_NowHeight);
		m_Rotation.x += 5.0f;
		//弾にスピードを加算
		m_Position.x += m_Angle.x * m_AddSpeed;
		m_Position.z += m_Angle.y * m_AddSpeed;
		if (Helper::CheckNotValueRange(m_Position.z, 0.0f, 4.5f)) {		//反射する
			m_Angle.y *= -1.0f;
		}
		if (Helper::CheckNotValueRange(m_Position.x, -6.0f, 10.0f)) {
			m_Alive = false;
			m_Position.x = -6.0f;
		}
	}
}
