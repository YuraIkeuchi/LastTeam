#include "GuardAction.h"
#include "ModelManager.h"
#include "ParticleEmitter.h"

//読み込み
GuardAction::GuardAction() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PANEL));
	//m_Object->SetScale({ 2.f,0.1f,2.f });
}
//初期化
bool GuardAction::Initialize() {
	m_Position = {};
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 0.0f,1.0f,0.3f,1.0f };
	m_Alive = true;
	m_Tag = "Guard";
	return true;
}
//行動
void GuardAction::Action() {
	Obj_SetParam();
	Collide();
}
//描画
void GuardAction::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//エフェクト描画
void GuardAction::EffecttexDraw(DirectXCommon* dxCommon) {

}
//ImGui
void GuardAction::ImGui_Origin() {

}
//ポーズ
void GuardAction::Pause() {

}