#include "SkillAction.h"
#include "ModelManager.h"
#include "ParticleEmitter.h"

//読み込み
SkillAction::SkillAction() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PANEL));
	//m_Object->SetScale({ 2.f,0.1f,2.f });
}
//初期化
bool SkillAction::Initialize() {
	m_Position = {};
	m_Scale = { 1.0f,1.0f,1.0f };
	m_Color = { 0.3f,0.0f,1.0f,1.0f };
	m_Alive = true;
	m_Tag = "Skill";
	return true;
}
//行動
void SkillAction::Action() {
	Obj_SetParam();
	Collide();
}
//描画
void SkillAction::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//エフェクト描画
void SkillAction::EffecttexDraw(DirectXCommon* dxCommon) {

}
//ImGui
void SkillAction::ImGui_Origin() {

}
//ポーズ
void SkillAction::Pause() {

}