#include "SceneChanger.h"
#include "ImageManager.h"
#include "Helper.h"
#include "Audio.h"

SceneChanger* SceneChanger::GetInstance() {
	static SceneChanger instance;

	return &instance;
}

void SceneChanger::Initialize() {
	change = IKESprite::Create(ImageManager::FEED, {});
	change->SetSize({ 1280.0f,720.0f });

	m_ChangeStart = false;
	_FeedState = FEED_START;
	m_AfterAlpha = 1.0f;
}
void SceneChanger::Update() {
	const float l_AddFrame = 0.01f;
	const int l_MaxTimer = 5;
	if (m_ChangeStart) {
		if (_FeedState == FEED_START) {			//徐々に暗くなる
			if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
				if (Helper::GetInstance()->CheckMin(m_ChangeTimer,l_MaxTimer,1)) {
					m_Change = true;
					_FeedState = FEED_FINISH;
					m_Frame = {};
					m_ChangeTimer = {};
					m_AfterAlpha = 0.0f;
				}
			}
		}
		else {		//フェード終わり、徐々に明るくなる
			if (Helper::GetInstance()->FrameCheck(m_Frame, l_AddFrame)) {
				m_Frame = {};
				m_ChangeStart = false;
				_FeedState = FEED_START;
			}
		}

		m_Color.w = Ease(In, Cubic, m_Frame, m_Color.w, m_AfterAlpha);
	}

	change->SetColor(m_Color);
}
void SceneChanger::InitializeOver() {
}
void SceneChanger::Draw() {
	change->Draw();
}
void SceneChanger::ImGuiDraw() {
	ImGui::Begin("Changer");
	ImGui::Text("Frame:%f", m_Frame);
	ImGui::Text("ColorW:%f", m_Color.w);
	ImGui::End();
}