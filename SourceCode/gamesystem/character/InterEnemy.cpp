#include "InterEnemy.h"
#include "Collision.h"
#include <Helper.h>
#include <Player.h>
#include <StagePanel.h>
#include <ParticleEmitter.h>
XMFLOAT3 InterEnemy::randPanelPos() {
	int width = Helper::GetInstance()->GetRanNum(4, 7);
	int height = Helper::GetInstance()->GetRanNum(4, 7);
	m_NowHeight = 3;
	m_NowWidth = 4;
	return StagePanel::GetInstance()->SetPositon(m_NowWidth, m_NowHeight);
}
//初期化
bool InterEnemy::Initialize() {
	return true;
}
//更新
void InterEnemy::Update() {
	Helper::GetInstance()->CheckMax(m_DamegeTimer, 0, -1);
	m_InterHP = (int)(m_HP);
	//各行動
	Action();
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
	HP_First[m_FirstNumber]->Draw();
	if (m_InterHP >= 10)
		HP_Second[m_SecondNumber]->Draw();
	if (m_InterHP >= 100)
		HP_Third[m_ThirdNumber]->Draw();
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
			if (_charaState == STATE_ATTACK && !Player::GetInstance()->GetIsCounter()) {
				Player::GetInstance()->SetIsCounter(true);
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

	//数値化したHP(ひとけた目)
	XMVECTOR texHPFirst;
	texHPFirst = { m_Position.x + 0.6f, m_Position.y, m_Position.z - 0.55f };
	texHPFirst = Helper::GetInstance()->PosDivi(texHPFirst, m_MatView, false);
	texHPFirst = Helper::GetInstance()->PosDivi(texHPFirst, m_MatProjection, true);
	texHPFirst = Helper::GetInstance()->WDivision(texHPFirst, false);
	texHPFirst = Helper::GetInstance()->PosDivi(texHPFirst, m_MatPort, false);
	//二桁め
	XMVECTOR texHPSecond;
	texHPSecond = { m_Position.x + 0.2f, m_Position.y, m_Position.z - 0.55f };
	texHPSecond = Helper::GetInstance()->PosDivi(texHPSecond, m_MatView, false);
	texHPSecond = Helper::GetInstance()->PosDivi(texHPSecond, m_MatProjection, true);
	texHPSecond = Helper::GetInstance()->WDivision(texHPSecond, false);
	texHPSecond = Helper::GetInstance()->PosDivi(texHPSecond, m_MatPort, false);
	//三桁め
	XMVECTOR texHPThird;
	texHPThird = { m_Position.x - 0.2f, m_Position.y, m_Position.z - 0.55f };
	texHPThird = Helper::GetInstance()->PosDivi(texHPThird, m_MatView, false);
	texHPThird = Helper::GetInstance()->PosDivi(texHPThird, m_MatProjection, true);
	texHPThird = Helper::GetInstance()->WDivision(texHPThird, false);
	texHPThird = Helper::GetInstance()->PosDivi(texHPThird, m_MatPort, false);

	m_FirstPos = { texHPFirst.m128_f32[0],texHPFirst.m128_f32[1] };
	m_SecondPos = { texHPSecond.m128_f32[0],texHPSecond.m128_f32[1] };
	m_ThirdPos = { texHPThird.m128_f32[0],texHPThird.m128_f32[1] };
}
//カメラデータを取得
void InterEnemy::GetData(const XMMATRIX& matView, const XMMATRIX& matProjection, const XMMATRIX& matPort) {
	m_MatView = matView;
	m_MatProjection = matProjection;
	m_MatPort = matPort;
}
//UIのためのHP管理
void InterEnemy::HPManage() {
	m_FirstNumber = getDigits(m_InterHP, 0, 0);
	m_SecondNumber = getDigits(m_InterHP, 1, 1);
	m_ThirdNumber = getDigits(m_InterHP, 2, 2);

	for (int i = 0; i < HP_First.size(); i++) {
		HP_First[i]->SetPosition(m_FirstPos);
		HP_Second[i]->SetPosition(m_SecondPos);
		HP_Third[i]->SetPosition(m_ThirdPos);
	}
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