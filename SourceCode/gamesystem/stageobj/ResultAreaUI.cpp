#include "ResultAreaUI.h"
#include "CsvLoader.h"
#include "ImageManager.h"

//リソース読み込み
ResultAreaUI::ResultAreaUI() {
	panels.sprite = IKESprite::Create(ImageManager::MAP_START, { 0,0 });
	panels.sprite->SetAnchorPoint({ 0.5f,0.5f });
	panels.sprite->SetSize({ 32.0f,32.0f });

	//プレイヤー
	player_panel = IKESprite::Create(ImageManager::MAP_CHARA, { 0,0 });
	player_panel->SetAnchorPoint({ 0.5f,0.5f });
	player_panel->SetSize({ 32.0f,32.0f });
}
//初期化
bool ResultAreaUI::Initialize() {
	const XMFLOAT2 l_BasePos = { 200.0f,400.0f };
	panels.position = { l_BasePos.x + (30.0f * (m_Width + m_DistanceX)),l_BasePos.y + (30.0f * (m_Height - m_DistanceY)) };
	panels.color = { 1.0f,1.0f,1.0f,1.0f };
	panels.sprite->SetPosition(panels.position);
	player_panel->SetPosition(l_BasePos);
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
	player_panel->Draw();
	IKESprite::PostDraw();
}

//ImGui
void ResultAreaUI::ImGuiDraw() {
	
}
