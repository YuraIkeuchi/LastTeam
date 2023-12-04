#include "Player.h"
#include "CsvLoader.h"
#include <Helper.h>
#include "Input.h"
#include "Audio.h"
#include <GameStateManager.h>
#include <StagePanel.h>
#include <ImageManager.h>
#include <ParticleEmitter.h>
#include "imgui.h"

float Player::startHP = 0.f;
//リソース読み込み
void Player::LoadResource() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	m_Object->SetPosition({ 0.0f,PANEL_SIZE,0.0f });

	//HPII
	hptex = IKESprite::Create(ImageManager::ENEMYHPUI, { 0.0f,0.0f });
	hptex->SetColor({ 0.5f,1.0f,0.5f,1.0f });

	hptex_under = IKESprite::Create(ImageManager::FEED, { 0.0f,0.0f });
	hptex_under->SetSize(m_HPSize);
	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i] = make_unique<DrawNumber>(0.5f);
		_drawnumber[i]->Initialize();
		_MaxHp[i] = make_unique<DrawNumber>(0.5f);
		_MaxHp[i]->Initialize();
	}

	_drawnumber[FIRST_DIGHT]->SetPosition({ m_HPPos.x + 80.0f,m_HPPos.y + 20.0f });
	_drawnumber[SECOND_DIGHT]->SetPosition({ m_HPPos.x + 60.0f,m_HPPos.y + 20.0f });
	_drawnumber[THIRD_DIGHT]->SetPosition({ m_HPPos.x + 40.f, m_HPPos.y + 20.0f });
	slash_ = IKESprite::Create(ImageManager::SLASH, { m_HPPos.x + 100.f,m_HPPos.y + 20.0f });
	slash_->SetScale(0.5f);
	slash_->SetAnchorPoint({ 0.5f,0.5f });
	_MaxHp[FIRST_DIGHT]->SetPosition({ m_HPPos.x + 160.0f,m_HPPos.y + 20.0f });
	_MaxHp[SECOND_DIGHT]->SetPosition({ m_HPPos.x + 140.0f,m_HPPos.y + 20.0f });
	_MaxHp[THIRD_DIGHT]->SetPosition({ m_HPPos.x + 120.f, m_HPPos.y + 20.0f });


	shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });
	shadow_tex->SetColor({ 1.0f,0.0f,0.0f,1.0f });
}
//初期化
bool Player::Initialize() {

	LoadCSV();
	m_ShadowScale = { 0.05f,0.05f,0.05f };
	//CSV読み込み
	return true;
}
//CSV読み込み
void Player::LoadCSV() {
	if (is_title) {
		m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "STARTHP")));
		startHP = m_HP;
		m_MaxHP = m_HP;
		m_OldHP = m_HP;
	} else {
		m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "NOWHP")));
		m_MaxHP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "MAXHP")));
	}
}
//ステータスの初期化
void Player::InitState(const XMFLOAT3& pos) {
	m_Position = pos;
	m_AfterPos = m_Position;
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_BaseScale = 0.5f;
	_charaState = STATE_MOVE;
	//イージング
	m_Frame = {};
	m_CoolTime = {};
	m_NowHeight = {};
	m_NowWidth = {};
	//数値化したHP表示のための変数
	for (auto i = 0; i < _drawnumber.size(); i++) {
		m_DigitNumber[i] = {};
		m_DigitNumberMax[i] = {};
	}
	m_InterMaxHP = {};//整数にしたHP
	m_InterHP = {};//整数にしたHP

	GameStateManager::GetInstance()->PlayerNowPanel(m_NowWidth, m_NowHeight);
}
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (Player::* Player::stateTable[])() = {
	&Player::Move,//移動
	&Player::Delay,//動きが止まる
};
//更新処理
void Player::Update() {
	if (is_title) {
		TitleUpdate();
	} else {
		const float l_GrazeMax = 2.0f;
		Obj_SetParam();
		//状態移行(charastateに合わせる)
		(this->*stateTable[_charaState])();
		BirthParticle();
		//プレイヤーのマスを取得する
		StagePanel::GetInstance()->SetPanelSearch(m_Object.get(), m_NowWidth, m_NowHeight);
		Obj_SetParam();
		BirthParticle();
		// グレイズ用にスコアを計算する
		m_Length = Helper::ChechLength(m_Position, m_GrazePos);
		m_GrazeScore = l_GrazeMax - m_Length;
		// 最大スコアは10
		Helper::Clamp(m_GrazeScore, 0.0f, l_GrazeMax);
		// プレイヤーの位置からスコアを加算する
		GameStateManager::GetInstance()->SetPosScore(GameStateManager::GetInstance()->GetPosScore() + ((float)(m_NowWidth) * 0.1f));
		GameStateManager::GetInstance()->PlayerNowPanel(m_NowWidth, m_NowHeight);

		//HPの限界値を決める
		Helper::Clamp(m_HP, 0.0f, m_MaxHP);
		//どこが初期化かわからん
		if (m_MaxHP > startHP) {
			_MaxHp[FIRST_DIGHT]->SetColor({ 0.f,1.0f,0.f,1.0f });
			_MaxHp[SECOND_DIGHT]->SetColor({ 0.f,1.0f,0.f,1.0f });
			_MaxHp[THIRD_DIGHT]->SetColor({ 0.f,1.0f,0.f,1.0f });
		}
		HPEffect();
		DamageUpdate();
		AttackMove();
		ShrinkScale();
		//表示用のHP
		m_InterHP = (int)(m_HP);
		m_InterMaxHP = (int)m_MaxHP;
		if (m_HP > 0.0f) {
			for (auto i = 0; i < _drawnumber.size(); i++) {
				//HPの限界値を決める
				Helper::Clamp(m_HP, 0.0f, m_MaxHP);
				// 表示用のHP
				m_InterHP = (int)(m_HP);
				m_InterMaxHP = (int)m_MaxHP;
				for (auto i = 0; i < _drawnumber.size(); i++) {
					_drawnumber[i]->SetNumber(m_DigitNumber[i]);
					_drawnumber[i]->Update();
					_MaxHp[i]->SetNumber(m_DigitNumberMax[i]);
					_MaxHp[i]->Update();
					m_DigitNumber[i] = Helper::getDigits(m_InterHP, i, i);
					m_DigitNumberMax[i] = Helper::getDigits(m_InterMaxHP, i, i);
				}
			}
		}
		hptex->SetPosition(m_HPPos);
		hptex_under->SetPosition(m_HPPos);
		hptex->SetSize({ HpPercent() * m_HPSize.x,m_HPSize.y });

		//手に入れたスキルのUIの更新
		for (auto i = 0; i < imageplayer.size(); i++) {
			if (imageplayer[i] == nullptr)continue;
			imageplayer[i]->Update();

			if (!imageplayer[i]->GetAlive()) {
				imageplayer.erase(cbegin(imageplayer) + i);
			}
		}

		//最後の一撃はジャンプする
		if (m_Jump) {
			m_AddPower -= m_Gravity;
			if (Helper::CheckMax(m_Position.y, 0.1f, m_AddPower)) {
				m_AddPower = {};
				m_Jump = false;
				m_Position.y = 0.1f;
			}
		}

		Helper::Clamp(m_BaseScale, 0.0f, 0.5f);
	}
	//影
	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	shadow_tex->SetPosition(m_ShadowPos);
	shadow_tex->SetScale(m_ShadowScale);
	shadow_tex->Update();

}
//描画
void Player::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	shadow_tex->Draw();
	IKETexture::PostDraw();
	//手に入れたスキルのUIの更新
	for (auto i = 0; i < imageplayer.size(); i++) {
		if (imageplayer[i] == nullptr)continue;
		imageplayer[i]->Draw(dxCommon);
	}
	if(m_Color.w != 0.0f)
	Obj_Draw();
}
//UIの描画
void Player::UIDraw() {
	IKESprite::PreDraw();
	//HPバー
	hptex_under->Draw();
	hptex->Draw();
	if (m_InterHP != 0) {
		_drawnumber[FIRST_DIGHT]->Draw();
	}
	if (m_InterHP >= 10) {
		_drawnumber[SECOND_DIGHT]->Draw();
	}
	if (m_InterHP >= 100) {
		_drawnumber[THIRD_DIGHT]->Draw();
	}
	slash_->Draw();
	if (m_InterMaxHP != 0) {
		_MaxHp[FIRST_DIGHT]->Draw();
	}
	if (m_InterMaxHP >= 10) {
		_MaxHp[SECOND_DIGHT]->Draw();
	}
	if (m_InterMaxHP >= 100) {
		_MaxHp[THIRD_DIGHT]->Draw();
	}

	IKESprite::PostDraw();
}
//ImGui
void Player::ImGuiDraw() {
	ImGui::Begin("Player");
	ImGui::Text("ScaleX:%f", m_BaseScale);
	ImGui::SliderFloat("HP", &m_HP, 0, m_MaxHP);
	ImGui::SliderFloat("Disolve", &m_AddDisolve, 0, 2);
	if (ImGui::Button("MOVE_NONE", ImVec2(100, 100)))
	{
		_MoveType = MOVE_NONE;
	}
	if (ImGui::Button("MOVE_EASE", ImVec2(100, 100)))
	{
		_MoveType = MOVE_EASE;
	}
	if (ImGui::Button("MOVE_DISOLVE", ImVec2(100, 100)))
	{
		_MoveType = MOVE_DISOLVE;
	}
	ImGui::End();
}
//移動
void Player::Move() {
	if (m_Delay) { return; }
	if (GameStateManager::GetInstance()->GetResetPredict()) { return; }
	if (!GameStateManager::GetInstance()->GetGameStart()) { return; }
	const int l_TargetTimer = 8;
	const float l_AddVelocity = PANEL_SIZE;
	const float l_SubVelocity = -PANEL_SIZE;
	const int l_AddSpace = 1;
	const int l_SubSpace = -1;
	//普通
	if (_MoveType == MOVE_NONE) {
		//ボタンでマスを移動する
		if (MoveButtonKey()) {
			if (UpButtonKey()) {
				m_InputTimer[DIR_UP]++;
			}
			else if (DownButtonKey()) {
				m_InputTimer[DIR_DOWN]++;
			}
			else if (RightButtonKey()) {
				m_InputTimer[DIR_RIGHT]++;
			}
			else if (LeftButtonKey()) {
				m_InputTimer[DIR_LEFT]++;
			}
		}
		else {			//離した瞬間
			if (m_LimitCount == 0) {
				if (m_InputTimer[DIR_UP] != 0 && m_NowHeight < PANEL_HEIGHT - 1) {
					MoveCommon(m_Position.z, l_AddVelocity);
					m_InputTimer[DIR_UP] = {};
				}
				else if (m_InputTimer[DIR_DOWN] != 0 && m_NowHeight > 0) {
					MoveCommon(m_Position.z, l_SubVelocity);
					m_InputTimer[DIR_DOWN] = {};
				}
				else if (m_InputTimer[DIR_RIGHT] != 0 && m_NowWidth < (PANEL_WIDTH / 2) - 1) {
					MoveCommon(m_Position.x, l_AddVelocity);
					m_InputTimer[DIR_RIGHT] = {};
				}
				else if (m_InputTimer[DIR_LEFT] != 0 && m_NowWidth > 0) {
					MoveCommon(m_Position.x, l_SubVelocity);
					m_InputTimer[DIR_LEFT] = {};
				}
			}
			for (int i = 0; i < DIR_MAX; i++) {
				m_InputTimer[i] = {};
			}
			m_LimitCount = {};
		}

		//一定フレーム立つと選択マス移動
		if (m_InputTimer[DIR_UP] == l_TargetTimer) {
			if (m_NowHeight < PANEL_HEIGHT - 1) {
				MoveCommon(m_Position.z, l_AddVelocity);
				m_LimitCount++;
			}
			m_InputTimer[DIR_UP] = {};
		}
		else if (m_InputTimer[DIR_DOWN] == l_TargetTimer) {
			if (m_NowHeight > 0) {
				MoveCommon(m_Position.z, l_SubVelocity);
				m_LimitCount++;
			}
			m_InputTimer[DIR_DOWN] = {};
		}
		else if (m_InputTimer[DIR_RIGHT] == l_TargetTimer) {
			if (m_NowWidth < (PANEL_WIDTH / 2) - 1) {
				MoveCommon(m_Position.x, l_AddVelocity);
				m_LimitCount++;
			}
			m_InputTimer[DIR_RIGHT] = {};
		}
		else if (m_InputTimer[DIR_LEFT] == l_TargetTimer) {
			if (m_NowWidth > 0) {
				MoveCommon(m_Position.x, l_SubVelocity);
				m_LimitCount++;
			}
			m_InputTimer[DIR_LEFT] = {};
		}
	}
	//イージング
	else if (_MoveType == MOVE_EASE) {
		if (MoveButtonKey()
			&& !m_Move) {
			if (UpButtonKey()) {
				m_InputTimer[DIR_UP]++;
			}
			else if (DownButtonKey()) {
				m_InputTimer[DIR_DOWN]++;
			}
			else if (RightButtonKey()) {
				m_InputTimer[DIR_RIGHT]++;
			}
			else if (LeftButtonKey()) {
				m_InputTimer[DIR_LEFT]++;
			}
		}
		else {			//離した瞬間
			if (m_LimitCount == 0 && !m_Move) {
				if (m_InputTimer[DIR_UP] != 0 && m_NowHeight < PANEL_HEIGHT - 1) {
					m_Move = true;
					m_AfterPos.z = m_Position.z + l_AddVelocity;
					m_InputTimer[DIR_UP] = {};
				}
				else if (m_InputTimer[DIR_DOWN] != 0 && m_NowHeight > 0) {
					m_AfterPos.z = m_Position.z + l_SubVelocity;
					m_Move = true;
					m_InputTimer[DIR_DOWN] = {};
				}
				else if (m_InputTimer[DIR_RIGHT] != 0 && m_NowWidth < (PANEL_WIDTH / 2) - 1) {
					m_AfterPos.x = m_Position.x + l_AddVelocity;
					m_Move = true;
					m_InputTimer[DIR_RIGHT] = {};
				}
				else if (m_InputTimer[DIR_LEFT] != 0 && m_NowWidth > 0) {
					m_AfterPos.x = m_Position.x + l_SubVelocity;
					m_Move = true;
					m_InputTimer[DIR_LEFT] = {};
				}
			}
			for (int i = 0; i < DIR_MAX; i++) {
				m_InputTimer[i] = {};
			}
			m_LimitCount = {};
		}

		//一定フレーム立つと選択マス移動
		if (!m_Move) {
			if (m_InputTimer[DIR_UP] == l_TargetTimer) {
				if (m_NowHeight < PANEL_HEIGHT - 1) {
					m_AfterPos.z = m_Position.z + l_AddVelocity;
					m_Move = true;
					m_LimitCount++;
				}
				m_InputTimer[DIR_UP] = {};
			}
			else if (m_InputTimer[DIR_DOWN] == l_TargetTimer) {
				if (m_NowHeight > 0) {
					m_AfterPos.z = m_Position.z + l_SubVelocity;
					m_Move = true;
					m_LimitCount++;
				}
				m_InputTimer[DIR_DOWN] = {};
			}
			else if (m_InputTimer[DIR_RIGHT] == l_TargetTimer) {
				if (m_NowWidth < (PANEL_WIDTH / 2) - 1) {
					m_AfterPos.x = m_Position.x + l_AddVelocity;
					m_Move = true;
					m_LimitCount++;
				}
				m_InputTimer[DIR_RIGHT] = {};
			}
			else if (m_InputTimer[DIR_LEFT] == l_TargetTimer) {
				if (m_NowWidth > 0) {
					m_AfterPos.x = m_Position.x + l_SubVelocity;
					m_Move = true;
					m_LimitCount++;
				}
				m_InputTimer[DIR_LEFT] = {};
			}
		}


		//イージングで移動するためのもの
		if (m_Move) {
			float l_AddScale = 0.1f;
			//拡縮処理
			if (_AttackState == ATTACK_NONE) {
				if (m_MoveFrame < m_FrameMax / 2) {
					m_BaseScale -= l_AddScale;
				}
				else {
					m_BaseScale += l_AddScale;
				}
			}
			if (Helper::FrameCheck(m_MoveFrame, 0.2f)) {
				m_MoveFrame = {};
				m_Move = false;
				for (int i = 0; i < 4; i++) {
					m_InputTimer[i] = {};
				}
				GameStateManager::GetInstance()->SetGrazeScore(GameStateManager::GetInstance()->GetGrazeScore() + (m_GrazeScore * 5.0f));
				GameStateManager::GetInstance()->SetResetPredict(true);
				m_Scale = { 0.5f,0.5f,0.5f };
			}

			m_Position = { Ease(Out,Cubic,m_MoveFrame,m_Position.x,m_AfterPos.x),
			m_Position.y,
			Ease(Out,Cubic,m_MoveFrame,m_Position.z,m_AfterPos.z) };
		}
	}
	//残像
	else {
		//ボタンでマスを移動する
		if (MoveButtonKey()) {
			if (UpButtonKey()) {
				m_InputTimer[DIR_UP]++;
			}
			else if (DownButtonKey()) {
				m_InputTimer[DIR_DOWN]++;
			}
			else if (RightButtonKey()) {
				m_InputTimer[DIR_RIGHT]++;
			}
			else if (LeftButtonKey()) {
				m_InputTimer[DIR_LEFT]++;
			}
		}
		else {			//離した瞬間
			if (m_LimitCount == 0) {
				if (m_InputTimer[DIR_UP] != 0 && m_NowHeight < PANEL_HEIGHT - 1) {
					BirthImagePlayer();
					MoveCommon(m_Position.z, l_AddVelocity);
					m_InputTimer[DIR_UP] = {};
				}
				else if (m_InputTimer[DIR_DOWN] != 0 && m_NowHeight > 0) {
					BirthImagePlayer();
					MoveCommon(m_Position.z, l_SubVelocity);
					m_InputTimer[DIR_DOWN] = {};
				}
				else if (m_InputTimer[DIR_RIGHT] != 0 && m_NowWidth < (PANEL_WIDTH / 2) - 1) {
					BirthImagePlayer();
					MoveCommon(m_Position.x, l_AddVelocity);
					m_InputTimer[DIR_RIGHT] = {};
				}
				else if (m_InputTimer[DIR_LEFT] != 0 && m_NowWidth > 0) {
					BirthImagePlayer();
					MoveCommon(m_Position.x, l_SubVelocity);
					m_InputTimer[DIR_LEFT] = {};
				}
			}
			for (int i = 0; i < DIR_MAX; i++) {
				m_InputTimer[i] = {};
			}
			m_LimitCount = {};
		}

		//一定フレーム立つと選択マス移動
		if (m_InputTimer[DIR_UP] == l_TargetTimer) {
			if (m_NowHeight < PANEL_HEIGHT - 1) {
				BirthImagePlayer();
				MoveCommon(m_Position.z, l_AddVelocity);
				m_LimitCount++;
			}
			m_InputTimer[DIR_UP] = {};
		}
		else if (m_InputTimer[DIR_DOWN] == l_TargetTimer) {
			if (m_NowHeight > 0) {
				BirthImagePlayer();
				MoveCommon(m_Position.z, l_SubVelocity);
				m_LimitCount++;
			}
			m_InputTimer[DIR_DOWN] = {};
		}
		else if (m_InputTimer[DIR_RIGHT] == l_TargetTimer) {
			if (m_NowWidth < (PANEL_WIDTH / 2) - 1) {
				BirthImagePlayer();
				MoveCommon(m_Position.x, l_AddVelocity);
				m_LimitCount++;
			}
			m_InputTimer[DIR_RIGHT] = {};
		}
		else if (m_InputTimer[DIR_LEFT] == l_TargetTimer) {
			if (m_NowWidth > 0) {
				BirthImagePlayer();
				MoveCommon(m_Position.x, l_SubVelocity);
				m_LimitCount++;
			}
			m_InputTimer[DIR_LEFT] = {};
		}
	}

	m_Scale = { m_BaseScale,m_BaseScale ,m_BaseScale };
}

