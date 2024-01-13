#include "SkillAction.h"
#include "ModelManager.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"
#include <Helper.h>

//読み込み
SkillAction::SkillAction(const int SkillID) {
	m_SkillID = SkillID;
	m_Pannel.reset(new IKETexture(ImageManager::ATTACK_TEX_0 + m_SkillID, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	m_Pannel->TextureCreate();
	m_Pannel->Initialize();
	m_Pannel->SetIsBillboard(true);

	shadow.tex.reset(new IKETexture(ImageManager::SHADOW2, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow.tex->TextureCreate();
	shadow.tex->Initialize();
	shadow.tex->SetRotation({ 90.0f,0.0f,0.0f });

}
//初期化
bool SkillAction::Initialize() {
	m_Position = {};
	m_Scale = { 0.3f,0.3f,0.3f };
	m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_Alive = true;
	m_Tag = "Skill";
	shadow.Scale = { 0.12f,0.12f,0.12f };
	return true;
}
//行動
void SkillAction::Action() {
	(this->*stateTable[_state])();
	Collide();
	GetData();

	shadow.Position = { m_Position.x + 0.2f,m_Position.y + 0.11f,m_Position.z + 0.15f };
	shadow.tex->SetPosition(shadow.Position);
	shadow.tex->SetScale(shadow.Scale);
	shadow.tex->Update();

}
//描画
void SkillAction::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon,AlphaBlendType);
	shadow.tex->Draw();
	m_Pannel->Draw();
	IKETexture::PostDraw();
}
//エフェクト描画
void SkillAction::EffecttexDraw(DirectXCommon* dxCommon) {

}
//ImGui
void SkillAction::ImGui_Origin() {
	ImGui::Begin("Skill");
	ImGui::Text("Token:%d", m_SkillID);
	ImGui::End();
}
//ポーズ
void SkillAction::Pause() {

}
//カメラデータ
void SkillAction::GetData() {
}