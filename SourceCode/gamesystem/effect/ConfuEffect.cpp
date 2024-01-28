#include "ConfuEffect.h"
#include "ImageManager.h"
#include "Helper.h"
#define WindowW 1920
#define WindowH 1020
//�ǂݍ���
ConfuEffect::ConfuEffect() {

	for (std::size_t i = 0; i < stuntex.size(); i++) {
		stuntex[i].reset(IKETexture::Create(ImageManager::STUN, { 0,0,0 }, { 0.5f,0.5f,0.5f }, { 1,1,1,1 }));
		stuntex[i]->TextureCreate();
		stuntex[i]->SetRotation({ 0,0,0 });
		stuntex[i]->SetScale({ 0.05f,0.05f,0.05f });
	}
}
//������
void ConfuEffect::Initialize() {
	for (int i = 0; i < stuntex.size(); i++) {
		m_Stunscale[i] = 1.0f;
		m_StunSpeed[i] = i * 90.0f;
		m_StunCircleX[i] = {};
		m_StunCircleZ[i] = {};
		m_StunPos[i] = {};
		m_Stunradius[i] = {};
	}
}
//�X�V
void ConfuEffect::Update() {
	//�҂�҂�
	StunEffect();
}
//�`��
void ConfuEffect::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	for (std::size_t i = 0; i < stuntex.size(); i++) {
		stuntex[i]->Draw();
	}
	IKETexture::PostDraw();
}
//ImGui
void ConfuEffect::ImGuiDraw() {
	ImGui::Text("Stun");
	ImGui::Text("Alive:%d", m_Alive);
	ImGui::Text("Speed:%f", m_StunSpeed[0]);
	ImGui::End();
}

//�X�^�����G�t�F�N�g
void ConfuEffect::StunEffect() {
	//�X�^�����̂҂�҂�
	for (int i = 0; i < stuntex.size(); i++) {
		if (m_Alive) {
			m_StunSpeed[i] += 2.0f;
		}
		m_Stunradius[i] = m_StunSpeed[i] * PI / 180.0f;
		m_StunCircleX[i] = cosf(m_Stunradius[i]) * m_Stunscale[i];
		m_StunCircleZ[i] = sinf(m_Stunradius[i]) * m_Stunscale[i];
		m_StunPos[i].x = m_StunCircleX[i] + m_BasePos.x;
		m_StunPos[i].z = m_StunCircleZ[i] + m_BasePos.z;
		m_StunPos[i].y = m_BasePos.y + 2.0f;
		stuntex[i]->SetPosition(m_StunPos[i]);
		stuntex[i]->Update();
		stuntex[i]->SetIsBillboard(true);
	}
}