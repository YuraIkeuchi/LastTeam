#include "StagePanel.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Input.h"
#include "Player.h"
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
