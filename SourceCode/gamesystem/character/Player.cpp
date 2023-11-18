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
	m_Object->SetScale({ 2.f,2.f,2.f });
	m_Object->SetPosition({ 0.0f,PANEL_SIZE,0.0f });
	m_Object->SetLightEffect(false);

	//HPII
	hptex = IKESprite::Create(ImageManager::ENEMYHPUI, { 0.0f,0.0f });
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
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };
	_charaState = STATE_MOVE;
	//攻撃先
	m_TargetPos = {};
	//戻り先
	m_ReturnPos = {};
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
		hptex->SetSize({ HpPercent() * m_HPSize.x,m_HPSize.y });
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
	Obj_Draw();
}

//UIの描画
void Player::UIDraw() {
	IKESprite::PreDraw();
	//HPバー
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
	ImGui::Text("NowHeight:%d,NowWidth:%d", m_NowHeight, m_NowWidth);
	ImGui::SliderFloat("HP", &m_HP, 0, m_MaxHP);
	ImGui::End();
}
//移動
void Player::Move() {
	if (m_Delay) { return; }
	if (GameStateManager::GetInstance()->GetResetPredict()) { return; }
	const int l_TargetTimer = 10;
	const float l_AddVelocity = PANEL_SIZE;
	const float l_SubVelocity = -PANEL_SIZE;
	const int l_AddSpace = 1;
	const int l_SubSpace = -1;
	//ボタンでマスを移動する
	if (input->PushButton(input->UP) ||
		input->PushButton(input->DOWN) ||
		input->PushButton(input->RIGHT) ||
		input->PushButton(input->LEFT) ||
		input->TiltPushStick(input->L_UP) ||
		input->TiltPushStick(input->L_DOWN) ||
		input->TiltPushStick(input->L_LEFT) ||
		input->TiltPushStick(input->L_RIGHT)
		) {
		if (input->PushButton(input->UP) || input->TiltPushStick(input->L_UP)) {
			m_InputTimer[DIR_UP]++;
		} else if (input->PushButton(input->DOWN) || input->TiltPushStick(input->L_DOWN)) {
			m_InputTimer[DIR_DOWN]++;
		} else if (input->PushButton(input->RIGHT) || input->TiltPushStick(input->L_RIGHT)) {
			m_InputTimer[DIR_RIGHT]++;
		} else if (input->PushButton(input->LEFT) || input->TiltPushStick(input->L_LEFT)) {
			m_InputTimer[DIR_LEFT]++;
		}
	} else {			//離した瞬間
		if (m_LimitCount == 0) {
			if (m_InputTimer[DIR_UP] != 0 && m_NowHeight < PANEL_HEIGHT - 1) {
				MoveCommon(m_Position.z, l_AddVelocity);
				m_InputTimer[DIR_UP] = {};
			} else if (m_InputTimer[DIR_DOWN] != 0 && m_NowHeight > 0) {
				MoveCommon(m_Position.z, l_SubVelocity);
				m_InputTimer[DIR_DOWN] = {};
			} else if (m_InputTimer[DIR_RIGHT] != 0 && m_NowWidth < (PANEL_WIDTH / 2) - 1) {
				MoveCommon(m_Position.x, l_AddVelocity);
				m_InputTimer[DIR_RIGHT] = {};
			} else if (m_InputTimer[DIR_LEFT] != 0 && m_NowWidth > 0) {
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
	} else if (m_InputTimer[DIR_DOWN] == l_TargetTimer) {
		if (m_NowHeight > 0) {
			MoveCommon(m_Position.z, l_SubVelocity);
			m_LimitCount++;
		}
		m_InputTimer[DIR_DOWN] = {};
	} else if (m_InputTimer[DIR_RIGHT] == l_TargetTimer) {
		if (m_NowWidth < (PANEL_WIDTH / 2) - 1) {
			MoveCommon(m_Position.x, l_AddVelocity);
			m_LimitCount++;
		}
		m_InputTimer[DIR_RIGHT] = {};
	} else if (m_InputTimer[DIR_LEFT] == l_TargetTimer) {
		if (m_NowWidth > 0) {
			MoveCommon(m_Position.x, l_SubVelocity);
			m_LimitCount++;
		}
		m_InputTimer[DIR_LEFT] = {};
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
//チュートリアルの更新
//プレイヤーのダメージ判定
void Player::RecvDamage(const float Damage, const string& name) {
	m_HP -= Damage;
	//ダメージの種類によってパーティクルを変える
	if (name == "NORMAL") {
		DamageParticle();
	} else if (name == "POISON") {
		BirthPoisonParticle();
	}
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
	const float s_scale = 2.0f;
	const float e_scale = 0.0f;
	for (int i = 0; i < 15; i++) {
		ParticleEmitter::GetInstance()->Break(50, m_Position, s_scale, e_scale, s_color, e_color, 0.02f, 5.0f);
	}
}
void Player::BirthPoisonParticle() {
	const XMFLOAT4 s_color = { 0.5f,0.0f,0.5f,1.0f };
	const XMFLOAT4 e_color = { 0.5f,0.0f,0.5f,1.0f };
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
		_drawnumber[FIRST_DIGHT]->SetColor({ 0.f,1.0f,0.f,1.0f });
		_drawnumber[SECOND_DIGHT]->SetColor({ 0.f,1.0f,0.f,1.0f });
		_drawnumber[THIRD_DIGHT]->SetColor({ 0.f,1.0f,0.f,1.0f });
	} else {
		isDamage = true;
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