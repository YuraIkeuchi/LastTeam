﻿#include "Player.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Input.h"
#include "Easing.h"
#include "Collision.h"
#include "GameMode.h"
Player* Player::GetInstance()
{
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
	m_Object->VertexCheck();
}
//初期化
bool Player::Initialize()
{
	//要素の全削除は一旦ここで
	actui.clear();
	LoadCSV();
	//CSV読み込み
	return true;
}
//CSV読み込み
void Player::LoadCSV() {
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "speed")));
}
//ステータスの初期化
void Player::InitState(const XMFLOAT3& pos) {
	m_Position = pos;
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,1.0f,1.0f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };
	//移動処理用
	velocity /= 5.0f;
}
//状態遷移
/*CharaStateのState並び順に合わせる*/
void (Player::* Player::stateTable[])() = {
	&Player::Move,//移動
	&Player::Attack,//攻撃
};
//更新処理
void Player::Update()
{
	//状態移行(charastateに合わせる)
	(this->*stateTable[_charaState])();
	Obj_SetParam();
	for (auto i = 0; i < actui.size(); i++) {
		if (actui[i] == nullptr)continue;
		actui[i]->Update();
	}
}
//VECTOR
XMFLOAT3 Player::MoveVECTOR(XMVECTOR v, float angle)
{
	XMMATRIX rot2 = {};
	rot2 = XMMatrixRotationY(XMConvertToRadians(angle));
	v = XMVector3TransformNormal(v, rot2);
	XMFLOAT3 pos = { v.m128_f32[0], v.m128_f32[1], v.m128_f32[2] };
	return pos;
}
//描画
void Player::Draw(DirectXCommon* dxCommon)
{
	Obj_Draw();
}
void Player::ActUIDraw() {
	IKESprite::PreDraw();
	for (auto i = 0; i < actui.size(); i++) {
		if (actui[i] == nullptr)continue;
		actui[i]->Draw();
	}
	IKESprite::PostDraw();
}
//ImGui
void Player::ImGuiDraw() {
	ImGui::Begin("Player");
	ImGui::Text("Attack:%d", m_ActCount[ACT_ATTACK]);
	ImGui::Text("Guard:%d", m_ActCount[ACT_GUARD]);
	ImGui::Text("Skill:%d", m_ActCount[ACT_SKILL]);
	ImGui::End();

	for (auto i = 0; i < actui.size(); i++) {
		if (actui[i] == nullptr)continue;
		actui[i]->ImGuiDraw();
	}
}

//移動
void Player::Move() {
	XMFLOAT3 rot = m_Rotation;

	Input* input = Input::GetInstance();
	float StickX = input->GetLeftControllerX();
	float StickY = input->GetLeftControllerY();
	const float STICK_MAX = 32768.0f;
	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f))
	{
		//上入力
		if (input->TiltPushStick(Input::L_UP, 0.0f))
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0, 0, velocity, 0 }, angle);

		//下入力
		if (input->TiltPushStick(Input::L_DOWN, 0.0f))
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ 0, 0, -velocity, 0 }, angle);

		//右入力
		if (input->TiltPushStick(Input::L_RIGHT, 0.0f))
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ velocity, 0, 0, 0 }, angle);

		//左入力
		if (input->TiltPushStick(Input::L_LEFT, 0.0f))
			XMFLOAT3 vecvel = MoveVECTOR(XMVECTOR{ -velocity, 0, 0, 0 }, angle);

		const float rnd_vel = 0.1f;

		XMFLOAT3 vel{};

		vel.x = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = static_cast<float>(rand()) / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		rot.y = angle + atan2f(StickX, StickY) * (PI_180 / PI);

		//プレイヤーの回転角を取る
		m_MoveRot = { rot.x, rot.y, rot.z };

		XMVECTOR move = { 0.0f, 0.0f, 0.1f, 0.0f };
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(m_MoveRot.y));
		move = XMVector3TransformNormal(move, matRot);
		if (GameMode::GetInstance()->GetGameTurn() == TURN_BATTLE) {
			m_Position.x += move.m128_f32[0] * m_AddSpeed;
			m_Position.z += move.m128_f32[2] * m_AddSpeed;
		}
	}
	m_Rotation = { m_MoveRot.x,m_MoveRot.y + 180.0f,m_MoveRot.z };

	//リミット制限
	Helper::GetInstance()->Clamp(m_Position.x, -7.5f, -1.3f);
	Helper::GetInstance()->Clamp(m_Position.z, -0.5f, 6.3f);
}
void Player::Attack() {
	const float l_AddFrame = 0.05f;
	const int l_CoolMax = 30;
	//攻撃のパネルを取った分だけ攻撃する
	if (m_AttackCount != m_ActCount[ACT_ATTACK]) {
		if (_AttackState == ATTACK_ENEMY) {
			if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
				m_Frame = {};
				m_Position = m_ReturnPos;
				_AttackState = ATTACK_INTER;
				m_AttackCount++;
			}
			m_Position = {
			Ease(In,Cubic,m_Frame,m_Position.x,m_TargetPos.x),
			Ease(In,Cubic,m_Frame,m_Position.y,m_TargetPos.y),
			Ease(In,Cubic,m_Frame,m_Position.z,m_TargetPos.z),
			};
		}
		//攻撃後のクールタイム
		else {
			if (Helper::GetInstance()->CheckMin(m_CoolTime, l_CoolMax, 1)) {
				_AttackState = ATTACK_ENEMY;
				m_CoolTime = {};
			}
		}
	}
	else {	//攻撃終了
		_charaState = STATE_MOVE;
		m_AttackCount = {};
		m_CoolTime = {};
		for (int i = 0; i < ACT_PATTERN; i++) {
			m_ActCount[i] = {};
		}
	}
}
//行動力を入手
void Player::AddAct(const string& Tag) {
	if (Tag == "Attack") {
		m_ActCount[ACT_ATTACK]++;
	}
	else if (Tag == "Guard") {
		m_ActCount[ACT_GUARD]++;
	}
	else if (Tag == "Skill") {
		m_ActCount[ACT_SKILL]++;
	}
	else {
		assert(0);
	}
	m_AllActCount++;
	BirthActUI(Tag);
}
//攻撃先指定
void Player::AttackTarget(const XMFLOAT3& pos) {
	_charaState = STATE_ATTACK;
	m_TargetPos = pos;
	m_ReturnPos = m_Position;
}
//行動UIの生成
void Player::BirthActUI(const string& Tag) {
	//アクションのセット
	ActionUI* newactUi = nullptr;
	newactUi = new ActionUI();
	newactUi->Initialize();
	newactUi->InitState(m_AllActCount,Tag);
	actui.emplace_back(newactUi);
}