#include "InterEnemy.h"
#include "Collision.h"
#include <Helper.h>
#include <Player.h>
#include <StagePanel.h>
#include <GameStateManager.h>
#include <ParticleEmitter.h>
#include <TutorialTask.h>
#include "ImageManager.h"
#include <Slow.h>
#include "Passive.h"
Player* InterEnemy::player = nullptr;
XMFLOAT3 InterEnemy::randPanelPos() {
	//本当は4~7
	int width = Helper::GetRanNum(4, 4);
	int height = Helper::GetRanNum(0, 3);
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
void InterEnemy::BaseInitialize(IKEModel* _model) {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(_model);
	m_Object->SetLightEffect(false);

	//HPII
	hptex = IKESprite::Create(ImageManager::ENEMYHPUI, { 0.0f,0.0f });
	hptex->SetColor({ 0.5f,1.0f,0.5f,1.0f });

	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i] = make_unique<DrawNumber>(0.5f);
		_drawnumber[i]->Initialize();
	}
	poisonState = IKESprite::Create(ImageManager::POIZONCOVER, { 0.0f,0.0f });
	poisonState->SetSize({ 32.f,32.f });
	poisonState->SetAnchorPoint({ 0.5f,0.5f });
	for (auto i = 0; i < _drawPoisonnumber.size(); i++) {
		_drawPoisonnumber[i] = make_unique<DrawPoisonNumber>(0.5f);
		_drawPoisonnumber[i]->Initialize();
	}

}
void InterEnemy::SkipInitialize() {
	m_AddDisolve = 0.0f;
}
//更新
void InterEnemy::Update() {
	if (!GameStateManager::GetInstance()->GetGameStart()) { return; }
	if (m_HP != 0.0f) {
		//各行動
		if (m_EnemyTag != "Bomb") {
			if (m_Alive) {
				Action();
			}
		} else {
			Action();
		}
	} else {
	}

	const int l_BasePanelCount = 4;
	Helper::CheckMax(m_DamegeTimer, 0, -1);
	//表示用のHP
	Helper::Clamp(m_HP, 0.0f, m_MaxHP);

	m_InterHP = (int)(m_HP);

	////敵のマスを取得する
	StagePanel::GetInstance()->SetEnemyHit(m_Object.get(), m_NowWidth, m_NowHeight, m_Alive);

	if (m_HP != 0.0f) {
		for (auto i = 0; i < _drawnumber.size(); i++) {
			_drawnumber[i]->Update();
		}

	} else {
		if (m_EnemyTag != "Bomb") {
			//m_Alive = false;
		}
	}

	//数字の削除
	for (int i = 0; i < _damagenumber.size(); i++) {
		if (_damagenumber[i] == nullptr) {
			continue;
		}

		_damagenumber[i]->Update();
		if (!_damagenumber[i]->GetAlive()) {
			_damagenumber.erase(cbegin(_damagenumber) + i);
		}
	}

	//数字の削除
	for (int i = 0; i < _healnumber.size(); i++) {
		if (_healnumber[i] == nullptr) {
			continue;
		}

		_healnumber[i]->Update();
		if (!_healnumber[i]->GetAlive()) {
			_healnumber.erase(cbegin(_healnumber) + i);
		}
	}
	//だめーじ関係
	DamageUpdate();
	//数値化したHP
	HPManage();
	//UIをワールド座標に変換する
	WorldDivision();
	hptex->SetPosition(m_HPPos);
	hptex->SetSize({ HpPercent() * m_HPSize.x,m_HPSize.y });
}
//バトル前の更新
void InterEnemy::AwakeUpdate() {
	if (GameStateManager::GetInstance()->GetGameStart()) { return; }
	if (!StagePanel::GetInstance()->GetCreateFinish()) { return; }
	const float l_AddDisolve = 0.05f;
	//ディゾルブを解除する
	if (Helper::CheckMax(m_AddDisolve, 0.0f, -l_AddDisolve)) {
		GameStateManager::GetInstance()->SetGameStart(true);
	}

	Obj_SetParam();
}
//描画
void InterEnemy::Draw(DirectXCommon* dxCommon) {
}

