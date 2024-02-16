#include "ActionUI.h"
#include "ImageManager.h"
#include "Easing.h"
#include <GameStateManager.h>
#include <Helper.h>
ActionUI::ActionUI(const int ID) {
	m_ID = ID;
	//カード
	tex = IKESprite::Create(ImageManager::ATTACK_0 + m_ID, { 0.0f,0.0f });
	tex->SetAnchorPoint({ 0.5f,0.5f });
	tex->SetPosition({ -100.0f,800.0f });
	tex->SetSize({ 64.0f,64.0f });

	const int NumberCount = 10;
	const float l_Width_Cut = 256.0f;
	const float l_Height_Cut = 256.0f;

	for (int i = 0; i < 10; i++) {
		vanishTexs[i] = IKESprite::Create(ImageManager::ATTACK_BACK, { 0.0f,0.0f });
		int number_index_y = i / NumberCount;
		int number_index_x = i % NumberCount;
		vanishTexs[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		vanishTexs[i]->SetAnchorPoint({ 0.5f,0.5f });
		vanishTexs[i]->SetSize({ 64.0f,64.0f });
		vanishTexs[i]->SetPosition({ -100.0f,800.0f });
	}

	//ID用のスプライト
	_drawnumber = make_unique<DrawNumber>(0.5f);
	_drawnumber->Initialize();
}
//初期化
void ActionUI::Initialize() {
	m_Position = { (32.0f + ((m_ActCount - 1) * 64.0f)),800.0f };
	m_Use = false;
	m_Frame = {};
	m_Alive = true;

}
//ステータス初期化
void ActionUI::InitState(const int ActCount, const int Type) {
	m_ActCount = ActCount;
	m_Type = Type;
}
//更新
void ActionUI::Update() {
	UiMove();
	tex->SetColor(m_Color);
	tex->SetSize(m_Size);
	tex->SetPosition(m_Position);

	for (int i = 0; i < 10; i++) {
		vanishTexs[i]->SetPosition(m_Position);
		vanishTexs[i]->SetSize(m_Size);
		vanishTexs[i]->SetColor(m_Color);
	}

	_drawnumber->SetPosition(m_Position);
	_drawnumber->SetNumber(m_ID);
	_drawnumber->Update();
}
//描画
void ActionUI::Draw() {
	IKESprite::PreDraw();
	if (vanishCount < 1) {
		tex->Draw();
	}
	if (m_Use && m_Alive) {
		vanishTexs[vanishCount]->Draw();
	}
	//_drawnumber->Draw();
	IKESprite::PostDraw();
}
//ImGui
void ActionUI::ImGuiDraw() {
	ImGui::Begin("ActUI");
	ImGui::Text("PosX:%f", m_Position.x);
	ImGui::Text("Count:%d", m_ActCount);
	//ImGui::Text("PosY:%f", m_Position.y);
	ImGui::End();
}
//UIの動き
void ActionUI::UiMove() {
	const float l_AddFrame = 1.f / 30.f;
	const float l_size = 128.f;
	if (m_ActCount == 0) {
		m_Position.x = Ease(In, Cubic, 0.5f, m_Position.x, 250.f+(16.f + 64.f));
		float size_ = Ease(Out, Quad, 0.5f, m_Size.x, l_size);
		m_Size = { size_,size_ };
	} else {
		m_Position.x = Ease(In, Cubic, 0.5f, m_Position.x, 250.f + (44.f + 64.f + ((m_ActCount) * 100.0f)));
		float size_ = Ease(Out, Quad, 0.5f, m_Size.x, l_size * 0.75f);
		m_Size = { size_,size_ };
	}
	if (!m_Use) {
		m_Position.y = Ease(In, Cubic, 0.5f, m_Position.y, 160.0f);
	} else {
		//if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
		m_Alive = false;
		//}
		//vanishTiming++;
		//if ((vanishTiming % 3) == 0) {
		//	vanishCount++;
		//	Helper::Clamp(vanishCount, 0, 9);
		//}
		//m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 630.0f);
		//m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 0.0f);
	}
}