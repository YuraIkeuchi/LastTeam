#include "AttackArea.h"
#include <StagePanel.h>
#include <ImageManager.h>
#include <GameStateManager.h>
#include <Helper.h>
//読み込み
AttackArea::AttackArea() {
	panels.tex.reset(new IKETexture(ImageManager::AREA, {}, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	panels.tex->TextureCreate();
	panels.tex->Initialize();
	panels.tex->SetScale({ 0.2f,0.2f,0.2f });
	panels.tex->SetRotation({ 90.0f,0.0f,0.0f });
	Initialize();
}
//初期化
bool AttackArea::Initialize() {
	return true;
}

//ステータス初期化
void AttackArea::InitState(const int width, const int height) {
	panels.position = SetPanelPos(width, height);
	panels.color = { 1.f,1.f,1.f,1.f };
	m_NowWidth = width, m_NowHeight = height;
	panels.position.y = 0.03f;
	m_Alive = true;
	m_Hit = false;
	m_AliveTimer = {};
}

//更新
void AttackArea::Update() {
	const int l_TargetTimer = 10;

	if (Helper::GetInstance()->CheckMin(m_AliveTimer, l_TargetTimer, 1)) {
		GameStateManager::GetInstance()->SetBuff(false);
		m_Alive = false;
	}

	panels.color = { 1.0f,0.3f,0.0f,1.0f };
	panels.tex->Update();
	panels.tex->SetPosition(panels.position);
	panels.tex->SetColor(panels.color);
}
//描画
void AttackArea::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	panels.tex->Draw();
	IKETexture::PostDraw();
}
//ImGui
void AttackArea::ImGuiDraw() {

}
//パネルの位置に置く
XMFLOAT3 AttackArea::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}