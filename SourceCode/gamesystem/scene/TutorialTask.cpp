#include "TutorialTask.h"
#include <ImageManager.h>
#include "imgui.h"
#include <Helper.h>
#include <Easing.h>
TutorialTask* TutorialTask::GetInstance() {
	static TutorialTask instance;

	return &instance;
}

void TutorialTask::Initialize() {
	for (int i = 0; i < TASK_MAX; i++) {
		m_TaskFinish[i] = false;
	}

	const int TaskCount = TASK_MAX;
	const float l_Width_Cut = 400.0f;
	const float l_Height_Cut = 96.0f;
	for (int i = 0; i < TASK_MAX; i++) {
		tutorial[i].text = IKESprite::Create(ImageManager::TUTORIAL_TASK, { 0.0f,0.0f });
		int number_index_y = i / TaskCount;
		int number_index_x = i % TaskCount;
		tutorial[i].text->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		tutorial[i].text->SetAnchorPoint({ 0.5f,0.5f });
		tutorial[i].pos = { 1020.0f,200 + (80.0f * i) };
		tutorial[i].text->SetSize({ l_Width_Cut,l_Height_Cut });
		tutorial[i].text->SetPosition(tutorial[i].pos);
		tutorial[i].text->SetScale({ 0.8f });

		tutorial[i].check = IKESprite::Create(ImageManager::TUTORIAL_CHECK, { 0.0f,0.0f });
		tutorial[i].check->SetPosition({ 860.0f,200 + (80.0f * i) });
		tutorial[i].check->SetAnchorPoint({ 0.5f,0.5f });
		tutorial[i].color = { 1.0f,1.0f,1.0f,0.0f };
		tutorial[i].size = { 150.0f,150.0f };
		tutorial[i].frame = {};
	}

	m_ChoiceSkill = false;
	m_ViewSkill = false;
}
void TutorialTask::Update(){
	const float l_AddFrame = 1.0f / 30.0f;
	for (int i = 0; i < TASK_MAX; i++) {
		if (m_TaskFinish[i]) {
			if (Helper::FrameCheck(tutorial[i].frame, l_AddFrame)) {
				tutorial[i].frame = 1.0f;
			}
			else {
				tutorial[i].color.w = Ease(In, Cubic, tutorial[i].frame, tutorial[i].color.w, 1.0f);
				tutorial[i].size = { Ease(In,Cubic,tutorial[i].frame,tutorial[i].size.x,tutorial[i].aftersize.x),
				Ease(In,Cubic,tutorial[i].frame,tutorial[i].size.y,tutorial[i].aftersize.y) };
			}
		}

		tutorial[i].check->SetSize(tutorial[i].size);
		tutorial[i].check->SetColor(tutorial[i].color);
	}
}

void TutorialTask::Draw() {
	IKESprite::PreDraw();
	for (int i = 0; i < TASK_MAX; i++) {
		tutorial[i].text->Draw();
		tutorial[i].check->Draw();
	}
	IKESprite::PostDraw();
}
//ImGui
void TutorialTask::ImGuiDraw() {
	ImGui::Begin("Task");
	for (int i = 0; i < TASK_MAX; i++) {
		ImGui::Text("Task[%d]:%d",i, m_TaskFinish[i]);
	}
	ImGui::End();
}