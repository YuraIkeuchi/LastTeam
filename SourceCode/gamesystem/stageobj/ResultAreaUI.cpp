#include "ResultAreaUI.h"
#include "CsvLoader.h"
#include "ImageManager.h"

//���\�[�X�ǂݍ���
ResultAreaUI::ResultAreaUI() {
	panels.sprite = IKESprite::Create(ImageManager::RESULTATTACKAREA, { 0,0 });
	panels.sprite->SetAnchorPoint({ 0.5f,0.5f });
	panels.sprite->SetSize({ 64.0f,64.0f });

	//�v���C���[
	player_panel = IKESprite::Create(ImageManager::MAP_CHARA, { 0,0 });
	player_panel->SetAnchorPoint({ 0.5f,0.5f });
	player_panel->SetSize({ 64.0f,64.0f });
}
//������
bool ResultAreaUI::Initialize() {
	const XMFLOAT2 l_BasePos = { 120.0f,360.0f+180.0f };
	panels.position = { l_BasePos.x + (60.0f * (m_Width + m_DistanceX)),l_BasePos.y + (60.0f * (m_Height - m_DistanceY)) };
	panels.color = { 1.0f,1.0f,1.0f,1.0f };
	panels.sprite->SetPosition(panels.position);
	player_panel->SetPosition(l_BasePos);
	//CSV�ǂݍ���
	return true;
}

//�X�V����
void ResultAreaUI::Update() {
	
}

//�`��
void ResultAreaUI::Draw() {
	IKESprite::PreDraw();
	panels.sprite->Draw();
	player_panel->Draw();
	IKESprite::PostDraw();
}

//ImGui
void ResultAreaUI::ImGuiDraw() {
	
}
