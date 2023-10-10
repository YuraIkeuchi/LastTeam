#include "ActionUI.h"
#include "ImageManager.h"

ActionUI::ActionUI() {
	const int texCount = TEX_MAX;
	const float l_Width_Cut = 64.0f;
	const float l_Height_Cut = 64.0f;
	for (int i = 0; i < tex.size(); i++) {
		//‚Ð‚Æ‚¯‚½–Ú
		tex[i] = IKESprite::Create(ImageManager::ACTIONUI, { 0.0f,0.0f });
		int tex_index_y = i / texCount;
		int tex_index_x = i % texCount;
		tex[i]->SetTextureRect(
			{ static_cast<float>(tex_index_x) * l_Width_Cut, static_cast<float>(tex_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		tex[i]->SetAnchorPoint({ 0.5f,0.5f });
		tex[i]->SetPosition({ 32.0f,690.0f });
		tex[i]->SetSize({ l_Width_Cut,l_Height_Cut });
	}
}

//‰Šú‰»
void ActionUI::Initialize() {

}
//XV
void ActionUI::Update() {

}
//•`‰æ
void ActionUI::Draw() {
	IKESprite::PreDraw();
	tex[0]->Draw();
}
//ImGui
void ActionUI::ImGuiDraw() {

}