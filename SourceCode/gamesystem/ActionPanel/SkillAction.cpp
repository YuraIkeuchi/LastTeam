#include "SkillAction.h"
#include "ModelManager.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include <Helper.h>

//読み込み
SkillAction::SkillAction() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PANEL));
	//m_Object->SetScale({ 2.f,0.1f,2.f });	
	m_Pannel.reset(new IKETexture(ImageManager::MAGIC, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	m_Pannel->TextureCreate();
	m_Pannel->Initialize();

	m_SkillID = Helper::GetInstance()->GetRanNum(0, 3);
}
//初期化
bool SkillAction::Initialize() {
	m_Position = {};
	m_Scale = { 0.5f,0.5f,0.5f };
	m_Color = { 0.0f, 1.0f, 0.3f, 1.0f };
	m_Alive = true;
	m_Tag = "Skill";
	return true;
}
//行動
void SkillAction::Action() {
	(this->*stateTable[_state])();
	Obj_SetParam();
	Collide();
}
//描画
void SkillAction::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon,0);
	m_Pannel->Draw();
	IKETexture::PostDraw();
	Obj_Draw();
}
//エフェクト描画
void SkillAction::EffecttexDraw(DirectXCommon* dxCommon) {

}
//ImGui
void SkillAction::ImGui_Origin() {
	ImGui::Begin("Skill");
	ImGui::Text("PosX:%f", m_Position.x);
	ImGui::Text("PosZ:%f", m_Position.z);
	ImGui::End();
}
//ポーズ
void SkillAction::Pause() {

}