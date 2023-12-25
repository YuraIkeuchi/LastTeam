#pragma once

//チュートリアルの進行状況を示すやつ
enum TutorialState {
	TASK_MOVE,
	TASK_GET,
	TASK_ATTACK,
	TASK_BREAK,
};
class TutorialTask {
public:
	static TutorialTask* GetInstance();

	//初期化
	void Initialize();

	void ImGuiDraw();
public:
	//setter getter
	const bool GetTaskFinish(const int TaskNum) { return m_TaskFinish[TaskNum]; }
	const bool GetChoiceSkill() { return m_ChoiceSkill; }
	const bool GetViewSkill() { return m_ViewSkill; }
	void SetTaskFinish(const bool TaskFinish, const int TaskNum) { m_TaskFinish[TaskNum] = TaskFinish; }
	void SetChoiceSkill(const bool ChoiceSkill) { m_ChoiceSkill = ChoiceSkill; }
	void SetViewSkill(const bool ViewSkill) { m_ViewSkill = ViewSkill; }

private:
	static const int TASK_MAX = 4;
private:
	bool m_TaskFinish[TASK_MAX];
	bool m_ChoiceSkill = false;
	bool m_ViewSkill = false;
};