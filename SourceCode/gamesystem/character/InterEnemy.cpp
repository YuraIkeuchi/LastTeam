﻿#include "InterEnemy.h"
#include "Collision.h"
#include <Helper.h>
#include <Player.h>
#include <StagePanel.h>
#include <GameStateManager.h>
#include <ParticleEmitter.h>
#include <TutorialTask.h>
#include "ImageManager.h"
#include <LastBossState.h>
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
	//描画する数字と座標をここでセットする
	_drawnumber[FIRST_DIGHT]->SetExplain({ m_Position.x + 0.35f, m_Position.y, m_Position.z - 0.55f });
	_drawnumber[SECOND_DIGHT]->SetExplain({ m_Position.x, m_Position.y, m_Position.z - 0.55f });
	_drawnumber[THIRD_DIGHT]->SetExplain({ m_Position.x - 0.35f, m_Position.y, m_Position.z - 0.55f });

	poisonState = IKESprite::Create(ImageManager::POIZONCOVER, { 0.0f,0.0f });
	poisonState->SetSize({ 32.f,32.f });
	poisonState->SetAnchorPoint({ 0.5f,0.5f });
	for (auto i = 0; i < _drawPoisonnumber.size(); i++) {
		_drawPoisonnumber[i] = make_unique<DrawPoisonNumber>(0.5f);
		_drawPoisonnumber[i]->Initialize();
		_drawPoisonnumber[i]->SetPosition({ -100.f,0.f });
	}

	poison_tex = std::make_unique<IKETexture>(ImageManager::POISON_EFFECT, XMFLOAT3{}, XMFLOAT3{ 1.f,1.f,1.f }, XMFLOAT4{ 1.f,0.5f,1.f,1.f });
	poison_tex->TextureCreate();
	poison_tex->Initialize();
	poison_tex->SetRotation({ 90.0f,0.0f,0.0f });
	healdamage_tex = std::make_unique<IKETexture>(ImageManager::HEAL_DAMAGE, XMFLOAT3{}, XMFLOAT3{ 1.f,1.f,1.f }, XMFLOAT4{ 1.f,1.f,1.f,1.f });
	healdamage_tex->TextureCreate();
	healdamage_tex->Initialize();
	healdamage_tex->SetRotation({ 90.0f,0.0f,0.0f });

	counter_tex = std::make_unique<IKETexture>(ImageManager::COUNTER, XMFLOAT3{}, XMFLOAT3{ 1.f,1.f,1.f }, XMFLOAT4{ 1.f,1.f,1.f,1.f });
	counter_tex->TextureCreate();
	counter_tex->Initialize();
	counter_tex->SetIsBillboard(true);
	//counter_tex->SetRotation({ 45.0f,0.0f,0.0f });
	counter2Tex = std::make_unique<IKETexture>(ImageManager::COUNTER_TWO, XMFLOAT3{}, XMFLOAT3{ 1.f,1.f,1.f }, XMFLOAT4{ 1.f,1.f,1.f,1.f });
	counter2Tex->TextureCreate();
	counter2Tex->Initialize();
	counter2Tex->SetIsBillboard(true);
	m_AddPoisonToken = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/EnemyCommon.csv", "ADD_TOKEN")));
	m_PoisonTimerMax = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/EnemyCommon.csv", "TIMER_MAX")));

	bomTex = std::make_unique<IKETexture>(ImageManager::COUNTER_TWO, XMFLOAT3{}, XMFLOAT3{ 1.f,1.f,1.f }, XMFLOAT4{ 1.f,0.6f,0.f,1.f });
	bomTex->TextureCreate();
	bomTex->Initialize();
	bomTex->SetRotation({ 90.0f,0.0f,0.0f });

	bom2Tex = std::make_unique<IKETexture>(ImageManager::BOM2, XMFLOAT3{}, XMFLOAT3{ 1.f,1.f,1.f }, XMFLOAT4{ 1.f,1.f,1.f,1.f });
	bom2Tex->TextureCreate();
	bom2Tex->Initialize();
	bom2Tex->SetRotation({ 60.0f,0.0f,0.0f });
	bom2Tex->SetIsBillboard(true);


	reloadTex = std::make_unique<IKETexture>(ImageManager::RELOADEFF, XMFLOAT3{}, XMFLOAT3{ 1.f,1.f,1.f }, XMFLOAT4{ 1.f,0.6f,0.f,1.f });
	reloadTex->TextureCreate();
	reloadTex->Initialize();
	reloadTex->SetRotation({ 90.0f,0.0f,0.0f });

	reload2Tex = std::make_unique<IKETexture>(ImageManager::BOM2, XMFLOAT3{}, XMFLOAT3{ 1.f,1.f,1.f }, XMFLOAT4{ 1.f,1.f,1.f,1.f });
	reload2Tex->TextureCreate();
	reload2Tex->Initialize();
	reload2Tex->SetRotation({ 60.0f,0.0f,0.0f });
	reload2Tex->SetIsBillboard(true);

	canCounterTex.reset(new IKETexture(ImageManager::CHARGE, m_Position, { 1.f,1.f,1.f }, { 0.9f,0.9f,0.9f,1.f }));
	canCounterTex->TextureCreate();
	canCounterTex->Initialize();
	canCounterTex->SetPosition({ m_Position.x,0.02f,m_Position.z });
	canCounterTex->SetRotation({ 90.0f,0.0f,0.0f });
}
void InterEnemy::SkipInitialize() {
	m_AddDisolve = 0.0f;
}
//更新
void InterEnemy::Update() {
	if (!GameStateManager::GetInstance()->GetGameStart()) { return; }
	if (GameStateManager::GetInstance()->GetBossCamera()) { return; }
	if (m_EnemyTag != "Bomb") {
		if (m_HP != 0.0f) {
			if (!m_Death)
				Action();
			//だめーじ関係
			DamageUpdate();
		}
	} else {
		//だめーじ関係
		DamageUpdate();
		Action();
	}

	if (m_HP <= 0.0f && m_EnemyTag != "Bomb") {
		m_Death = true;
		DeathUpdate();
	}


	const int l_BasePanelCount = 4;
	Helper::CheckMax(m_DamegeTimer, 0, -1);
	//表示用のHP
	Helper::Clamp(m_HP, 0.0f, m_MaxHP);

	m_InterHP = (int)(m_HP);
	if (m_EnemyTag != "Bomb") {
		Helper::Clamp(m_InterHP, 1, (int)m_MaxHP);
	} else {
		Helper::Clamp(m_InterHP, 0, (int)m_MaxHP);
	}
	////敵のマスを取得する
	if (m_Position.y <= 0.3f) {
		StagePanel::GetInstance()->SetEnemyHit(m_Object.get(), m_NowWidth, m_NowHeight, m_Alive);
	}
	if (m_HP != 0.0f) {
		for (auto i = 0; i < _drawnumber.size(); i++) {
			_drawnumber[i]->Update();
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
	HealDamageEffect();
	SuperPoisonEffect();
	RegeneUpdate();

	//数値化したHP
	HPManage();
	//UIをワールド座標に変換する
	if (m_EnemyTag != "Rock") {
		WorldDivision({ m_Position.x - 0.45f, m_Position.y, m_Position.z - 0.25f });
		hptex->SetPosition(m_HPPos);
		hptex->SetSize({ HpPercent() * m_HPSize.x,m_HPSize.y });
	}
	CounterUpdate();
	BomUpdate();
	ReLoadUpdate();
	rotCounter.y+=6.0f;
	if (rotCounter.y >= 360.f) {
		rotCounter.y = 0.f;
	}
	canCounterTex->SetColor({ 1.f,1.f,0.55f,1.f});
	canCounterTex->SetRotation(rotCounter);
	canCounterTex->SetScale(scaleCounter);
	canCounterTex->SetPosition({ m_Position.x,0.02f,m_Position.z });
	canCounterTex->Update();
}
//バトル前の更新
void InterEnemy::AwakeUpdate() {
	if (!StagePanel::GetInstance()->GetCreateFinish()) { return; }
	const float l_AddDisolve = 0.05f;
	//ディゾルブを解除する
	if (Helper::CheckMax(m_AddDisolve, 0.0f, -l_AddDisolve)) {
		GameStateManager::GetInstance()->SetGameStart(true);
	}

	Obj_SetParam();
}
void InterEnemy::HealDamageEffect() {
	if (!m_HealDamage) { return; }
	if (Helper::FrameCheck(m_HealFrame, 1 / 30.f)) {
		m_HealDamage = false;
		m_HealFrame = 0.f;
	} else {
		XMFLOAT3 scale = {
			Ease(Out,Back,m_HealFrame,0.f,0.5f),
			Ease(Out,Back,m_HealFrame,0.f,0.5f),
			Ease(Out,Back,m_HealFrame,0.f,0.5f)
		};
		float alpha = Ease(In, Quint, m_HealFrame, 1.f, 0.f);
		float posY = Ease(Out, Cubic, m_HealFrame, 0.5f, 2.5f);
		healdamage_tex->SetScale(scale);
		healdamage_tex->SetColor(XMFLOAT4{ 1.f,1.f,1.f,alpha });
		healdamage_tex->SetPosition({ m_Position.x,posY,m_Position.z });
		healdamage_tex->Update();
	}
}
void InterEnemy::SuperPoisonEffect() {
	if (!m_SuperPoison) { return; }
	if (Helper::FrameCheck(m_poisonFrame, 1 / 30.f)) {
		m_SuperPoison = false;
		m_poisonFrame = 0.f;
	} else {
		XMFLOAT3 scale = {
			Ease(Out,Back,m_poisonFrame,0.f,0.5f),
			Ease(Out,Back,m_poisonFrame,0.f,0.5f),
			Ease(Out,Back,m_poisonFrame,0.f,0.5f)
		};
		float alpha = Ease(In, Quint, m_poisonFrame, 1.f, 0.f);
		float posY = Ease(Out, Cubic, m_poisonFrame, 0.5f, 2.5f);
		poison_tex->SetScale(scale);
		poison_tex->SetColor(XMFLOAT4{ 1.f,0.5f,1.f,alpha });
		poison_tex->SetPosition({ m_Position.x,posY,m_Position.z });
		poison_tex->Update();
	}
}
void InterEnemy::BaseFrontDraw(DirectXCommon* dxCommon) {
	if (m_SuperPoison) { poison_tex->Draw(); }
	if (m_HealDamage) { healdamage_tex->Draw(); }

}
//描画
void InterEnemy::Draw(DirectXCommon* dxCommon) {
}

void InterEnemy::BaseBackDraw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	reload2Tex->Draw();
	reloadTex->Draw();

	bomTex->Draw();
	bom2Tex->Draw();

	counter_tex->Draw();
	counter2Tex->Draw();
	if (m_CanCounter&&!m_Death) {
		canCounterTex->Draw();
	}
	IKETexture::PostDraw();

}

void InterEnemy::ImGuiDraw() {
	//ImGui::Begin("Enemy");
	//ImGui::Text("HP:%f", m_Position.y);
	//ImGui::End();
	//ImGui_Origin();
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
	if (!m_Death) {
		//HPバー
		hptex->Draw();
		//HP(数字)
		if (m_InterHP > 0) {
			_drawnumber[FIRST_DIGHT]->Draw();
		}
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
	if (m_Poison && m_EnemyTag != "Rock") {
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
	if (m_Scale.x <= 0.15f) { return; }
	if (area.empty()) { return; }

	for (unique_ptr<AttackArea>& _area : area) {
		if ((_area->GetNowHeight() == m_NowHeight && _area->GetNowWidth() == m_NowWidth) && (_area->GetAttack()) &&
			!_area->GetHit() && _area->GetName() == "Player") {
			float damage = _area->GetDamage();
			//固定ダメージか否か
			if (!_area->GetIsFixed()) {
				if (_area->GetBuff()) {
					damage *= 2.0f;
				}
				if (m_CanCounter &&
					!GameStateManager::GetInstance()->GetCounter()) {

					GameStateManager::GetInstance()->SetCounter(true);
					isCounterEffect = true;
					m_CounterFrame = 0.f;
					m_CounterFinishFrame = 0.f;
					effectPos = m_Position;
					damage *= 1.5f;
					TutorialTask::GetInstance()->SetTaskFinish(true, TASK_COUNTER);
				}
				if (GameStateManager::GetInstance()->GetIsFivePower()) {
					damage *= 1.2f;
					GameStateManager::GetInstance()->SetPassiveActive((int)Passive::ABILITY::FIVE_POWER);
				}
				if (_area->GetIsKnight()) {
					damage *= 1.25f;
					GameStateManager::GetInstance()->SetPassiveActive((int)Passive::ABILITY::EXTEND_KNIGHT);
				}

				if (GameStateManager::GetInstance()->GetTakenDamageUp()) {
					float up = (float)GameStateManager::GetInstance()->GetTakenDamageNum() * 1.5f;
					if (up >= 1.0f) {
						damage += up;
						GameStateManager::GetInstance()->SetPassiveActive((int)Passive::ABILITY::TAKENDAMAGEUP);
					}
				}
			} else {
				if (m_CanCounter &&
					!GameStateManager::GetInstance()->GetCounter()) {
					GameStateManager::GetInstance()->SetCounter(true);
				}

			}
			m_Damege = true;
			m_DamageTimer = {};
			//ラスボス周りの敵はカウンターをする
			if (m_BombCounter) {
				m_BirthBomb = true;
			}
			Helper::Clamp(damage, 0.0f, 999.0f);
			if (m_EnemyTag == "Mob" && !TutorialTask::GetInstance()->GetTaskFinish(TASK_COUNTER)) {
				damage = 0.0f;
			}
			if (m_EnemyTag == "LASTBOSS" && LastBossState::GetInstance()->GetBossShield()) {
				m_DamageCut = true;
				damage /= 2.0f;
			}
			m_HP -= damage;
			GameStateManager::GetInstance()->DamageCheck((int)damage);
			BirthDamage(damage);
			std::string name = _area->GetStateName();
			if (name == "BOM" && m_HP <= 0.0f) {
				GameStateManager::GetInstance()->SetIsBomSuccess(true);
			} else	if (name == "DRAIN") {
				float rate = 0.2f;
				player->SetHealPower(damage * rate);		//HP回復
			} else if (name == "POISON") {		//毒
				m_Poison = true;
				if (!m_IsVenom) {
					m_PoisonToken += _area->GetPoisonToken();
				} else {
					GameStateManager::GetInstance()->SetPassiveActive((int)Passive::ABILITY::POISON_DAMAGEUP);
					m_SuperPoison = true;
					m_PoisonToken += _area->GetPoisonToken() * 2;
				}
			} else if (name == "VENOM") {		//ヴェノム効果
				if (m_Poison) {
					m_PoisonToken *= 3;
				}
			} else if (name == "PASSIVEPOISON") {		//毒
				m_Poison = true;
				if (!m_IsVenom) {
					m_PoisonToken += _area->GetPoisonToken();
				} else {
					GameStateManager::GetInstance()->SetPassiveActive((int)Passive::ABILITY::POISON_DAMAGEUP);
					m_SuperPoison = true;
					m_PoisonToken += _area->GetPoisonToken() * 2;
				}
			} else if (name == "FAR" && !m_Induction && m_NowWidth != PANEL_WIDTH - 1 && _charaState != STATE_ATTACK) {		//敵を吹き飛ばす
				m_Induction = true;
				m_InductionFrame = {};
				const int l_NowWidth = m_NowWidth;
				for (int i = l_NowWidth; i < PANEL_WIDTH; i++) {
					if (m_NowWidth == i) { continue; }
					if (StagePanel::GetInstance()->GetisEnemyHit(i, m_NowHeight)) {

						m_InductionPos = StagePanel::GetInstance()->GetPosition(i - 1, m_NowHeight).x;
						break;
					} else {
						if (i == PANEL_WIDTH - 1) {

							m_InductionPos = StagePanel::GetInstance()->GetPosition(PANEL_WIDTH - 1, m_NowHeight).x;
							break;
						}
					}
				}
			} else if (name == "NEAR" && !m_Induction && m_NowWidth != PANEL_WIDTH / 2 && _charaState != STATE_ATTACK) {		//敵を引き寄せる
				m_Induction = true;
				m_InductionFrame = {};
				const int l_NowWidth = m_NowWidth;
				for (int i = l_NowWidth; i >= (PANEL_WIDTH / 2); i--) {

					if (m_NowWidth == i) { continue; }
					if (StagePanel::GetInstance()->GetisEnemyHit(i, m_NowHeight)) {
						m_InductionPos = StagePanel::GetInstance()->GetPosition(i + 1, m_NowHeight).x;
						break;
					} else {
						if (i == PANEL_WIDTH / 2) {
							m_InductionPos = StagePanel::GetInstance()->GetPosition(PANEL_WIDTH / 2, m_NowHeight).x;
							break;
						}
					}
				}
			}

			if (GameStateManager::GetInstance()->GetAttackedPoison()) {
				GameStateManager::GetInstance()->SetPassiveActive((int)Passive::ABILITY::ATTACK_POISON);
				m_Poison = true;
				m_PoisonToken += 3;
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
		}
	}
}
void InterEnemy::SimpleDamege(float damage, bool isLimit) {
	if (m_HP <= 0.0f) { return; }
	if (isLimit) {
		float hp = m_HP;
		hp -= damage;
		if (hp < 1.0f) {
			damage -= 1.0f;
		}
		Helper::Clamp(hp, 1.0f, m_HP);
		m_HP = hp;
	} else {
		m_HP -= damage;
	}
	BirthDamage(damage);
	BirthParticle();
}

void InterEnemy::SimpleHeal(const bool Regene) {
	if (m_HP <= 0.0f) { return; }
	if (m_EnemyTag == "SUPPORT2") { return; }
	float heal = {};

	if (Regene) {
		heal = 20.0f;
	} else {
		if (m_EnemyTag == "LASTBOSS") {
			heal = 150.0f;
		} else {
			heal = 50.0f;
		}
	}
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

void InterEnemy::SimplePosion(int poison) {
	m_Poison = true;
	m_PoisonToken += poison;

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
void InterEnemy::WorldDivision(XMVECTOR tex2DPos) {
	const float l_BasePos = 0.1f;
	float l_AddPosX = {};
	float l_AddPosZ = {};

	if (m_NowHeight == 3) {
		l_AddPosZ = 0.71f;
		l_AddPosX = 0.35f;
	}
	else if (m_NowHeight == 2) {
		l_AddPosZ = 0.65f;
		l_AddPosX = 0.32f;
	}
	else if (m_NowHeight == 1) {
		l_AddPosZ = 0.62f;
		l_AddPosX = 0.3f;
	}
	else {
		l_AddPosZ = 0.55f;
		l_AddPosX = 0.28f;
	}
	Camera* camera = Helper::GetCamera();
	m_MatView = camera->GetViewMatrix();
	m_MatProjection = camera->GetProjectionMatrix();
	m_MatPort = camera->GetViewPort();
	//HPバー
	
	tex2DPos = Helper::PosDivi(tex2DPos, m_MatView, false);
	tex2DPos = Helper::PosDivi(tex2DPos, m_MatProjection, true);
	tex2DPos = Helper::WDivision(tex2DPos, false);
	tex2DPos = Helper::PosDivi(tex2DPos, m_MatPort, false);

	m_HPPos = { tex2DPos.m128_f32[0],tex2DPos.m128_f32[1] };

	//描画する数字と座標をここでセットする
	_drawnumber[FIRST_DIGHT]->SetExplain({ (m_Position.x - l_BasePos) + l_AddPosX, m_Position.y, m_Position.z- l_AddPosZ });
	_drawnumber[SECOND_DIGHT]->SetExplain({ (m_Position.x - l_BasePos), m_Position.y, m_Position.z- l_AddPosZ });
	_drawnumber[THIRD_DIGHT]->SetExplain({ (m_Position.x - l_BasePos) - l_AddPosX, m_Position.y, m_Position.z- l_AddPosZ });
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
	for (auto i = 0; i < _drawPoisonnumber.size(); i++) {
		_drawPoisonnumber[i]->GetCameraData();
		_drawPoisonnumber[i]->SetNumber(m_PoisonTokenNum[i]);
		_drawPoisonnumber[i]->Update();
	}
	if (m_Poison) {
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
	m_PoisonTimer++;
	if (m_PoisonTimer % m_PoisonTimerMax == 0) {	//一定フレームで1ずつ減らす
		m_HP -= m_PoisonToken;
		BirthDamage((float)m_PoisonToken);
		BirthPoisonParticle();
		if (m_PoisonLong) {
			if (m_PoisonToken != 1) {
				float poisons = (float)m_PoisonToken;
				poisons *= 0.75f;
				m_PoisonToken = (int)poisons;
			} else {
				m_PoisonToken /= 2;
			}
		} else {
			if (m_PoisonToken % 2 == 0) {
				m_PoisonToken /= 2;
			} else {
				if (m_PoisonToken != 1) {
					m_PoisonToken++;
					m_PoisonToken /= 2;
				} else {
					m_PoisonToken /= 2;
				}
			}
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
	} else if (l_InterDamage >= 10 && l_InterDamage < 100) {
		int l_DightDamage[DAMAGE_MAX - 1];
		for (auto i = 0; i < DAMAGE_MAX - 1; i++) {
			l_DightDamage[i] = Helper::getDigits(l_InterDamage, i, i);
			unique_ptr<DrawDamageNumber> _newnumber = make_unique<DrawDamageNumber>();
			_newnumber->GetCameraData();
			if (i == 0) {
				_newnumber->SetExplain({ m_Position.x + 0.4f, m_Position.y, m_Position.z + 1.0f });
			} else {
				_newnumber->SetExplain({ m_Position.x - 0.4f, m_Position.y, m_Position.z + 1.0f });
			}
			_newnumber->Initialize();
			_newnumber->SetNumber(l_DightDamage[i]);
			_damagenumber.push_back(std::move(_newnumber));
		}
	} else {
		int l_DightDamage[DAMAGE_MAX];
		for (auto i = 0; i < DAMAGE_MAX; i++) {
			l_DightDamage[i] = Helper::getDigits(l_InterDamage, i, i);
			unique_ptr<DrawDamageNumber> _newnumber = make_unique<DrawDamageNumber>();
			_newnumber->GetCameraData();
			if (i == 0) {
				_newnumber->SetExplain({ m_Position.x + 0.6f, m_Position.y, m_Position.z + 1.0f });
			} else if (i == 1) {
				_newnumber->SetExplain({ m_Position.x, m_Position.y, m_Position.z + 1.0f });
			} else {
				_newnumber->SetExplain({ m_Position.x - 0.6f, m_Position.y, m_Position.z + 1.0f });
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
	if (l_InterHeal < 10) {

		unique_ptr<DrawHealNumber> _newnumber = make_unique<DrawHealNumber>();
		_newnumber->GetCameraData();
		_newnumber->SetExplain({ m_Position.x, m_Position.y, m_Position.z + 1.0f });
		_newnumber->Initialize();
		_newnumber->SetNumber(l_InterHeal);
		_healnumber.push_back(std::move(_newnumber));
	} else if (l_InterHeal >= 10 && l_InterHeal < 100) {
		int l_DightDamage[HEAL_MAX - 1];
		for (auto i = 0; i < HEAL_MAX - 1; i++) {
			l_DightDamage[i] = Helper::getDigits(l_InterHeal, i, i);
			unique_ptr<DrawHealNumber> _newnumber = make_unique<DrawHealNumber>();
			_newnumber->GetCameraData();
			if (i == 0) {
				_newnumber->SetExplain({ m_Position.x + 0.3f, m_Position.y, m_Position.z + 1.0f });
			} else {
				_newnumber->SetExplain({ m_Position.x - 0.3f, m_Position.y, m_Position.z + 1.0f });
			}
			_newnumber->Initialize();
			_newnumber->SetNumber(l_DightDamage[i]);
			_healnumber.push_back(std::move(_newnumber));
		}
	} else {
		int l_DightDamage[HEAL_MAX];
		for (auto i = 0; i < HEAL_MAX; i++) {
			l_DightDamage[i] = Helper::getDigits(l_InterHeal, i, i);
			unique_ptr<DrawHealNumber> _newnumber = make_unique<DrawHealNumber>();
			_newnumber->GetCameraData();
			if (i == 0) {
				_newnumber->SetExplain({ m_Position.x + 0.6f, m_Position.y, m_Position.z + 1.0f });
			} else if (i == 1) {
				_newnumber->SetExplain({ m_Position.x, m_Position.y, m_Position.z + 1.0f });
			} else {
				_newnumber->SetExplain({ m_Position.x - 0.6f, m_Position.y, m_Position.z + 1.0f });
			}
			_newnumber->Initialize();
			_newnumber->SetNumber(l_DightDamage[i]);
			_healnumber.push_back(std::move(_newnumber));
		}
	}
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

	if (m_EnemyTag == "Rock") {
		StagePanel::GetInstance()->SetClose(m_NowWidth, m_NowHeight, false);
		StagePanel::GetInstance()->SetRock(m_NowWidth, m_NowHeight, false);
		m_RockAddPower -= m_Gravity;
		m_Scale = Helper::Float3AddFloat(m_Scale, 0.04f);
		Helper::CheckMax(m_Position.y, 0.0f, m_RockAddPower);
		if (Helper::CheckMin(m_AddDisolve, 2.5f, 0.1f)) {
			m_Alive = false;
		}
	} else {
		if (Helper::FrameCheck(m_OverFrame, l_AddFrame)) {		//最初はイージングで回す
			m_OverFrame = 1.0f;
			if (m_Death) {
				if (m_DeathTimer == 0) {
					DeathParticle();
				}
				if (Helper::CheckMin(m_DeathTimer, 20, 1)) {
					if (m_EnemyTag == "SUPPORT" || m_EnemyTag == "SUPPORT2") {
						GameStateManager::GetInstance()->SetIsHeal(true);
						DeathSpecial();
					} else if (m_EnemyTag == "CLOSER" || m_EnemyTag == "LASTBOSS") {
						DeathSpecial();
					}
					m_Alive = false;
				}
			}
		} else {
			RotPower = Ease(In, Cubic, m_OverFrame, RotPower, 20.0f);
			m_Rotation.y += RotPower;
			m_Position.y = Ease(In, Cubic, m_OverFrame, m_Position.y, 0.5f);

			m_Scale = { Ease(In,Cubic,m_OverFrame,m_Scale.x,0.0f),
				Ease(In,Cubic,m_OverFrame,m_Scale.y,0.0f),
			Ease(In,Cubic,m_OverFrame,m_Scale.z,0.0f) };
		}
	}
	Obj_SetParam();
}
//リジュネ回復
void InterEnemy::RegeneUpdate() {
	if (StagePanel::GetInstance()->GetHeal(m_NowWidth, m_NowHeight)) {
		if (Helper::CheckMin(m_HealTimer, 100, 1)) {
			SimpleHeal(true);
			m_HealTimer = {};
		}
	} else {
		m_HealTimer = {};
	}
}
//誘導された動き
void InterEnemy::InductionMove() {
	const float l_AddFrame = 1 / 10.0f;

	if (Helper::FrameCheck(m_InductionFrame, l_AddFrame)) {
		m_Induction = false;
		m_InductionFrame = {};
		StagePanel::GetInstance()->EnemyHitReset();
	} else {
		m_Position.x = Ease(In, Cubic, m_InductionFrame, m_Position.x, m_InductionPos);
	}
}

void InterEnemy::CounterUpdate() {
	if (!isCounterEffect) { return; }
	if (!Helper::FrameCheck(m_CounterFrame, 1 / 40.f)) {
		XMFLOAT3 scale = {
			Ease(InOut,Back,m_CounterFrame,0.f,0.4f),
			Ease(InOut,Back,m_CounterFrame,0.f,0.4f),
			Ease(InOut,Back,m_CounterFrame,0.f,0.4f)
		};
		counter_tex->SetScale(scale);
		float alpha = Ease(In, Sine, m_CounterFrame, 1.f, 0.f);
		counter_tex->SetColor(XMFLOAT4{ 1.f,1.f,1.f,alpha });
		counter_tex->Update();

		counter_tex->SetPosition({ effectPos.x,effectPos.y + 0.5f,effectPos.z });
		counter_tex->Update();

		XMFLOAT3 scale2 = {
		Ease(Out,Back,m_CounterFrame,0.f,-0.30f),
		Ease(Out,Back,m_CounterFrame,0.f,-0.30f),
		Ease(Out,Back,m_CounterFrame,0.f,-0.30f)
		};
		counter2Tex->SetScale(scale2);
		float rot_ = Ease(Out, Quint, m_CounterFrame, 0.f, 180.0f);
		counter2Tex->SetRotation({ 0.f,0.f,rot_ });
		counter2Tex->SetPosition({ effectPos.x,effectPos.y + 0.5f,effectPos.z });
		counter2Tex->Update();
	}
	if (m_CounterFrame >= 0.7f) {
		if (Helper::FrameCheck(m_CounterFinishFrame, 1 / 20.f)) {
			isCounterEffect = false;
		} else {
			float alpha = Ease(InOut, Sine, m_CounterFinishFrame, 1.f, 0.f);
			counter2Tex->SetColor(XMFLOAT4{ 0.8f,0.8f,0.8f,alpha });
			counter2Tex->Update();
		}
	}
}
void InterEnemy::BomUpdate() {
	if (!m_BomEffect) { return; }
	if (!Helper::FrameCheck(m_BomFrame, 1 / 20.f)) {
		XMFLOAT3 scale2 = {
		Ease(Out,Back,m_BomFrame,0.f,0.4f),
		Ease(Out,Back,m_BomFrame,0.f,0.4f),
		Ease(Out,Back,m_BomFrame,0.f,0.4f)
		};
		bom2Tex->SetScale(scale2);
		bom2Tex->SetPosition({ effect2Pos.x,effect2Pos.y + 0.5f,effect2Pos.z });

		XMFLOAT3 scale = {
		Ease(Out,Back,m_BomFrame,0.f,0.4f),
		Ease(Out,Back,m_BomFrame,0.f,0.4f),
		Ease(Out,Back,m_BomFrame,0.f,0.4f)
		};
		bomTex->SetScale(scale);
		float rot2 = Ease(In, Back, m_BomFrame, 0.f, -45.0f);
		bomTex->SetRotation({ 60.f,0.f,rot2 });
		float alpha = Ease(In, Exp, m_BomFrame, 1.f, 0.0f);
		bomTex->SetColor({ 0.f,0.f,0.f,alpha });
		bomTex->SetPosition({ effect2Pos.x,effect2Pos.y + 0.5f,effect2Pos.z });
		bomTex->Update();

		bom2Tex->SetColor({ 1,1,1,alpha });
		bom2Tex->Update();
	} else {
		m_BomEffect = false;
	}
}


void InterEnemy::BomStart() {
	m_BomEffect = true;
	m_BomFrame = 0.f;
	effect2Pos = m_Position;

}
void InterEnemy::ReLoadUpdate() {
	if (!m_ReloadEffect) { return; }
	if (!Helper::FrameCheck(m_ReloadFrame, 1 / 20.f)) {
		XMFLOAT3 scale2 = {
		(0.4f),
		(0.4f),
		(0.4f)
		};
		reload2Tex->SetScale(scale2);
		reload2Tex->SetPosition({ effect3Pos.x,effect3Pos.y + 0.5f,effect3Pos.z });

		XMFLOAT3 scale = {
		Ease(Out,Back,m_ReloadFrame,0.f,0.25f),
		Ease(Out,Back,m_ReloadFrame,0.f,0.25f),
		Ease(Out,Back,m_ReloadFrame,0.f,0.25f)
		};
		reloadTex->SetScale(scale);
		float alpha = Ease(In, Exp, m_ReloadFrame, 1.f, 0.0f);
		reloadTex->SetColor({ 1.f,1.f,1.f,alpha });
		reloadTex->SetPosition({ effect3Pos.x,effect3Pos.y + 0.5f,effect3Pos.z });
		reloadTex->Update();

		reload2Tex->SetColor({ 1,1,1,alpha });
		reload2Tex->Update();
	} else {
		m_ReloadEffect = false;
	}


}
void InterEnemy::ReLoadStart() {
	m_ReloadEffect = true;
	m_ReloadFrame = 0.f;
	effect3Pos = m_Position;

}
//死亡時パーティクル
void InterEnemy::DeathParticle() {
	int l_life = 20;
	const float s_Scale = 1.5f;
	const float e_Scale = 0.5f;
	const XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	for (int i = 1; i < 7; i++) {
		ParticleEmitter::GetInstance()->DeathEffect(l_life, m_Position, s_Scale, e_Scale, color, color, i);
	}
}
//クリアシーンの更新
void InterEnemy::ClearUpdate() {
	m_Rotation.y = 180.0f;
	ClearAction();
	//Obj_SetParam();
}

//ゲームオーバーシーンの更新
void InterEnemy::GameOverUpdate() {
	GameOverAction();
}