#include "ActionUI.h"
#include "ImageManager.h"
#include "Easing.h"
#include <GameStateManager.h>
#include <Helper.h>
ActionUI::ActionUI() {
	//�J�[�h
	tex = IKESprite::Create(ImageManager::ACTIONUI, { 0.0f,0.0f });
	tex->SetAnchorPoint({ 0.5f,0.5f });
	tex->SetPosition({ -100.0f,800.0f });

	//ID�p�̃X�v���C�g
	_drawnumber = make_unique<DrawNumber>();
	_drawnumber->Initialize();
}
//������
void ActionUI::Initialize() {
	m_Position = { (32.0f + ((m_ActCount - 1) * 64.0f)),800.0f };
	m_Use = false;
	m_Frame = {};
	m_Alive = true;
}
//�X�e�[�^�X������
void ActionUI::InitState(const int ActCount, const int ID) {
	m_ActCount = ActCount;
	m_ID = ID;
	m_ActType = ACT_ATTACK;
}
//�X�V
void ActionUI::Update() {
	UiMove();

	tex->SetColor(m_Color);
	tex->SetPosition(m_Position);

	_drawnumber->SetPosition(m_Position);
	_drawnumber->SetNumber(m_ID);
	_drawnumber->Update();
}
//�`��
void ActionUI::Draw() {
	IKESprite::PreDraw();
	tex->Draw();
	_drawnumber->Draw();
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
//UI�̓���
void ActionUI::UiMove() {
	const float l_AddFrame = 0.1f;
	m_Position.x = Ease(In, Cubic, 0.5f, m_Position.x, (20.f + 32.f + ((m_ActCount) * 64.0f)));
	if (!m_Use) {
		m_Position.y = Ease(In, Cubic, 0.5f, m_Position.y, 680.0f);
	} else {
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Alive = false;
		}
		m_Position.y = Ease(In, Cubic, m_Frame, m_Position.y, 630.0f);
		m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, 0.0f);
	}
}