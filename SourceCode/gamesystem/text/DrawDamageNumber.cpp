#include "DrawDamageNumber.h"
#include <ImageManager.h>
#include <Helper.h>
#include <Easing.h>

DrawDamageNumber::DrawDamageNumber() {
	const int NumberCount = NUMBER_MAX;
	const float l_Width_Cut = 64.0f;
	const float l_Height_Cut = 64.0f;

	for (auto i = 0; i < NUMBER_MAX; i++) {
		//�ǂݍ���
		_Number[i] = IKESprite::Create(ImageManager::DAMAGENUMBER, { 0.0f,0.0f });
		int number_index_y = i / NumberCount;
		int number_index_x = i % NumberCount;
		_Number[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		_Number[i]->SetAnchorPoint({ 0.5f,0.5f });
		_Number[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		_Number[i]->SetScale(0.5f);
	}
}

//������
void DrawDamageNumber::Initialize() {
	m_AfterPos = { m_Position.x,m_Position.y - 120.0f };
	m_Alive = true;
}
//�X�V
void DrawDamageNumber::Update() {
	for (int i = 0; i < _Number.size(); i++) {
		_Number[i]->SetPosition(m_Position);
		_Number[i]->SetSize(m_Size);
	}

	NumberMove();
}
//�`��
void DrawDamageNumber::Draw() {
	_Number[m_Number]->Draw();
}
//ImGui�`��
void DrawDamageNumber::ImGuiDraw() {
	ImGui::Begin("Damage");
	ImGui::Text("SizeX:%f", m_Size.x);
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::End();
}
//�����Ƃ����W�̐ݒ�
void DrawDamageNumber::SetExplain(const XMFLOAT3& pos) {
	//���[���h���W�ɕϊ�����
	XMVECTOR texHPFirst;
	texHPFirst = { pos.x, pos.y, pos.z };
	texHPFirst = Helper::PosDivi(texHPFirst, m_MatView, false);
	texHPFirst = Helper::PosDivi(texHPFirst, m_MatProjection, true);
	texHPFirst = Helper::WDivision(texHPFirst, false);
	texHPFirst = Helper::PosDivi(texHPFirst, m_MatPort, false);

	m_Position = { texHPFirst.m128_f32[0],texHPFirst.m128_f32[1] };
}
//�J�������
void DrawDamageNumber::GetCameraData() {
	Camera* camera = Helper::GetCamera();
	m_MatView = camera->GetViewMatrix();
	m_MatProjection = camera->GetProjectionMatrix();
	m_MatPort = camera->GetViewPort();
}
//����
void DrawDamageNumber::NumberMove() {
	static float addFrame = 1.f / 100.f;
	if (Helper::FrameCheck(m_Frame, addFrame)) {
		m_Alive = false;
	}

	m_Position = { m_Position.x,
	Ease(In,Cubic,m_Frame,m_Position.y,m_AfterPos.y) };

	m_Size = { Ease(In,Cubic,m_Frame,m_Size.x,m_AfterSize),
		Ease(In,Cubic,m_Frame,m_Size.y,m_AfterSize),
	};
}