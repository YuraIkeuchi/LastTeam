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

	for (auto i = 0; i < width_num; i++) {
		for (auto j = 0; j < height_num; j++) {
			std::unique_ptr<IKESprite> newSprite;
			newSprite = IKESprite::Create(ImageManager::FEED, { 0,0 });
			newSprite->SetAnchorPoint({ 0.5f,0.5f });
			newSprite->SetPosition({ (float)(i * base_size) + base_size / 2,(float)(j * base_size) + base_size / 2 });
			newSprite->SetSize({ 0,0 });
			DirectX::XMFLOAT4 col = { 0.f,0.f,0.f,1 };
			newSprite->SetColor(col);

			change2.push_back(std::move(newSprite));
		}
	}

	m_AfterSize = (float)(base_size);
	m_WideSize = {};
}
void SceneChanger::Update() {
	//状態移行(charastateに合わせる)
	(this->*stateTable[_ChangeType])();
	change->SetColor(m_Color);

	for (std::unique_ptr<IKESprite>& sprite : change2) {
		sprite->SetSize(m_WideSize);
		sprite->SetColor(m_Color);
	}
}
void SceneChanger::InitializeOver() {
}
void SceneChanger::Draw() {
	if (_ChangeType == CHANGE_FEED) {
		change->Draw();
	}
	else {
		for (std::unique_ptr<IKESprite>& sprite : change2) {
			sprite->Draw();
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

	m_WideSize = { Ease(In, Cubic, m_Frame, m_WideSize.x, m_AfterSize),
			Ease(In, Cubic, m_Frame, m_WideSize.y, m_AfterSize)};
}