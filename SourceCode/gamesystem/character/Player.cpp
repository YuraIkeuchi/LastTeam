#include "Player.h"
#include "CsvLoader.h"
#include <Helper.h>
#include "Input.h"
#include "Audio.h"
#include <GameStateManager.h>
#include <StagePanel.h>
#include <ImageManager.h>
#include <ParticleEmitter.h>
Player* Player::GetInstance() {
	static Player instance;

	return &instance;
}
//リソース読み込み
void Player::LoadResource() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	m_Object->SetScale({ 2.f,2.f,2.f });
	m_Object->SetPosition({ 0.0f,2.0f,0.0f });
	m_Object->SetLightEffect(false);

	//HPII
	hptex = IKESprite::Create(ImageManager::ENEMYHPUI, { 0.0f,0.0f });
	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i] = make_unique<DrawNumber>();
		_drawnumber[i]->Initialize();
	}
	_drawnumber[FIRST_DIGHT]->SetPosition({ 100.0f,620.0f });
	_drawnumber[SECOND_DIGHT]->SetPosition({ 80.0f,620.0f });
	_drawnumber[THIRD_DIGHT]->SetPosition({ 60.0f,620.0f });
}
//初期化
bool Player::Initialize() {

	LoadCSV();
	m_MaxHP = m_HP;
	//CSV読み込み
	return true;
}
//CSV読み込み
void Player::LoadCSV() {
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "speed")));
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "HP")));
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
	}
	m_InterHP = {};//整数にしたHP

}
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (Player::* Player::stateTable[])() = {
	&Player::Move,//移動
	&Player::Delay,//動きが止まる
};
//更新処理
void Player::Update() {
	const float l_GrazeMax = 2.0f;

	//状態移行(charastateに合わせる)
	(this->*stateTable[_charaState])();
	////ディレイタイマーが0以外ならディレイにする
	//if (m_DelayTimer != 0) {
	//	_charaState = STATE_DELAY;
	//}
	Obj_SetParam();

	BirthParticle();

	//グレイズ用にスコアを計算する
	m_Length = Helper::GetInstance()->ChechLength(m_Position, m_GrazePos);
	m_GrazeScore = l_GrazeMax - m_Length;
	//最大スコアは10
	Helper::GetInstance()->Clamp(m_GrazeScore, 0.0f, l_GrazeMax);
	//プレイヤーの位置からスコアを加算する
	GameStateManager::GetInstance()->SetPosScore(GameStateManager::GetInstance()->GetPosScore() + ((float)(m_NowWidth) * 0.1f));
	GameStateManager::GetInstance()->PlayerNowPanel(m_NowWidth, m_NowHeight);

	//HPの限界値を決める
	Helper::GetInstance()->Clamp(m_HP, 0.0f, m_MaxHP);
	//表示用のHP
	m_InterHP = (int)(m_HP);
	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i]->SetNumber(m_DigitNumber[i]);
		_drawnumber[i]->Update();
		m_DigitNumber[i] = Helper::GetInstance()->getDigits(m_InterHP, i, i);
	}
	hptex->SetPosition(m_HPPos);
	hptex->SetSize({ HpPercent() * m_HPSize.x,m_HPSize.y });
}
//描画
void Player::Draw(DirectXCommon* dxCommon) {

	Obj_Draw();
}
//UIの描画
void Player::UIDraw() {
	IKESprite::PreDraw();
	//HPバー
	hptex->Draw();
	_drawnumber[FIRST_DIGHT]->Draw();
	if (m_InterHP >= 10)
		_drawnumber[SECOND_DIGHT]->Draw();
	if (m_InterHP >= 100)
		_drawnumber[THIRD_DIGHT]->Draw();
	IKESprite::PostDraw();
}
//ImGui
void Player::ImGuiDraw() {
	ImGui::Begin("Player");
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::Text("POSZ:%f", m_Position.z);
	ImGui::SliderFloat("HP", &m_HP, 0.0f, 100.0f);
	ImGui::End();
}
//移動
void Player::Move() {
	if (m_Delay) { return; }
	const int l_TargetTimer = 10;
	const float l_AddVelocity = 2.0f;
	const float l_SubVelocity = -2.0f;
	const int l_AddSpace = 1;
	const int l_SubSpace = -1;
	//ボタンでマスを移動する
	if (input->PushButton(input->UP) ||
		input->PushButton(input->DOWN) ||
		input->PushButton(input->RIGHT) ||
		input->PushButton(input->LEFT) ||
		input->TiltPushStick(input->L_UP)||
		input->TiltPushStick(input->L_DOWN) ||
		input->TiltPushStick(input->L_LEFT) ||
		input->TiltPushStick(input->L_RIGHT)
		) {
		if (input->PushButton(input->UP)|| input->TiltPushStick(input->L_UP)) {
			m_InputTimer[DIR_UP]++;
		} else if (input->PushButton(input->DOWN)|| input->TiltPushStick(input->L_DOWN)) {
			m_InputTimer[DIR_DOWN]++;
		} else if (input->PushButton(input->RIGHT)|| input->TiltPushStick(input->L_RIGHT)) {
			m_InputTimer[DIR_RIGHT]++;
		} else if (input->PushButton(input->LEFT)|| input->TiltPushStick(input->L_LEFT)) {
			m_InputTimer[DIR_LEFT]++;
		}
	} else {			//離した瞬間
		if (m_LimitCount == 0) {
			if (m_InputTimer[DIR_UP] != 0 && m_NowHeight < PANEL_HEIGHT - 1) {
				MoveCommon(m_Position.z, l_AddVelocity, m_NowHeight, l_AddSpace);
				m_InputTimer[DIR_UP] = {};
			} else if (m_InputTimer[DIR_DOWN] != 0 && m_NowHeight > 0) {
				MoveCommon(m_Position.z, l_SubVelocity, m_NowHeight, l_SubSpace);
				m_InputTimer[DIR_DOWN] = {};
			} else if (m_InputTimer[DIR_RIGHT] != 0 && m_NowWidth < (PANEL_WIDTH / 2) - 1) {
				MoveCommon(m_Position.x, l_AddVelocity, m_NowWidth, l_AddSpace);
				m_InputTimer[DIR_RIGHT] = {};
			} else if (m_InputTimer[DIR_LEFT] != 0 && m_NowWidth > 0) {
				MoveCommon(m_Position.x, l_SubVelocity, m_NowWidth, l_SubSpace);
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
			MoveCommon(m_Position.z, l_AddVelocity, m_NowHeight, l_AddSpace);
			m_LimitCount++;
		}
		m_InputTimer[DIR_UP] = {};
	} else if (m_InputTimer[DIR_DOWN] == l_TargetTimer) {
		if (m_NowHeight > 0) {
			MoveCommon(m_Position.z, l_SubVelocity, m_NowHeight, l_SubSpace);
			m_LimitCount++;
		}
		m_InputTimer[DIR_DOWN] = {};
	} else if (m_InputTimer[DIR_RIGHT] == l_TargetTimer) {
		if (m_NowWidth < (PANEL_WIDTH / 2) - 1) {
			MoveCommon(m_Position.x, l_AddVelocity, m_NowWidth, l_AddSpace);
			m_LimitCount++;
		}
		m_InputTimer[DIR_RIGHT] = {};
	} else if (m_InputTimer[DIR_LEFT] == l_TargetTimer) {
		if (m_NowWidth > 0) {
			MoveCommon(m_Position.x, l_SubVelocity, m_NowWidth, l_SubSpace);
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
	Helper::GetInstance()->Clamp(temp, 0.0f, 1.0f);
	return temp;
}
//ディレイ処理
void Player::Delay() {

}
//プレイヤーの動きの基本
void Player::MoveCommon(float& pos, float velocity, int& playerspace,const int addspace) {
	pos += velocity;
	playerspace += addspace;
	GameStateManager::GetInstance()->SetGrazeScore(GameStateManager::GetInstance()->GetGrazeScore() + (m_GrazeScore * 5.0f));
	GameStateManager::GetInstance()->SetResetPredict(true);
}
//プレイヤーのHP回復
void Player::HealPlayer(const float power) {
	m_HP += power;
	for (int i = 0; i < 15; i++) {
		Particle();
	}
}
//チュートリアルの更新
void Player::TitleUpdate() {
	Obj_SetParam();
}
//パーティクル
void Player::Particle() {
	XMFLOAT4 s_color = { 0.5f,1.0f,0.1f,1.0f };
	XMFLOAT4 e_color = { 0.5f,1.0f,0.1f,1.0f };
	float s_scale = 1.0f;
	float e_scale = 0.0f;
	ParticleEmitter::GetInstance()->HealEffect(50, { m_Position.x,m_Position.y,m_Position.z }, s_scale, e_scale, s_color, e_color);
}