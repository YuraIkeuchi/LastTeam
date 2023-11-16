#include "InterEnemy.h"
#include "Collision.h"
#include <Helper.h>
#include <Player.h>
#include <StagePanel.h>
#include <GameStateManager.h>
#include <ParticleEmitter.h>
#include <TutorialTask.h>
Player* InterEnemy::player = nullptr;
XMFLOAT3 InterEnemy::randPanelPos() {
	//本当は4~7
	int width = Helper::GetInstance()->GetRanNum(4, 4);
	int height = Helper::GetInstance()->GetRanNum(0, 3);
	m_NowHeight = height;
	m_NowWidth = width;
	return StagePanel::GetInstance()->SetPositon(m_NowWidth, m_NowHeight);
}
XMFLOAT3 InterEnemy::SetPannelPos(int width, int height) {
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

	//各行動
	if (m_Alive) {
		Action();
	}
	const int l_BasePanelCount = 4;
	Helper::GetInstance()->CheckMax(m_DamegeTimer, 0, -1);
	//表示用のHP
	Helper::GetInstance()->Clamp(m_HP, 0.0f, m_MaxHP);

	m_InterHP = (int)(m_HP);

	////敵のマスを取得する
	if (m_EnemyTag == "Normal") {
	}
	StagePanel::GetInstance()->SetEnemyHit(m_Object.get(), m_NowWidth, m_NowHeight,m_Alive);

	if (m_HP != 0.0f) {
		for (auto i = 0; i < _drawnumber.size(); i++) {
			_drawnumber[i]->Update();
		}
	}
	else {
		m_Alive = false;
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
	if (!m_Alive) { return; }
	IKESprite::PreDraw();
	//HPバー
	hptex->Draw();
	//HP(数字)
	if(m_InterHP != 0)
	_drawnumber[FIRST_DIGHT]->Draw();
	if (m_InterHP >= 10)
		_drawnumber[SECOND_DIGHT]->Draw();
	if (m_InterHP >= 100)
		_drawnumber[THIRD_DIGHT]->Draw();
	IKESprite::PostDraw();
}
//当たり判定
void InterEnemy::Collide(vector<unique_ptr<AttackArea>>& area) {
	if (m_HP <= 0.0f) { return; }
	if (area.empty()) { return; }

	for (unique_ptr<AttackArea>& _area : area) {
		if ((_area->GetNowHeight() == m_NowHeight && _area->GetNowWidth() == m_NowWidth) &&
			!_area->GetHit()) {
			float damage = _area->GetDamage();
			if (_charaState == STATE_ATTACK && !GameStateManager::GetInstance()->GetCounter()) {
				GameStateManager::GetInstance()->SetCounter(true);
				damage *= 2.0f;
			}
			if (GameStateManager::GetInstance()->GetBuff()) {
				damage *= 2.0f;
			}
			m_HP -= damage;
			std::string name = _area->GetStateName();

			if (name == "DRAIN") {
				float rate = 0.2f;
				if (m_IsDrainUp) { rate *= 2.f; }
				player->HealPlayer(damage * rate);		//HP回復
			}
			else if (name == "POISON") {
				m_Poison = true;
			}
			BirthParticle();
			_area->SetHit(true);
			//チュートリアル専用
			if (TutorialTask::GetInstance()->GetTutorialState() == TASK_ATTACK) {
				TutorialTask::GetInstance()->SetTutorialState(TASK_DAMAGE);
			}
		}
	}
}
//パーティクル(ダメージ)
void InterEnemy::BirthParticle() {
	const XMFLOAT4 s_color = { 1.0f,0.3f,0.0f,1.0f };
	const XMFLOAT4 e_color = { 1.0f,0.3f,0.0f,1.0f };
	const float s_scale = 2.0f;
	const float e_scale = 0.0f;
	for (int i = 0; i < 20; i++) {
		ParticleEmitter::GetInstance()->Break(50, m_Position, s_scale, e_scale, s_color, e_color, 0.02f, 8.0f);
	}
}
//パーティクル(毒)
void InterEnemy::BirthPoisonParticle() {
	const XMFLOAT4 s_color = { 0.5f,0.0f,0.5f,1.0f };
	const XMFLOAT4 e_color = { 0.5f,0.0f,0.5f,1.0f };
	const float s_scale = 1.0f;
	const float e_scale = 0.0f;
	for (int i = 0; i < 3; i++) {
		ParticleEmitter::GetInstance()->PoisonEffect(50, { m_Position.x,m_Position.y + 1.0f,m_Position.z }, s_scale, e_scale, s_color, e_color);
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
	_drawnumber[FIRST_DIGHT]->SetExplain({ m_Position.x + 0.55f, m_Position.y, m_Position.z - 0.55f });
	_drawnumber[SECOND_DIGHT]->SetExplain({ m_Position.x + 0.2f, m_Position.y, m_Position.z - 0.55f });
	_drawnumber[THIRD_DIGHT]->SetExplain({ m_Position.x - 0.15f, m_Position.y, m_Position.z - 0.55f });
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

//毒
void InterEnemy::PoisonState() {
	if (!m_Poison) { return; }
	int kTimerMax = 800;
	if (m_PoisonLong) { kTimerMax *= 2; }
	m_PoisonTimer++;

	if (m_PoisonTimer % 80 == 0) {	//一定フレームで1ずつ減らす
		if (!m_IsVenom) {
			m_HP -= 1.0f;
		} else {
			m_HP -= 2.0f;
		}
		BirthPoisonParticle();
	}

	if (m_PoisonTimer == kTimerMax) {	//一定時間立ったら毒終了
		m_Poison = false;
		m_PoisonTimer = {};
	}
}