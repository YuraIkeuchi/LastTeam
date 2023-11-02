#pragma once

//チュートリアルの進行状況を示すやつ
enum TutorialState {
	TASK_MOVE,
	TASK_BIRTH_BEFORE,
	TASK_BIRTHSKIL,
	TASK_ATTACK,
	TASK_DAMAGE,
};
class TutorialTask {
public:
	static TutorialTask* GetInstance();

	//初期化
	void Initialize();

	void ImGuiDraw();
public:
	//setter getter
	const int GetTutorialState() { return m_TutorialState; }
	void SetTutorialState(const int TutorialState) { m_TutorialState = TutorialState; }
private:

	int m_TutorialState = TASK_MOVE;
};