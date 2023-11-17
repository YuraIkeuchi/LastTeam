#include "SceneChanger.h"
#include "ImageManager.h"
#include "Helper.h"
#include "Audio.h"

SceneChanger* SceneChanger::GetInstance() {
	static SceneChanger instance;

	return &instance;
}
void (SceneChanger::* SceneChanger::stateTable[])() = {
	&SceneChanger::FeedChange,//�t�F�[�h
	&SceneChanger::WideChange,//���C�h
};
void SceneChanger::Initialize() {
	change = IKESprite::Create(ImageManager::FEED, {});
	change->SetSize({ 1280.0f,720.0f });

	m_ChangeStart = false;
	_ChangeState = CHANGE_START;
	m_AfterAlpha = 1.0f;

	for (auto i = 0; i < WIDTH_NUM; i++) {
		for (auto j = 0; j < HEIGHT_NUM; j++) {
			change2[i][j] = IKESprite::Create(ImageManager::FEED, { 0,0 });
			change2[i][j]->SetAnchorPoint({ 0.5f,0.5f });
			change2[i][j]->SetPosition({ (float)(i * base_size) + base_size / 2,(float)(j * base_size) + base_size / 2 });
			change2[i][j]->SetSize({ 0,0 });
		}
	}

	m_AfterSize = (float)(base_size);
	m_WideSize = {};
}
void SceneChanger::Update() {
	//��Ԉڍs(charastate�ɍ��킹��)
	(this->*stateTable[_ChangeType])();
	change->SetColor(m_Color);

	for (auto i = 0; i < WIDTH_NUM; i++) {
		for (auto j = 0; j < HEIGHT_NUM; j++) {
			change2[i][j]->SetSize(m_WideSize);
			change2[i][j]->SetColor(m_Color2[i][j]);
		}
	}
}
void SceneChanger::InitializeOver() {
}
void SceneChanger::Draw() {
	IKESprite::PreDraw();
	if (_ChangeType == CHANGE_FEED) {
		change->Draw();
	}
	else {
		for (auto i = 0; i < WIDTH_NUM; i++) {
			for (auto j = 0; j < HEIGHT_NUM; j++) {
				change2[i][j]->Draw();
			}
		}
	}
	IKESprite::PostDraw();
}
void SceneChanger::ImGuiDraw() {
	ImGui::Begin("Changer");
	if (_ChangeType == CHANGE_FEED) {
		ImGui::Text("FEED");
	}
	else if (_ChangeType == CHANGE_WIDE) {
		ImGui::Text("WIDE");
	}

	if (ImGui::Button("SETFEED", ImVec2(50, 50))) {
		_ChangeType = CHANGE_FEED;
	}
	if (ImGui::Button("SETWIDE", ImVec2(50, 50))) {
		_ChangeType = CHANGE_WIDE;
	}
	ImGui::End();
}

//���ʂ̃t�F�[�h
void SceneChanger::FeedChange() {
	if (!m_ChangeStart) { return; }
	const float l_AddFrame = 1.f/45.f;
	const int l_MaxTimer = 5;
	m_Color.x = 1.0f;
	if (_ChangeState == CHANGE_START) {			//���X�ɈÂ��Ȃ�
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Change = true;
			_ChangeState = CHANGE_FINISH;
			m_Frame = {};
			m_AfterAlpha = 0.0f;
		}
	}
	else {		//�t�F�[�h�I���A���X�ɖ��邭�Ȃ�
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			m_ChangeStart = false;
			_ChangeState = CHANGE_START;
			m_AfterAlpha = 1.0f;
		}
	}

	m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, m_AfterAlpha);
}
//�L������
void SceneChanger::WideChange() {
	if (!m_ChangeStart) { return; }
	const float l_AddFrame = 1.f/45.f;
	const int l_MaxTimer = 5;
	m_Color.w = 1.0f;
	if (_ChangeState == CHANGE_START) {			//���X�ɈÂ��Ȃ�
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Change = true;
			_ChangeState = CHANGE_FINISH;
			m_Frame = {};
			m_AfterSize = 0.0f;
		}
	}
	else {		//�t�F�[�h�I���A���X�ɖ��邭�Ȃ�
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			m_ChangeStart = false;
			_ChangeState = CHANGE_START;
			m_AfterSize = (float)(base_size);
		}
	}

	m_WideSize = { Ease(In, Quad, m_Frame, m_WideSize.x, m_AfterSize),
			Ease(In, Quad, m_Frame, m_WideSize.y, m_AfterSize)};

	for (auto i = 0; i < WIDTH_NUM; i++) {
		for (auto j = 0; j < HEIGHT_NUM; j++) {
			m_Color2[i][j] = ChangeColor(i, j);
		}
	}
}
//�p�l���̐F�����߂�
XMFLOAT4 SceneChanger::ChangeColor(const int Weight, const int Height) {
	XMFLOAT4 color;
	if (Weight % 2 == 0) {
		if (Height % 2 == 0) {
			color = { 0.8f,0.8f,0.8f,1.0f };
		}
		else {
			color = { 0.3f,0.3f,0.3f,1.0f };
		}
	}
	else {
		if (Height % 2 != 0) {
			color = { 0.8f,0.8f,0.8f,1.0f };
		}
		else {
			color = { 0.3f,0.3f,0.3f,1.0f };
		}
	}

	return color;
}