void InterEnemy::ImGuiDraw() {
	ImGui_Origin();
	////敵のダメージテキスト
	//for (unique_ptr<DrawDamageNumber>& newnumber : _damagenumber) {
	//	if (newnumber != nullptr) {
	//		newnumber->ImGuiDraw();
	//	}
	//}
}
//UIの描画
void InterEnemy::UIDraw() {
	//if (!GameStateManager::GetInstance()->GetGameStart() && m_EnemyTag != "Mob") { return; }
	IKESprite::PreDraw();
	if (m_HP != 0.0f) {
		//HPバー
		hptex->Draw();
		//HP(数字)
		if (m_InterHP != 0)
			_drawnumber[FIRST_DIGHT]->Draw();
		if (m_InterHP >= 10)
			_drawnumber[SECOND_DIGHT]->Draw();
		if (m_InterHP >= 100)
			_drawnumber[THIRD_DIGHT]->Draw();
	}
	//敵のダメージテキスト
	for (unique_ptr<DrawDamageNumber>& newnumber : _damagenumber) {
		if (newnumber != nullptr) {
			newnumber->Draw();
		}
	}
	//敵のヒールテキスト
	for (unique_ptr<DrawHealNumber>& newnumber : _healnumber) {
		if (newnumber != nullptr) {
			newnumber->Draw();
		}
	}
	if (m_Poison) {
		poisonState->Draw();
		//敵のポイズンテキスト
		if (m_PoisonToken >= 0) {
			_drawPoisonnumber[FIRST_DIGHT]->Draw();
		}
		if (m_PoisonToken >= 10) {
			_drawPoisonnumber[SECOND_DIGHT]->Draw();
		}
		if (m_PoisonToken >= 100) {
			_drawPoisonnumber[THIRD_DIGHT]->Draw();
		}
	}
	IKESprite::PostDraw();
}
//当たり判定
void InterEnemy::Collide(vector<unique_ptr<AttackArea>>& area) {
	if (m_HP <= 0.0f) { return; }
	if (area.empty()) { return; }

	for (unique_ptr<AttackArea>& _area : area) {
		if ((_area->GetNowHeight() == m_NowHeight && _area->GetNowWidth() == m_NowWidth) &&
			!_area->GetHit() && _area->GetName() == "Player") {
			float damage = _area->GetDamage();
			//固定ダメージか否か
			if (!_area->GetIsFixed()) {
				if (_charaState == STATE_ATTACK &&
					!GameStateManager::GetInstance()->GetCounter()) {
					GameStateManager::GetInstance()->SetCounter(true);
					damage *= 1.5f;
				}
				if (GameStateManager::GetInstance()->GetBuff()) {
					damage *= 2.0f;
				}
				if (GameStateManager::GetInstance()->GetIsFivePower()) {
					damage *= 1.2f;
					GameStateManager::GetInstance()->SetPassiveActive((int)Passive::ABILITY::FIVE_POWER);
				}
				if (GameStateManager::GetInstance()->GetTakenDamageUp()) {
					float up = (float)GameStateManager::GetInstance()->GetTakenDamageNum() * 0.2f;
					if (up >= 1.0f) {
						damage += up;
						GameStateManager::GetInstance()->SetPassiveActive((int)Passive::ABILITY::TAKENDAMAGEUP);
					}
				}
			} else {
				if (_charaState == STATE_ATTACK &&
					!GameStateManager::GetInstance()->GetCounter()) {
					GameStateManager::GetInstance()->SetCounter(true);
				}

			}
			m_Damege = true;
			m_DamageTimer = {};
			m_HP -= damage;
			GameStateManager::GetInstance()->DamageCheck((int)damage);
			BirthDamage(damage);
			std::string name = _area->GetStateName();

			if (name == "DRAIN") {
				float rate = 0.2f;
				if (m_IsDrainUp) {
					rate *= 2.f;
					GameStateManager::GetInstance()->SetPassiveActive((int)Passive::ABILITY::DRAIN_HEALUP);
				}
				player->HealPlayer(damage * rate);		//HP回復
			} else if (name == "POISON") {
				m_Poison = true;
				if (!m_IsVenom) {
					m_PoisonToken += 4;
				} else {
					GameStateManager::GetInstance()->SetPassiveActive((int)Passive::ABILITY::POIZON_DAMAGEUP);
					m_PoisonToken += 8;
				}
			}
			BirthParticle();

			Camera* camera = Helper::GetCamera();
			m_MatView = camera->GetViewMatrix();
			m_MatProjection = camera->GetProjectionMatrix();
			m_MatPort = camera->GetViewPort();
			//HPバー
			XMVECTOR tex2DPos = { m_Position.x, m_Position.y, m_Position.z };
			tex2DPos = Helper::PosDivi(tex2DPos, m_MatView, false);
			tex2DPos = Helper::PosDivi(tex2DPos, m_MatProjection, true);
			tex2DPos = Helper::WDivision(tex2DPos, false);
			tex2DPos = Helper::PosDivi(tex2DPos, m_MatPort, false);

			GameStateManager::GetInstance()->DamageEffectInit({ tex2DPos.m128_f32[0],tex2DPos.m128_f32[1] });
			_area->SetHit(true);
			//チュートリアル専用
			if (TutorialTask::GetInstance()->GetTutorialState() == TASK_ATTACK) {
				TutorialTask::GetInstance()->SetTutorialState(TASK_DAMAGE);
			}
		}
	}
}
void InterEnemy::SimpleDamege(float damage) {
	if (m_HP <= 0.0f) { return; }
	float hp = m_HP;
	hp -= damage;
	Helper::Clamp(hp, 1.0f, m_HP);
	m_HP = hp;
	BirthParticle();
}

