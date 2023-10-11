#include "InterEnemy.h"
#include "Collision.h"
#include <Helper.h>
#include <Player.h>
#include <StagePanel.h>
#include <ParticleEmitter.h>
XMFLOAT3 InterEnemy::randPanelPos() {
	int width = Helper::GetInstance()->GetRanNum(4, 7);
	int height = Helper::GetInstance()->GetRanNum(4, 7);
	return StagePanel::GetInstance()->SetPositon(width, height);;
}
//初期化
bool InterEnemy::Initialize() {
	return true;
}
//更新
void InterEnemy::Update() {
	Helper::GetInstance()->CheckMax(m_DamegeTimer, 0, -1);
	//各行動
	Action();
	//UIをワールド座標に変換する
	WorldDivision();
	hptex->SetPosition(m_HPPos);
	hptex->SetSize({ HpPercent() * m_HPSize.x,m_HPSize.y });
}
//描画
void InterEnemy::Draw(DirectXCommon* dxCommon) {
}

void InterEnemy::ImGuiDraw() {
	ImGui_Origin();
}
//UIの描画
void InterEnemy::UIDraw() {
	IKESprite::PreDraw();
	hptex->Draw();
	IKESprite::PostDraw();
}
//当たり判定
void InterEnemy::Collide() {
	if (m_DamegeTimer != 0) { return; }
	if (Player::GetInstance()->GetCharaState() != STATE_ATTACK) { return; }
	if (Collision::SphereCollision(Player::GetInstance()->GetPosition(), m_Radius, m_Position, m_Radius)) {
		m_HP -= 5.0f;
		m_DamegeTimer = 40;
		BirthParticle();
	}
}
//パーティクル
void InterEnemy::BirthParticle() {
	const XMFLOAT4 s_color = { 0.5f,0.5f,0.5f,1.0f };
	const XMFLOAT4 e_color = { 0.5f,0.5f,0.5f,1.0f };
	const float s_scale = 2.0f;
	const float e_scale = 0.0f;
	for (int i = 0; i < 20; i++) {
		ParticleEmitter::GetInstance()->Break(50, m_Position, s_scale, e_scale, s_color, e_color, 0.02f, 8.0f);
	}
}
//HPの割合
float InterEnemy::HpPercent() {
	float temp = m_HP / m_MaxHP;
	Helper::GetInstance()->Clamp(temp, 0.0f, 1.0f);
	return temp;
}
//スプライトを敵の市に出す
void InterEnemy::WorldDivision() {
	XMVECTOR tex2DPos = { m_Position.x - 0.2f, m_Position.y, m_Position.z - 0.25f };
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, m_MatView, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, m_MatProjection, true);
	tex2DPos = Helper::GetInstance()->WDivision(tex2DPos, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, m_MatPort, false);

	m_HPPos = { tex2DPos.m128_f32[0],tex2DPos.m128_f32[1] };
}
//カメラデータを取得
void InterEnemy::GetData(const XMMATRIX& matView, const XMMATRIX& matProjection, const XMMATRIX& matPort) {
	m_MatView = matView;
	m_MatProjection = matProjection;
	m_MatPort = matPort;
}
