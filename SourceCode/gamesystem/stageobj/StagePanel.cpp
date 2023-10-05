#include "StagePanel.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Input.h"
#include "Player.h"
#include "GameMode.h"
#include "Collision.h"
StagePanel* StagePanel::GetInstance()
{
	static StagePanel instance;

	return &instance;
}
//リソース読み込み
void StagePanel::LoadResource() {
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
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
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			m_Position[i][j] = { (2.0f * i),0.0f,(2.0f * j)};
			m_Color[i][j] = { 1.0f,1.0f,1.0f,1.0f };
			m_PanelType[i][j] = NO_PANEL;
			m_PanelHit[i][j] = false;
		}
	}
	m_SelectHeight = 0;
	m_SelectWidth = 0;

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

	//セレクトしているものの座標
	m_SelectPos = m_Position[m_SelectWidth][m_SelectHeight];
}

//描画
void StagePanel::Draw(DirectXCommon* dxCommon)
{
	IKEObject3d::PreDraw();
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			m_Object[i][j]->Draw();
		}
	}
	IKEObject3d::PostDraw();
}

//ImGui
void StagePanel::ImGuiDraw() {
	//ImGui::Begin("Panel");
	//ImGui::Text("Can:%d", m_CanSet);
	//ImGui::Text("Height:%d", m_SelectHeight);
	//ImGui::Text("Width:%d", m_SelectWidth);
	//for (int i = 0; i < PANEL_WIDTH; i++) {
	//	for (int j = 0; j < PANEL_HEIGHT; j++) {
	//		ImGui::Text("PanelType[%d][%d]:%d", i, j, m_PanelType[i][j]);
	//	}
	//}
	//ImGui::End();
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
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			if (m_PanelType[i][j] == NO_PANEL) {
				if (m_SelectHeight == j && m_SelectWidth == i) {
					m_Color[i][j] = { 0.8f,0.8f,0.0f,1.0f };
				}
				else {
					m_Color[i][j] = { 1.0f,1.0f,1.0f,1.0f };
				}
			}
			else {
				m_Color[i][j] = { 0.5f,0.5f,0.5f,1.0f };
			}
			m_Object[i][j]->Update();
			m_Object[i][j]->SetPosition(m_Position[i][j]);
			m_Object[i][j]->SetColor(m_Color[i][j]);
		}
	}

	//パネルを置けるかどうかをチェックする
	if ((m_PanelType[m_SelectWidth][m_SelectHeight] == NO_PANEL) && !m_PanelHit[m_SelectWidth][m_SelectHeight]) {
		m_CanSet = true;
	}
	else {
		m_CanSet = false;
	}
}

//バトルの更新
void StagePanel::BattleUpdate() {
	//プレイヤーが居るマスが黄色くなる
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			if (!m_PanelHit[i][j]) {
				if (m_PanelType[i][j] == NO_PANEL) {
					m_Color[i][j] = { 1.0f,1.0f,1.0f,1.0f };
				}
				else {
					m_Color[i][j] = { 0.5f,0.5f,0.5f,1.0f };
				}
			}
			else {
				m_Color[i][j] = { 0.8f,0.8f,0.0f,1.0f };
			}
			m_Object[i][j]->Update();
			m_Object[i][j]->SetPosition(m_Position[i][j]);
			m_Object[i][j]->SetColor(m_Color[i][j]);
		}
	}

	Collide();
}
//パネルの変更
void StagePanel::PanelChange(const string& Tag) {
	//パネル状況によって色が変わる
	if (Tag == "Attack") {
		m_PanelType[m_SelectWidth][m_SelectHeight] = ATTACK_PANEL;
	}
	else if (Tag == "Guard") {
		m_PanelType[m_SelectWidth][m_SelectHeight] = GUARD_PANEL;
	}
	else if (Tag == "Skill") {
		m_PanelType[m_SelectWidth][m_SelectHeight] = SKILL_PANEL;
	}
}
//パネルの除去
void StagePanel::DeletePanel() {
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			if (m_PanelHit[i][j]) {
				m_PanelType[i][j] = NO_PANEL;
			}
		}
	}
}

void StagePanel::Collide() {
	m_OBB1.SetParam_Pos(Player::GetInstance()->GetPosition());
	m_OBB1.SetParam_Rot(Player::GetInstance()->GetMatrot());
	m_OBB1.SetParam_Scl(Player::GetInstance()->GetScale());
	for (int i = 0; i < PANEL_WIDTH; i++) {
		for (int j = 0; j < PANEL_HEIGHT; j++) {
			m_OBB2.SetParam_Pos({ m_Object[i][j]->GetPosition().x,1.0f,m_Object[i][j]->GetPosition().z });
			m_OBB2.SetParam_Rot(m_Object[i][j]->GetMatrot());
			m_OBB2.SetParam_Scl({0.5f,1.0f,0.5f});
			if ((Collision::OBBCollision(m_OBB1, m_OBB2))) {
				m_PanelHit[i][j] = true;
			}
			else {
				m_PanelHit[i][j] = false;
			}
		}
	}
}