void InterEnemy::SimpleHeal(float heal) {
	if (m_HP <= 0.0f) { return; }

	float l_HealNum = {};

	if (m_HP != m_MaxHP) {
		BirthHealParticle();

		if (m_MaxHP - m_HP >= heal) {
			l_HealNum = heal;
		} else {
			l_HealNum = m_MaxHP - m_HP;
		}
		BirthHealNumber(l_HealNum);
		m_HP += heal;
	}
}


//パーティクル(ダメージ)
void InterEnemy::BirthParticle() {
	const XMFLOAT4 s_color = { 1.0f,0.3f,0.0f,1.0f };
	const XMFLOAT4 e_color = { 1.0f,0.3f,0.0f,1.0f };
	const float s_scale = 1.0f;
	const float e_scale = 0.0f;
	int l_Life = {};
	float l_Divi = {};

	//最後の敵かどうかでパーティクルが変わる
	if (m_LastEnemy && m_HP <= 0.1f) {
		l_Life = 500;
		l_Divi = 20.0f;
	} else {
		l_Life = 50;
		l_Divi = 8.0f;
	}
	for (int i = 0; i < 20; i++) {
		ParticleEmitter::GetInstance()->Break(l_Life, m_Position, s_scale, e_scale, s_color, e_color, 0.02f, l_Divi);
	}
}
//パーティクル(毒)
void InterEnemy::BirthPoisonParticle() {
	const XMFLOAT4 s_color = { 0.5f,0.0f,0.5f,1.0f };
	const XMFLOAT4 e_color = { 0.5f,0.0f,0.5f,1.0f };
	const float s_scale = 1.0f;
	const float e_scale = 0.0f;
	int l_Life = {};
	float l_Divi = {};

	//最後の敵かどうかでパーティクルが変わる
	if (!m_LastEnemy) {
		l_Life = 50;
		l_Divi = 3.0f;
	} else {
		if (m_HP == 0.0f) {
			l_Life = 500;
			l_Divi = 20.0f;
		} else {
			l_Life = 50;
			l_Divi = 8.0f;
		}
	}
	for (int i = 0; i < 3; i++) {
		ParticleEmitter::GetInstance()->PoisonEffect(l_Life, { m_Position.x,m_Position.y + 1.0f,m_Position.z }, s_scale, e_scale, s_color, e_color, 0.02f, l_Divi);
	}
}

//パーティクル(ヒール)
void InterEnemy::BirthHealParticle() {
	const XMFLOAT4 s_color = { 0.5f,1.0f,0.1f,1.0f };
	const XMFLOAT4 e_color = { 0.5f,1.0f,0.1f,1.0f };
	const float s_scale = 1.0f;
	const float e_scale = 0.0f;
	int l_Life = {};
	float l_Divi = {};

	//最後の敵かどうかでパーティクルが変わる
	if (!m_LastEnemy) {
		l_Life = 50;
		l_Divi = 5.0f;
	} else {
		if (m_HP == 0.0f) {
			l_Life = 500;
			l_Divi = 20.0f;
		} else {
			l_Life = 50;
			l_Divi = 8.0f;
		}
	}
	for (int i = 0; i < 20; i++) {
		ParticleEmitter::GetInstance()->HealEffect(l_Life, m_Position, s_scale, e_scale, s_color, e_color);
	}
}