bool Player::MoveButtonKey() {
	if (input->PushButton(input->UP) ||
		input->PushButton(input->DOWN) ||
		input->PushButton(input->RIGHT) ||
		input->PushButton(input->LEFT) ||
		input->TiltPushStick(input->L_UP) ||
		input->TiltPushStick(input->L_DOWN) ||
		input->TiltPushStick(input->L_LEFT) ||
		input->TiltPushStick(input->L_RIGHT) ||
		input->TriggerKey(DIK_W) ||
		input->TriggerKey(DIK_A) ||
		input->TriggerKey(DIK_S) ||
		input->TriggerKey(DIK_D)
		) {
		return true;
	} else {
		return false;
	}
}

bool Player::UpButtonKey() {
	if (input->PushButton(input->UP) || input->TiltPushStick(input->L_UP) || input->Pushkey(DIK_W)) {
		return true;
	} else {
		return false;
	}
}

bool Player::DownButtonKey() {
	if (input->PushButton(input->DOWN) || input->TiltPushStick(input->L_DOWN) || input->Pushkey(DIK_S)) {
		return true;
	} else {
		return false;
	}
}

bool Player::RightButtonKey() {
	if (input->PushButton(input->RIGHT) || input->TiltPushStick(input->L_RIGHT) || input->Pushkey(DIK_D)) {
		return true;
	} else {
		return false;
	}
}

