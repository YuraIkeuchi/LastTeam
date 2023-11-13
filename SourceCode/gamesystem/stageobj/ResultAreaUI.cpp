#include "ResultAreaUI.h"
#include "CsvLoader.h"
#include "ImageManager.h"

//リソース読み込み
ResultAreaUI::ResultAreaUI() {
	panels.sprite = IKESprite::Create(ImageManager::MAP_START, { 0,0 });
	panels.sprite->SetAnchorPoint({ 0.5f,0.5f });
	panels.sprite->SetSize({ 32.0f,32.0f });
}
//初期化
bool ResultAreaUI::Initialize() {
	panels.position = { 200.0f + (30.0f * m_Width),400.0f + (30.0f * m_Height) };
	panels.color = { 1.0f,1.0f,1.0f,1.0f };
	panels.sprite->SetPosition(panels.position);
	//CSV読み込み
	return true;
}

//更新処理
void ResultAreaUI::Update() {
	
}

//描画
void ResultAreaUI::Draw() {
	IKESprite::PreDraw();
	panels.sprite->Draw();
	IKESprite::PostDraw();
}

//ImGui
void ResultAreaUI::ImGuiDraw() {
	
}
