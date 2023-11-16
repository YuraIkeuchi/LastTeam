#include "PoisonArea.h"
#include <StagePanel.h>
#include <ImageManager.h>
#include <GameStateManager.h>
#include <Helper.h>
//�ǂݍ���
PoisonArea::PoisonArea() {
	panels.tex.reset(new IKETexture(ImageManager::AREA, {}, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	panels.tex->TextureCreate();
	panels.tex->Initialize();
	panels.tex->SetScale({ 0.2f,0.2f,0.2f });
	panels.tex->SetRotation({ 90.0f,0.0f,0.0f });
	Initialize();
}
//������
bool PoisonArea::Initialize() {
	return true;
}

//�X�e�[�^�X������
void PoisonArea::InitState(const int width, const int height) {
	panels.position = SetPanelPos(width, height);
	panels.Width = width, panels.Height = height;
	panels.position.y = 0.011f;
	panels.color = { 0.5f,0.0f,0.5f,1.0f };
	panels.Alive = true;
	panels.Timer = {};
}

//�X�V
void PoisonArea::Update() {
	const int l_TargetTimer = 300;

	if (Helper::GetInstance()->CheckMin(panels.Timer, l_TargetTimer, 1)) {
		GameStateManager::GetInstance()->SetBuff(false);
		panels.Alive = false;
	}
	Collide();
	panels.tex->Update();
	panels.tex->SetPosition(panels.position);
	panels.tex->SetColor(panels.color);
}
//�`��
void PoisonArea::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	panels.tex->Draw();
	IKETexture::PostDraw();
}
//ImGui
void PoisonArea::ImGuiDraw() {
	ImGui::Begin("Poison");
	ImGui::Text("PosX:%f,PosZ:%f", panels.position.x, panels.position.z);
	ImGui::Text("Timer:%d", panels.DamageTimer);
	ImGui::End();
}
//�p�l���̈ʒu�ɒu��
XMFLOAT3 PoisonArea::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}
void PoisonArea::Collide() {
	int l_PlayerWidth = player->GetNowWidth();
	int l_PlayerHeight = player->GetNowHeight();
	const float l_Damage = 10.0f;
	//�ł̃}�X�ƃv���C���[���ꏏ���ƃ_���[�W��H�炤
	if (panels.Width == l_PlayerWidth && panels.Height == l_PlayerHeight) {
		panels.DamageTimer++;
	}
	else {
		panels.DamageTimer = {};
	}

	//
	if (panels.DamageTimer == 50) {
		player->RecvDamage(l_Damage,"POISON");
		panels.DamageTimer = {};
	}
}