bool Player::LeftButtonKey() {
	if (input->PushButton(input->LEFT) || input->TiltPushStick(input->L_LEFT) || input->Pushkey(DIK_A)) {
		return true;
	} else {
		return false;
	}
}

void Player::BirthParticle() {
}
//HPの割合
float Player::HpPercent() {
	float temp = m_HP / m_MaxHP;
	Helper::Clamp(temp, 0.0f, 1.0f);
	return temp;
}
//ディレイ処理
void Player::Delay() {

}
//プレイヤーの動きの基本
void Player::MoveCommon(float& pos, float velocity) {
	pos += velocity;
	GameStateManager::GetInstance()->SetGrazeScore(GameStateManager::GetInstance()->GetGrazeScore() + (m_GrazeScore * 5.0f));
	GameStateManager::GetInstance()->SetResetPredict(true);
}
//プレイヤーのHP回復
void Player::HealPlayer(const float power) {
	m_HP += power;
	HealParticle();
}
//プレイヤーのダメージ判定
void Player::RecvDamage(const float Damage, const string& name) {
	m_HP -= Damage;
	//ダメージの種類によってパーティクルを変える
	if (name == "NORMAL") {
		DamageParticle();
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Damage.wav", 0.02f);
	} else if (name == "POISON") {
		BirthPoisonParticle();
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Fire.wav", 0.04f);
	}
	m_Damege = true;
	m_DamageTimer = {};
}
void Player::TitleUpdate() {
	Obj_SetParam();
}
//パーティクル(回復)
void Player::HealParticle() {
	XMFLOAT4 s_color = { 0.5f,1.0f,0.1f,1.0f };
	XMFLOAT4 e_color = { 0.5f,1.0f,0.1f,1.0f };
	float s_scale = 1.0f;
	float e_scale = 0.0f;
	for (int i = 0; i < 15; i++) {
		ParticleEmitter::GetInstance()->HealEffect(50, { m_Position.x,m_Position.y,m_Position.z }, s_scale, e_scale, s_color, e_color);
	}
}
//ダメージパーティクル
void Player::DamageParticle() {
	const XMFLOAT4 s_color = { 0.5f,0.5f,0.5f,1.0f };
	const XMFLOAT4 e_color = { 0.5f,0.5f,0.5f,1.0f };
	const float s_scale = 1.0f;
	const float e_scale = 0.0f;
	for (int i = 0; i < 15; i++) {
		ParticleEmitter::GetInstance()->Break(50, m_Position, s_scale, e_scale, s_color, e_color, 0.02f, 5.0f);
	}
}
void Player::BirthPoisonParticle() {
	const XMFLOAT4 s_color = { 1.f,0.0f,0.0f,1.0f };
	const XMFLOAT4 e_color = { 1.f,0.5f,0.0f,1.0f };
	const float s_scale = 1.0f;
	const float e_scale = 0.0f;
	for (int i = 0; i < 3; i++) {
		ParticleEmitter::GetInstance()->PoisonEffect(50, { m_Position.x,m_Position.y + 1.0f,m_Position.z }, s_scale, e_scale, s_color, e_color, 0.02f, 3.0f);
	}
}
bool Player::HPEffect() {
	if (m_OldHP == m_HP) { return false; }
	static float frame = 0.f;
	static float frameMax = 1 / 10.0f;
	if (isHeal || isDamage) {
		if (Helper::FrameCheck(frame, frameMax)) {
			XMFLOAT2 size = { 32.f,32.f };
			_drawnumber[FIRST_DIGHT]->SetSize(size);
			_drawnumber[SECOND_DIGHT]->SetSize(size);
			_drawnumber[THIRD_DIGHT]->SetSize(size);
			_drawnumber[FIRST_DIGHT]->SetColor({ 1.f,1.0f,1.f,1.0f });
			_drawnumber[SECOND_DIGHT]->SetColor({ 1.f,1.0f,1.f,1.0f });
			_drawnumber[THIRD_DIGHT]->SetColor({ 1.f,1.0f,1.f,1.0f });
			hptex->SetColor({ 0.5f,1.0f,0.5f,1.0f });
			frame = 0.f;
			isHeal = false;
			isDamage = false;
			m_OldHP = m_HP;
			return false;
		} else {
			XMFLOAT2 size = {64.f,64.f};
			size.x=Ease(Out,Quad,frame,64.f,32.f);
			size.y=Ease(Out,Quad,frame,64.f,32.f);
			_drawnumber[FIRST_DIGHT]->SetSize(size);
			_drawnumber[SECOND_DIGHT] ->SetSize(size);
			_drawnumber[THIRD_DIGHT]->SetSize(size);
		}
	}
	if (m_OldHP < m_HP) {
		isHeal = true;
		hptex->SetColor({ 0.0f,1.0f,0.0f,1.0f });
		_drawnumber[FIRST_DIGHT]->SetColor({ 0.f,1.0f,0.f,1.0f });
		_drawnumber[SECOND_DIGHT]->SetColor({ 0.f,1.0f,0.f,1.0f });
		_drawnumber[THIRD_DIGHT]->SetColor({ 0.f,1.0f,0.f,1.0f });
	} else {
		isDamage = true;
		hptex->SetColor({ 1.0f,0.5f,0.5f,1.0f });
		_drawnumber[FIRST_DIGHT]->SetColor({ 1.f,0.0f,0.f,1.0f });
		_drawnumber[SECOND_DIGHT]->SetColor({ 1.f,0.0f,0.f,1.0f });
		_drawnumber[THIRD_DIGHT]->SetColor({ 1.f,0.0f,0.f,1.0f });
	}
	return true;
}
//プレイヤーの情報をセーブ
void Player::PlayerSave() {
	const float l_StartHp = 100.0f;
	std::ofstream playerofs("Resources/csv/chara/player/player.csv");  // ファイルパスを指定する
	playerofs << "STARTHP" << "," << l_StartHp << std::endl;
	playerofs << "NOWHP" << "," << m_HP << std::endl;
	playerofs << "MAXHP" << "," << m_MaxHP << std::endl;
}
//ダメージ関係
void Player::DamageUpdate() {
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
	}
	else {
		m_Color.w = 0.0f;
	}
}
//残像作る
void Player::BirthImagePlayer() {
	std::unique_ptr<ImagePlayer> newimage = std::make_unique<ImagePlayer>();
	newimage->Initialize();
	newimage->SetPosition(m_Position);
	imageplayer.push_back(std::move(newimage));
}
//ゲームーオーバー時
void Player::GameOverUpdate() {
	const float l_AddRotZ = 0.5f;
	const float l_AddFrame = 0.005f;
	float RotPower = 5.0f;
	m_Color.w = 1.0f;

	if (Helper::FrameCheck(m_Frame, l_AddFrame)) {		//最初はイージングで回す
		m_Frame = 1.0f;
		if (Helper::CheckMin(m_Rotation.z, 90.0f, l_AddRotZ)) {		//最後は倒れる
			m_FinishGameOver = true;
		}
	}
	else {
		RotPower = Ease(In, Cubic, m_Frame, RotPower, 20.0f);
		m_Rotation.z =Ease(In,Cubic,m_Frame,m_Rotation.z,45.0f);
		m_Rotation.y += RotPower;
		m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 0.5f);
	}
	
	Obj_SetParam();
}
//プレイヤーの攻撃時瞬間取得
void Player::AttackCheck(const bool LastAttack) {
	if (LastAttack) {
		_AttackState = ATTACK_LAST;
		m_Jump = true;
		m_AddPower = 0.2f;
	}
	else {
		_AttackState = ATTACK_NORMAL;
	}
}
void Player::AttackMove() {
	const float l_AddFrame = 1 / 20.0f;
	if (_AttackState == ATTACK_NONE) { return; }
	m_AfterScale = 0.5f;
	if (Helper::FrameCheck(m_AttackFrame, l_AddFrame)) {
		m_AttackFrame = {};
		_AttackState = ATTACK_NONE;
		m_ShrinkTimer = {};
		m_Rotation.y = 90.0f;
	}
	else {
		m_BaseScale = Ease(In, Cubic, m_AttackFrame, m_BaseScale, m_AfterScale);
	}
	m_Scale = { m_BaseScale,m_BaseScale ,m_BaseScale };
	if (_AttackState == ATTACK_LAST) {
		m_Rotation.y = Ease(In, Cubic, m_AttackFrame, m_Rotation.y, 450.0f);
	}
}
//縮小
void Player::ShrinkScale() {
	if (!m_Delay) { return; }

	m_ShrinkTimer++;
	if (m_ShrinkTimer < 8) {
		m_AfterScale = 0.5f;
	}
	else if (m_ShrinkTimer >= 8 && m_ShrinkTimer < 20) {
		m_AfterScale = 0.4f;
	}
	else if (m_ShrinkTimer >= 20 && m_ShrinkTimer < 30) {
		m_AfterScale = 0.3f;
	}
	else if (m_ShrinkTimer >= 31) {
		m_AfterScale = 0.2f;
	}

	m_BaseScale = Ease(In, Cubic, 0.7f, m_BaseScale, m_AfterScale);
	m_Scale = { m_BaseScale,m_BaseScale ,m_BaseScale };
}