#include "AttackAction.h"
#include "ModelManager.h"
#include "ParticleEmitter.h"

//読み込み
AttackAction::AttackAction() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PANEL));
	//m_Object->SetScale({ 2.f,0.1f,2.f });
}
//初期化
bool AttackAction::Initialize() {
	m_Position = {};
	m_Scale = { 0.5f,0.5f,0.5f };
	m_Color = { 1.0f,0.3f,0.0f,1.0f };
	m_Alive = true;
	m_Tag = "Attack";
	return true;
}
//行動
void AttackAction::Action() {
	Obj_SetParam();
	Collide();
}
//描画
void AttackAction::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//エフェクト描画
void AttackAction::EffecttexDraw(DirectXCommon* dxCommon) {

}
//ImGui
void AttackAction::ImGui_Origin() {
	ImGui::Begin("Attack");
	ImGui::Text("PosX:%f", m_Position.x);
	ImGui::Text("PosZ:%f", m_Position.z);
	ImGui::End();
}
//ポーズ
void AttackAction::Pause() {

}