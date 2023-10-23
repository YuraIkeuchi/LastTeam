#include "InterEnemy.h"
#include "Collision.h"
#include <Helper.h>
#include <Player.h>
#include <StagePanel.h>
#include <GameStateManager.h>
#include <ParticleEmitter.h>
XMFLOAT3 InterEnemy::randPanelPos() {
	int width = Helper::GetInstance()->GetRanNum(4, 7);
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
			!_area->GetHit()) {
			float damage = 5.0f;
			if (_charaState == STATE_ATTACK && !GameStateManager::GetInstance()->GetCounter()) {
				GameStateManager::GetInstance()->SetCounter(true);
				damage *= 2.0f;
			}
			m_HP -= damage;
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
	//HPバー
	XMVECTOR tex2DPos = { m_Position.x - 0.2f, m_Position.y, m_Position.z - 0.25f };
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, m_MatView, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, m_MatProjection, true);
	tex2DPos = Helper::GetInstance()->WDivision(tex2DPos, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, m_MatPort, false);

	m_HPPos = { tex2DPos.m128_f32[0],tex2DPos.m128_f32[1] };

	//描画する数字と座標をここでセットする
	_drawnumber[FIRST_DIGHT]->SetExplain(m_FirstNumber, { m_Position.x + 0.6f, m_Position.y, m_Position.z - 0.55f });
	_drawnumber[SECOND_DIGHT]->SetExplain(m_SecondNumber, { m_Position.x + 0.2f, m_Position.y, m_Position.z - 0.55f });
	_drawnumber[THIRD_DIGHT]->SetExplain(m_ThirdNumber, { m_Position.x - 0.2f, m_Position.y, m_Position.z - 0.55f });
}
//カメラデータを取得
void InterEnemy::GetData(const XMMATRIX& matView, const XMMATRIX& matProjection, const XMMATRIX& matPort) {
	m_MatView = matView;
	m_MatProjection = matProjection;
	m_MatPort = matPort;

	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i]->GetCameraData(m_MatView,m_MatProjection,m_MatPort);
	}
}
//UIのためのHP管理
void InterEnemy::HPManage() {
	m_FirstNumber = getDigits(m_InterHP, 0, 0);
	m_SecondNumber = getDigits(m_InterHP, 1, 1);
	m_ThirdNumber = getDigits(m_InterHP, 2, 2);
}
//割合を求める
int InterEnemy::getDigits(int value, int m, int n) {
	int mod_value;
	int result;

	/* n桁目以下の桁を取得 */
	mod_value = value % (int)pow(10, n + 1);

	/* m桁目以上の桁を取得 */
	result = mod_value / (int)pow(10, m);

	return result;
}