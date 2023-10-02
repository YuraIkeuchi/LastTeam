#include "StagePanel.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Input.h"
#include "Player.h"
#include "GameMode.h"
StagePanel* StagePanel::GetInstance()
{
	static StagePanel instance;

	return &instance;
}
//リソース読み込み
void StagePanel::LoadResource() {
	for (size_t i = 0; i < PANEL_HEIGHT; i++) {
		for (size_t j = 0; j < PANEL_WIDTH; j++) {
			m_Object[i][j].reset(new IKEObject3d());
			m_Object[i][j]->Initialize();
			m_Object[i][j]->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PANEL));
			m_Object[i][j]->SetScale({ 2.f,0.1f,2.f });
		}
	}
}
//初期化
bool StagePanel::Initialize()
{
	for (size_t i = 0; i < PANEL_HEIGHT; i++) {
		for (size_t j = 0; j < PANEL_WIDTH; j++) {
			m_Position[i][j] = { (2.0f * i),0.0f,(2.0f * j)};
			m_Color[i][j] = { 1.0f,1.0f,1.0f,1.0f };
		}
	}
	//CSV読み込み
	return true;
}

//更新処理
void StagePanel::Update()
{
	if (GameMode::GetInstance()->GetGameTurn() == TURN_SET) {
		SetUpdate();
	}
	else {
		BattleUpdate();
	}

}

//描画
void StagePanel::Draw(DirectXCommon* dxCommon)
{
	IKEObject3d::PreDraw();
	for (size_t i = 0; i < PANEL_HEIGHT; i++) {
		for (size_t j = 0; j < PANEL_WIDTH; j++) {
			m_Object[i][j]->Draw();
		}
	}
	IKEObject3d::PostDraw();
}

//ImGui
void StagePanel::ImGuiDraw() {
	ImGui::Begin("Panel");
	ImGui::SliderInt("Width", &m_SelectHeight, 0, PANEL_HEIGHT - 1);
	ImGui::SliderInt("Height", &m_SelectWidth, 0, PANEL_WIDTH - 1);
	ImGui::End();
}

//スキルセットの更新(バトル前)
void StagePanel::SetUpdate() {
	const int l_TimerMax = 10;
	Input* input = Input::GetInstance();
	//スティックでマスを選ぶ
	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f))
	{
		if (input->TiltPushStick(Input::L_UP, 0.0f) && m_SelectHeight < PANEL_HEIGHT - 1) {
			m_Timer[DIR_UP]++;
		}
		else if (input->TiltPushStick(Input::L_DOWN, 0.0f) && m_SelectHeight > 0) {
			m_Timer[DIR_DOWN]++;
		}
		else if (input->TiltPushStick(Input::L_RIGHT, 0.0f) && m_SelectWidth < PANEL_WIDTH - 1) {
			m_Timer[DIR_RIGHT]++;
		}
		else if (input->TiltPushStick(Input::L_LEFT, 0.0f) && m_SelectWidth > 0) {
			m_Timer[DIR_LEFT]++;
		}
	}
	else {
		for (int i = 0; i < DIR_MAX; i++) {
			m_Timer[i] = {};
		}
	}

	//一定フレーム立つと選択マス移動
	if (m_Timer[DIR_UP] == l_TimerMax) {
		m_SelectHeight++;
		m_Timer[DIR_UP] = {};
	}
	else if (m_Timer[DIR_DOWN] == l_TimerMax) {
		m_SelectHeight--;
		m_Timer[DIR_DOWN] = {};
	}else if (m_Timer[DIR_RIGHT] == l_TimerMax) {
		m_SelectWidth++;
		m_Timer[DIR_RIGHT] = {};
	}
	else if (m_Timer[DIR_LEFT] == l_TimerMax) {
		m_SelectWidth--;
		m_Timer[DIR_LEFT] = {};
	}

	//選択マスの色が変わる
	for (size_t i = 0; i < PANEL_HEIGHT; i++) {
		for (size_t j = 0; j < PANEL_WIDTH; j++) {
			if (m_SelectHeight == j && m_SelectWidth == i) {
				m_Color[i][j] = { 0.8f,0.8f,0.0f,1.0f };
			}
			else {
				m_Color[i][j] = { 1.0f,1.0f,1.0f,1.0f };
			}
			m_Object[i][j]->Update();
			m_Object[i][j]->SetPosition(m_Position[i][j]);
			m_Object[i][j]->SetColor(m_Color[i][j]);
		}
	}
}

//バトルの更新
void StagePanel::BattleUpdate() {
	//プレイヤーが居るマスが黄色くなる
	for (size_t i = 0; i < PANEL_HEIGHT; i++) {
		for (size_t j = 0; j < PANEL_WIDTH; j++) {
			if (Player::GetInstance()->GetNowHeight() == j && Player::GetInstance()->GetNowWidth() == i) {
				m_Color[i][j] = { 0.8f,0.8f,0.0f,1.0f };
			}
			else {
				m_Color[i][j] = { 1.0f,1.0f,1.0f,1.0f };
			}
			m_Object[i][j]->Update();
			m_Object[i][j]->SetPosition(m_Position[i][j]);
			m_Object[i][j]->SetColor(m_Color[i][j]);
		}
	}
}