//HPの割合
float InterEnemy::HpPercent() {
	float temp = m_HP / m_MaxHP;
	Helper::Clamp(temp, 0.0f, 1.0f);
	return temp;
}
//スプライトを敵の市に出す
void InterEnemy::WorldDivision() {
	Camera* camera = Helper::GetCamera();
	m_MatView = camera->GetViewMatrix();
	m_MatProjection = camera->GetProjectionMatrix();
	m_MatPort = camera->GetViewPort();
	//HPバー
	XMVECTOR tex2DPos = { m_Position.x - 0.2f, m_Position.y, m_Position.z - 0.25f };
	tex2DPos = Helper::PosDivi(tex2DPos, m_MatView, false);
	tex2DPos = Helper::PosDivi(tex2DPos, m_MatProjection, true);
	tex2DPos = Helper::WDivision(tex2DPos, false);
	tex2DPos = Helper::PosDivi(tex2DPos, m_MatPort, false);

	m_HPPos = { tex2DPos.m128_f32[0],tex2DPos.m128_f32[1] };

	//描画する数字と座標をここでセットする
	_drawnumber[FIRST_DIGHT]->SetExplain({ m_Position.x + 0.35f, m_Position.y, m_Position.z - 0.55f });
	_drawnumber[SECOND_DIGHT]->SetExplain({ m_Position.x, m_Position.y, m_Position.z - 0.55f });
	_drawnumber[THIRD_DIGHT]->SetExplain({ m_Position.x - 0.35f, m_Position.y, m_Position.z - 0.55f });
	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i]->GetCameraData();
		_drawnumber[i]->SetNumber(m_DigitNumber[i]);
	}
}
//UIのためのHP管理
void InterEnemy::HPManage() {
	for (auto i = 0; i < _drawnumber.size(); i++) {
		m_DigitNumber[i] = Helper::getDigits(m_InterHP, i, i);
	}

	for (auto i = 0; i < _drawPoisonnumber.size(); i++) {
		m_PoisonTokenNum[i] = Helper::getDigits(m_PoisonToken, i, i);
	}

	//描画する数字と座標をここでセットする
	if (m_PoisonToken >= 100) {
		_drawPoisonnumber[FIRST_DIGHT]->SetExplain({ m_Position.x + 2.0f, m_Position.y, m_Position.z - 0.55f });
		_drawPoisonnumber[SECOND_DIGHT]->SetExplain({ m_Position.x + 1.65f, m_Position.y, m_Position.z - 0.55f });
		_drawPoisonnumber[THIRD_DIGHT]->SetExplain({ m_Position.x + 1.3f, m_Position.y, m_Position.z - 0.55f });
		XMFLOAT2 pos = _drawPoisonnumber[THIRD_DIGHT]->GetPosition();
		poisonState->SetPosition({ pos.x - 35.f,pos.y });
	} else if (m_PoisonToken >= 10) {
		_drawPoisonnumber[FIRST_DIGHT]->SetExplain({ m_Position.x + 1.65f, m_Position.y, m_Position.z - 0.55f });
		_drawPoisonnumber[SECOND_DIGHT]->SetExplain({ m_Position.x + 1.3f, m_Position.y, m_Position.z - 0.55f });
		XMFLOAT2 pos = _drawPoisonnumber[SECOND_DIGHT]->GetPosition();
		poisonState->SetPosition({ pos.x - 35.f,pos.y });
	} else {
		_drawPoisonnumber[FIRST_DIGHT]->SetExplain({ m_Position.x + 1.3f, m_Position.y, m_Position.z - 0.55f });
		XMFLOAT2 pos = _drawPoisonnumber[FIRST_DIGHT]->GetPosition();
		poisonState->SetPosition({ pos.x - 35.f,pos.y });
	}
	for (auto i = 0; i < _drawPoisonnumber.size(); i++) {
		_drawPoisonnumber[i]->GetCameraData();
		_drawPoisonnumber[i]->SetNumber(m_PoisonTokenNum[i]);
		_drawPoisonnumber[i]->Update();
	}
}

