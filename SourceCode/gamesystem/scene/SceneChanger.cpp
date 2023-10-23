#include "SceneChanger.h"
#include "ImageManager.h"
#include "Helper.h"
#include "Audio.h"

SceneChanger* SceneChanger::GetInstance() {
	static SceneChanger instance;

	return &instance;
}
void (SceneChanger::* SceneChanger::stateTable[])() = {
	&SceneChanger::FeedChange,//フェード
	&SceneChanger::WideChange,//ワイド
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
	//状態移行(charastateに合わせる)
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

//普通のフェード
void SceneChanger::FeedChange() {
	if (!m_ChangeStart) { return; }
	const float l_AddFrame = 0.01f;
	const int l_MaxTimer = 5;
	m_Color.x = 1.0f;
	if (_ChangeState == CHANGE_START) {			//徐々に暗くなる
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Change = true;
			_ChangeState = CHANGE_FINISH;
			m_Frame = {};
			m_AfterAlpha = 0.0f;
		}
	}
	else {		//フェード終わり、徐々に明るくなる
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = {};
			m_ChangeStart = false;
			_ChangeState = CHANGE_START;
			m_AfterAlpha = 1.0f;
		}
	}

	m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, m_AfterAlpha);
}
//広がるやつ
void SceneChanger::WideChange() {
	if (!m_ChangeStart) { return; }
	const float l_AddFrame = 0.01f;
	const int l_MaxTimer = 5;
	m_Color.w = 1.0f;
	if (_ChangeState == CHANGE_START) {			//徐々に暗くなる
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
			m_Change = true;
			_ChangeState = CHANGE_FINISH;
			m_Frame = {};
			m_AfterSize = 0.0f;
		}
	}
	else {		//フェード終わり、徐々に明るくなる
		if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
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
			if (i % 2 == 0) {
				if (j % 2 == 0) {
					m_Color2[i][j] = { 0.8f,0.8f,0.8f,1.0f };
				}
				else {
					m_Color2[i][j] = { 0.3f,0.3f,0.3f,1.0f };
				}
			}
			else {
				if (j % 2 != 0) {
					m_Color2[i][j] = { 0.8f,0.8f,0.8f,1.0f };
				}
				else {
					m_Color2[i][j] = { 0.3f,0.3f,0.3f,1.0f };
				}
			}
		}
	}
}