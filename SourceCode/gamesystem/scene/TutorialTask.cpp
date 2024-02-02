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
		tutorial[i].Afterpos = { 1500.0f,200 + (80.0f * i) };
		tutorial[i].text->SetSize({ l_Width_Cut,l_Height_Cut });
		tutorial[i].text->SetPosition(tutorial[i].pos);
		tutorial[i].text->SetScale({ 0.8f });

		tutorial[i].check = IKESprite::Create(ImageManager::TUTORIAL_CHECK, { 0.0f,0.0f });
		tutorial[i].check->SetPosition({ tutorial[i].pos.x - 160.0f,200 + (80.0f * i)});
		tutorial[i].check->SetAnchorPoint({ 0.5f,0.5f });
		tutorial[i].color = { 1.0f,1.0f,1.0f,0.0f };
		tutorial[i].size = { 150.0f,150.0f };
		tutorial[i].frame = {};
		tutorial[i].posframe = {};
	}

	m_Frame = {};
	tutorial[TASK_BREAK].pos.x = 1500.0f;
	m_ChoiceSkill = false;
	m_ViewSkill = false;
}
void TutorialTask::Update(){
	const float l_AddFrame = 1.0f / 30.0f;
	for (int i = 0; i < TASK_MAX - 1; i++) {
		if (m_TaskFinish[i]) {
			if (Helper::FrameCheck(tutorial[i].frame, l_AddFrame)) {
				tutorial[i].frame = 1.0f;
				if (Helper::FrameCheck(tutorial[i].posframe, l_AddFrame)) {
					tutorial[i].posframe = 1.0f;
				}
				else {
					tutorial[i].pos.x = Ease(In, Cubic, tutorial[i].posframe, tutorial[i].pos.x, tutorial[i].Afterpos.x);
				}
			}
			else {
				tutorial[i].color.w = Ease(Out, Quad, tutorial[i].frame, tutorial[i].color.w, 1.0f);
				tutorial[i].size = { Ease(Out,Quad,tutorial[i].frame,tutorial[i].size.x,tutorial[i].aftersize.x),
				Ease(Out,Quad,tutorial[i].frame,tutorial[i].size.y,tutorial[i].aftersize.y) };
			}
		}

		tutorial[i].text->SetPosition(tutorial[i].pos);
		tutorial[i].check->SetPosition({ tutorial[i].pos.x - 125.0f,200 + (80.0f * i) });
		tutorial[i].check->SetSize(tutorial[i].size);
		tutorial[i].check->SetColor(tutorial[i].color);
	}

	if (m_TaskFinish[TASK_COUNTER]) {
		if (m_TaskFinish[TASK_BREAK]) {
			if (Helper::FrameCheck(tutorial[TASK_BREAK].frame, l_AddFrame)) {
				tutorial[TASK_BREAK].frame = 1.0f;
				if (Helper::FrameCheck(tutorial[TASK_BREAK].posframe, l_AddFrame)) {
					tutorial[TASK_BREAK].posframe = 1.0f;
				}
				else {
					tutorial[TASK_BREAK].pos.x = Ease(In, Cubic, tutorial[TASK_BREAK].posframe, tutorial[TASK_BREAK].pos.x, tutorial[TASK_BREAK].Afterpos.x);
				}
			}
			else {
				tutorial[TASK_BREAK].pos.x = Ease(In, Cubic, tutorial[TASK_BREAK].frame, tutorial[TASK_BREAK].pos.x, tutorial[TASK_BREAK].Afterpos.x);
				tutorial[TASK_BREAK].color.w = Ease(Out, Quad, tutorial[TASK_BREAK].frame, tutorial[TASK_BREAK].color.w, 1.0f);
				tutorial[TASK_BREAK].size = { Ease(Out,Quad,tutorial[TASK_BREAK].frame,tutorial[TASK_BREAK].size.x,tutorial[TASK_BREAK].aftersize.x),
				Ease(Out,Quad,tutorial[TASK_BREAK].frame,tutorial[TASK_BREAK].size.y,tutorial[TASK_BREAK].aftersize.y) };
			}
		}
		else {
			if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
				m_Frame = 1.0f;
			}
			else {
				tutorial[TASK_BREAK].pos.x = Ease(In, Cubic, m_Frame, tutorial[TASK_BREAK].pos.x,1020.0f);
			}
		}
	}

	tutorial[TASK_BREAK].text->SetPosition(tutorial[TASK_BREAK].pos);
	tutorial[TASK_BREAK].check->SetPosition({ tutorial[TASK_BREAK].pos.x - 125.0f,520.0f });
	tutorial[TASK_BREAK].check->SetSize(tutorial[TASK_BREAK].size);
	tutorial[TASK_BREAK].check->SetColor(tutorial[TASK_BREAK].color);
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
	ImGui::Text("ViewSkill%d", m_ViewSkill);
	ImGui::End();
}
void TutorialTask::AllFinish() {
	for (int i = 0; i < TASK_MAX; i++) {
		m_TaskFinish[i] = true;
	}
}