//毒
void InterEnemy::PoisonState() {
	if (!m_Poison) { return; }
	if (m_PoisonToken <= 0) {
		m_Poison = false;
		m_PoisonToken = 0;
		m_PoisonTimer = 0;
	}
	//毒ヒット時タイマーリセットするか
	//仕様変更必要
	//新毒仕様
	int kTimerMax = 140;//
	m_PoisonTimer++;
	if (m_PoisonTimer % kTimerMax == 0) {	//一定フレームで1ずつ減らす
		m_HP -= m_PoisonToken;
		BirthDamage((float)m_PoisonToken);
		BirthPoisonParticle();
		if (m_PoisonLong) {
			//GameStateManager::GetInstance()->SetPassiveActive();
			m_PoisonToken /= 4;
		} else {
			m_PoisonToken /= 2;
		}

		m_PoisonTimer = 0;
	}
}
//ダメージテキスト
void InterEnemy::BirthDamage(const float Damage) {
	int l_InterDamage = {};//int変換したダメージ
	l_InterDamage = (int)Damage;

	if (l_InterDamage < 10) {
		unique_ptr<DrawDamageNumber> _newnumber = make_unique<DrawDamageNumber>();
		_newnumber->GetCameraData();
		_newnumber->SetExplain({ m_Position.x, m_Position.y, m_Position.z + 1.0f });
		_newnumber->Initialize();
		_newnumber->SetNumber(l_InterDamage);
		_damagenumber.push_back(std::move(_newnumber));
	} else {
		int l_DightDamage[DAMAGE_MAX];
		for (auto i = 0; i < DAMAGE_MAX; i++) {
			l_DightDamage[i] = Helper::getDigits(l_InterDamage, i, i);
			unique_ptr<DrawDamageNumber> _newnumber = make_unique<DrawDamageNumber>();
			_newnumber->GetCameraData();
			if (i == 0) {
				_newnumber->SetExplain({ m_Position.x + 0.3f, m_Position.y, m_Position.z + 1.0f });
			} else {
				_newnumber->SetExplain({ m_Position.x - 0.3f, m_Position.y, m_Position.z + 1.0f });
			}
			_newnumber->Initialize();
			_newnumber->SetNumber(l_DightDamage[i]);
			_damagenumber.push_back(std::move(_newnumber));
		}
	}
}
void InterEnemy::BirthHealNumber(const float heal) {
	int l_InterHeal = {};//int変換したダメージ
	l_InterHeal = (int)heal;
	unique_ptr<DrawHealNumber> _newnumber = make_unique<DrawHealNumber>();
	_newnumber->GetCameraData();
	_newnumber->SetExplain({ m_Position.x, m_Position.y, m_Position.z + 1.0f });
	_newnumber->Initialize();
	_newnumber->SetNumber(l_InterHeal);
	_healnumber.push_back(std::move(_newnumber));
}
//ダメージ関係
void InterEnemy::DamageUpdate() {
	const int l_TargetTimer = 80;
	if (!m_Damege) { return; }
	if (Helper::CheckMin(m_DamageTimer, l_TargetTimer, 1)) {
		m_DamageTimer = {};
		m_FlashCount = {};
		m_Damege = false;
	}

	//一定フレームのときは見えないようにする
	if (m_DamageTimer % 10 == 0) {
		m_FlashCount++;
	}

	if (m_FlashCount % 2 != 0) {
		m_Color.w = 1.0f;
	} else {
		m_Color.w = 0.0f;
	}
}

//死んだときの動き
void InterEnemy::DeathUpdate() {
	if (m_HP != 0.0f) { return; }

	const float l_AddFrame = 0.05f;
	float RotPower = 5.0f;
	m_Color.w = 1.0f;

	if (Helper::FrameCheck(m_OverFrame, l_AddFrame)) {		//最初はイージングで回す
		m_OverFrame = 1.0f;
		m_Alive = false;
	} else {
		RotPower = Ease(In, Cubic, m_OverFrame, RotPower, 20.0f);
		m_Rotation.y += RotPower;
		m_Position.y = Ease(In, Cubic, m_OverFrame, m_Position.y, 0.5f);

		m_Scale = { Ease(In,Cubic,m_OverFrame,m_Scale.x,0.0f),
			Ease(In,Cubic,m_OverFrame,m_Scale.y,0.0f),
		Ease(In,Cubic,m_OverFrame,m_Scale.z,0.0f) };
	}

	Obj_SetParam();
}