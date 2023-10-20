#include "ActionUI.h"
#include "ImageManager.h"
#include "Easing.h"
#include <Player.h>
#include <Helper.h>
ActionUI::ActionUI() {
	const int texCount = TEX_MAX;
	const float l_Width_Cut = 64.0f;
	const float l_Height_Cut = 64.0f;
	for (int i = 0; i < tex.size(); i++) {
		//ひとけた目
		tex[i] = IKESprite::Create(ImageManager::ACTIONUI, { 0.0f,0.0f });
		int tex_index_y = i / texCount;
		int tex_index_x = i % texCount;
		tex[i]->SetTextureRect(
			{ static_cast<float>(tex_index_x) * l_Width_Cut, static_cast<float>(tex_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		tex[i]->SetAnchorPoint({ 0.5f,0.5f });
		tex[i]->SetPosition({ -100.0f,800.0f });
		tex[i]->SetSize({ l_Width_Cut,l_Height_Cut });
	}
}
//初期化
void ActionUI::Initialize() {
	m_Position = { (32.0f + ((m_ActCount - 1) * 64.0f)),800.0f };
	m_Use = false;
	m_Frame = {};
	m_Alive = true;
}
//ステータス初期化
void ActionUI::InitState(const int ActCount,const string& Tag) {
	m_ActCount = ActCount;
	if (Tag == "Attack") {
		m_ActType = ACT_ATTACK;
	}
	else if (Tag == "Skill") {
		m_ActType = ACT_ATTACK;
	}
	else if (Tag == "Skill") {
		m_ActType = ACT_GUARD;
	}
	else {
		assert(0);
	}
}
//更新
void ActionUI::Update() {
	UiMove();
	
	tex[m_ActType]->SetColor(m_Color);
	tex[m_ActType]->SetPosition(m_Position);
}
//描画
void ActionUI::Draw() {
	IKESprite::PreDraw();
	tex[m_ActType]->Draw();
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
	const float l_AddFrame = 0.1f;
	m_Position.x = Ease(In, Cubic, 0.5f, m_Position.x, (32.0f + ((m_ActCount) * 64.0f)));
	if (!m_Use) {
		m_Position.y = Ease(In, Cubic, 0.5f, m_Position.y, 690.0f);
	}
	else {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
		}
		m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 630.0f);
		m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 0.0f);
	}
}