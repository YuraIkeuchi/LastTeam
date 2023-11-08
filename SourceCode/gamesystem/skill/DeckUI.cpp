#include "DeckUI.h"
#include "ImageManager.h"
#include "Easing.h"
#include <GameStateManager.h>
#include <Helper.h>
DeckUI::DeckUI() {
	//カード
	tex = IKESprite::Create(ImageManager::ACTIONUI, { 0.0f,0.0f });
	tex->SetAnchorPoint({ 0.5f,0.5f });
	tex->SetPosition({ -100.0f,800.0f });

	//ID用のスプライト
	_drawnumber = make_unique<DrawNumber>();
	_drawnumber->Initialize();
}
//初期化
void DeckUI::Initialize() {
	m_Position = { 200.0f,500.0f - ((m_ActCount - 1) * 64.0f) };
	m_Use = false;
	m_Frame = {};
	m_Alive = true;
}
//ステータス初期化
void DeckUI::InitState(const int ActCount) {
	m_ActCount = ActCount;
}
//更新
void DeckUI::Update() {
	UiMove();

	tex->SetColor(m_Color);
	tex->SetPosition(m_Position);

	_drawnumber->SetPosition(m_Position);
	_drawnumber->SetNumber(m_ID);
	_drawnumber->Update();
}
//描画
void DeckUI::Draw() {
	IKESprite::PreDraw();
	tex->Draw();
	_drawnumber->Draw();
	IKESprite::PostDraw();
}
//ImGui
void DeckUI::ImGuiDraw() {
	ImGui::Begin("ActUI");
	ImGui::End();
}
//UIの動き
void DeckUI::UiMove() {
	const float l_AddFrame = 0.1f;
	m_Position.y = Ease(In, Cubic, 0.5f, m_Position.y,(455.0f - ((m_ActCount - 1) * 64.0f)));
	if (!m_Use) {
		m_Position.x = Ease(In, Cubic, 0.5f, m_Position.x, 110.0f);
	}
	else {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
		}
		m_Position.x = Ease(In, Cubic, 0.5f, m_Position.x, 300.0f);
		m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 0.0f);
	}
}