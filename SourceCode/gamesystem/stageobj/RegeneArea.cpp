#include "RegeneArea.h"
#include <StagePanel.h>
#include <ImageManager.h>
#include <GameStateManager.h>
#include <Helper.h>
//�ǂݍ���
RegeneArea::RegeneArea() {
	panels.tex = std::make_unique<IKETexture>(ImageManager::HEALAREA, XMFLOAT3{}, XMFLOAT3{ 1.f,1.f,1.f }, XMFLOAT4{ 1.f,1.f,1.f,1.f });
	panels.tex->TextureCreate();
	panels.tex->Initialize();
	m_AfterScale = PANEL_SIZE * 0.1f;
	panels.tex->SetScale({ 0.0f,0.0f,0.0f });
	panels.tex->SetRotation({ 90.0f,0.0f,0.0f });

	Initialize();
}
//������
bool RegeneArea::Initialize() {
	return true;
}

//�X�e�[�^�X������
void RegeneArea::InitState(const int width, const int height) {
	panels.position = SetPanelPos(width, height);
	panels.color = { 1.f,1.f,1.f,1.f };
	m_NowWidth = width, m_NowHeight = height;
	panels.position.y = 0.03f;
	m_Alive = true;
	StagePanel::GetInstance()->SetHeal(m_NowWidth, m_NowHeight, true);
}

//�X�V
void RegeneArea::Update() {
	const int l_TargetTimer = 300;
	if (Helper::CheckMin(m_BirthTimer, l_TargetTimer, 1)) {
		if (Helper::CheckMax(panels.color.w,0.0f, -0.05f)) {
			m_Alive = false;
			StagePanel::GetInstance()->SetHeal(m_NowWidth, m_NowHeight, false);
		}
	}
	panels.scale = { Ease(In,Cubic,0.5f,panels.scale.x,m_AfterScale),
	Ease(In,Cubic,0.5f,panels.scale.y,m_AfterScale),
	Ease(In,Cubic,0.5f,panels.scale.z,m_AfterScale) };

	panels.tex->Update();
	panels.tex->SetScale(panels.scale);
	panels.tex->SetPosition(panels.position);
	panels.tex->SetColor(panels.color);
}
//�`��
void RegeneArea::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	panels.tex->Draw();
	IKETexture::PostDraw();
}
//ImGui
void RegeneArea::ImGuiDraw() {
	ImGui::Begin("Regene");
	ImGui::Text("Timer:%d", m_BirthTimer);
	ImGui::End();
}
//�p�l���̈ʒu�ɒu��
XMFLOAT3 RegeneArea::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}