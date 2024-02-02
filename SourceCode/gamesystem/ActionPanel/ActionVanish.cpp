#include "ActionVanish.h"
#include "ImageManager.h"
#include "Easing.h"
#include <GameStateManager.h>
#include <Helper.h>
ActionVanish::ActionVanish() {
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
		m_Size = { 128.0f,128.0f };
		vanishTexs[i]->SetSize({ 128.0f,128.0f });
		vanishTexs[i]->SetPosition({ -100.0f,640.0f });
	}
}
//初期化
void ActionVanish::Initialize() {
	m_Position = { (16.f + 64.f),640.0f };

}
//ステータス初期化
void ActionVanish::InitState() {
	m_Alive = true;
	m_Position = { (16.f + 64.f),640.0f };
	vanishTiming = 0;
	vanishCount = 0;
}
//更新
void ActionVanish::Update() {
	UiMove();
	for (int i = 0; i < 10; i++) {
		vanishTexs[i]->SetPosition(m_Position);
		vanishTexs[i]->SetSize(m_Size);
		vanishTexs[i]->SetColor(m_Color);
	}
}
//描画
void ActionVanish::Draw() {
	IKESprite::PreDraw();
	vanishTexs[vanishCount]->Draw();
	IKESprite::PostDraw();
}
//ImGui
void ActionVanish::ImGuiDraw() {
}
//UIの動き
void ActionVanish::UiMove() {
	if (!m_Alive) { return; }
	const int kVanishMax = 30;
	float l_frame = (float)vanishTiming / (float)kVanishMax;
	l_frame = clamp(l_frame,0.f,1.f);
	if (l_frame >= 1.f) {
		m_Alive = false;
	}
	vanishTiming++;
	if ((vanishTiming % 3) == 0) {
		vanishCount++;
		Helper::Clamp(vanishCount, 0, 9);
	}
	m_Position.y = Ease(In, Cubic, l_frame, 640.0f, 500.0f);
	m_Color.w = Ease(In, Cubic, l_frame, 1.f, 0.0f);
}