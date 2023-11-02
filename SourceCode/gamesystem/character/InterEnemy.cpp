#include "InterEnemy.h"
#include "Collision.h"
#include <Helper.h>
#include <Player.h>
#include <StagePanel.h>
#include <GameStateManager.h>
#include <ParticleEmitter.h>
XMFLOAT3 InterEnemy::randPanelPos() {
	//本当は4~7
	int width = Helper::GetInstance()->GetRanNum(4, 4);
	int height = Helper::GetInstance()->GetRanNum(0, 3);
	m_NowHeight = height;
	m_NowWidth = width;
	return StagePanel::GetInstance()->SetPositon(m_NowWidth, m_NowHeight);
}
//初期化
bool InterEnemy::Initialize() {
	return true;
}
//更新
void InterEnemy::Update() {
	const int l_BasePanelCount = 4;
	Helper::GetInstance()->CheckMax(m_DamegeTimer, 0, -1);
	//表示用のHP
	Helper::GetInstance()->Clamp(m_HP, 0.0f, m_MaxHP);

	m_InterHP = (int)(m_HP);
	//敵のマスを取得する
	StagePanel::GetInstance()->SetEnemyHit(m_Object.get(),m_NowWidth,m_NowHeight);

	//各行動
	Action();
	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i]->Update();
	}
	//数値化したHP
	HPManage();
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
	//HPバー
	hptex->Draw();
	//HP(数字)
	_drawnumber[FIRST_DIGHT]->Draw();
	if (m_InterHP >= 10)
		_drawnumber[SECOND_DIGHT]->Draw();
	if (m_InterHP >= 100)
		_drawnumber[THIRD_DIGHT]->Draw();
	IKESprite::PostDraw();
}
//当たり判定
void InterEnemy::Collide(vector<AttackArea*>area) {
	//if (m_DamegeTimer != 0) { return; }
	//if (Player::GetInstance()->GetCharaState() != STATE_ATTACK) { return; }
	for (AttackArea* _area : area) {
		if (Collision::SphereCollision(_area->GetPosition(), m_Radius, m_Position, m_Radius) &&
			!_area->GetHit() && (m_HP > 0.0f)) {
			float damage = 5.0f;
			if (_charaState == STATE_ATTACK && !GameStateManager::GetInstance()->GetCounter()) {
				GameStateManager::GetInstance()->SetCounter(true);
				damage *= 2.0f;
			}
			m_HP -= _area->GetDamage();
			m_DamegeTimer = 40;
			BirthParticle();
			_area->SetHit(true);
		}
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
	Camera* camera = Helper::GetInstance()->GetCamera();
	m_MatView = camera->GetViewMatrix();
	m_MatProjection = camera->GetProjectionMatrix();
	m_MatPort = camera->GetViewPort();
	//HPバー
	XMVECTOR tex2DPos = { m_Position.x - 0.2f, m_Position.y, m_Position.z - 0.25f };
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, m_MatView, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, m_MatProjection, true);
	tex2DPos = Helper::GetInstance()->WDivision(tex2DPos, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, m_MatPort, false);

	m_HPPos = { tex2DPos.m128_f32[0],tex2DPos.m128_f32[1] };

	//描画する数字と座標をここでセットする
	_drawnumber[FIRST_DIGHT]->SetExplain({ m_Position.x + 0.6f, m_Position.y, m_Position.z - 0.55f });
	_drawnumber[SECOND_DIGHT]->SetExplain({ m_Position.x + 0.2f, m_Position.y, m_Position.z - 0.55f });
	_drawnumber[THIRD_DIGHT]->SetExplain({ m_Position.x - 0.2f, m_Position.y, m_Position.z - 0.55f });
	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i]->GetCameraData();
		_drawnumber[i]->SetNumber(m_DigitNumber[i]);
	}
}
//UIのためのHP管理
void InterEnemy::HPManage() {
	for (auto i = 0; i < _drawnumber.size(); i++) {
		m_DigitNumber[i] = Helper::GetInstance()->getDigits(m_InterHP, i, i);
	}
}
