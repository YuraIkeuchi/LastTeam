#include "Player.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Input.h"
#include "Easing.h"
#include "Collision.h"
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
	LoadCSV();
	//CSV読み込み
	return true;
}
//CSV読み込み
void Player::LoadCSV() {
	m_AddSpeed = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/player/player.csv", "speed2")));
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

//更新処理
void Player::Update()
{
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

		m_Position.x += move.m128_f32[0] * m_AddSpeed;
		m_Position.z += move.m128_f32[2] * m_AddSpeed;
	}
	m_Rotation = { m_MoveRot.x,m_MoveRot.y + 180.0f,m_MoveRot.z };

	//プレイヤーの現在マスを取得する
	m_PanelPos.z = m_Position.z / 1.5f;
	m_PanelPos.x = m_Position.x / 1.5f;

	m_NowHeight = (int)(m_PanelPos.z);
	m_NowWidth = (int)(m_PanelPos.x);
	Obj_SetParam();
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

//ImGui
void Player::ImGuiDraw() {
	ImGui::Begin("Player");
	ImGui::Text("NowWidth:%d", m_NowWidth);
	ImGui::Text("NowHeight:%d", m_NowHeight);
	ImGui::Text("PosX:%f", m_Position.x);
	ImGui::Text("PosZ:%f", m_Position.z);
	ImGui::End();
}
