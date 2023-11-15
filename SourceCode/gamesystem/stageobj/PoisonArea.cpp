#include "PoisonArea.h"
#include <StagePanel.h>
#include <ModelManager.h>
#include <GameStateManager.h>
#include <Helper.h>
//読み込み
PoisonArea::PoisonArea() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::PANEL);
	Initialize();
}
//初期化
bool PoisonArea::Initialize() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
	return true;
}

//ステータス初期化
void PoisonArea::InitState(const int width, const int height) {
	m_Position = SetPanelPos(width, height);
	m_NowWidth = width, m_NowHeight = height;
	m_Scale = { 2.0f,0.1f,2.0f };
	m_Position.y = 0.1f;
	m_Color = { 0.5f,0.0f,0.5f,1.0f };
	m_Alive = true;
	m_Hit = false;
	m_AliveTimer = {};
}

//更新
void PoisonArea::Update() {
	const int l_TargetTimer = 10;

	if (Helper::GetInstance()->CheckMin(m_AliveTimer, l_TargetTimer, 1)) {
		GameStateManager::GetInstance()->SetBuff(false);
		m_Alive = false;
	}

	Obj_SetParam();
}
//描画
void PoisonArea::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui
void PoisonArea::ImGuiDraw() {
	ImGui::Begin("Area");
	ImGui::Text("POSX:%f", m_Position.x);
	//ImGui::Text("POSZ:%f", m_Position.z);
	ImGui::End();
}
//パネルの位置に置く
XMFLOAT3 PoisonArea::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}