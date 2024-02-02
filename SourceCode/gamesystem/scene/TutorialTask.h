#pragma once
#include "IKESprite.h"
#include <memory>

using namespace std;
//チュートリアルの進行状況を示すやつ
enum TutorialState {
	TASK_MOVE,
	TASK_GET,
	TASK_ATTACK,
	TASK_COUNTER,
	TASK_BREAK,
};
class TutorialTask {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static TutorialTask* GetInstance();

	//初期化
	void Initialize();

	void Update();

	void Draw();

	void ImGuiDraw();

	void AllFinish();
public:
	//setter getter
	const bool GetTaskFinish(const int TaskNum) { return m_TaskFinish[TaskNum]; }
	const bool GetChoiceSkill() { return m_ChoiceSkill; }
	const bool GetViewSkill() { return m_ViewSkill; }
	void SetTaskFinish(const bool TaskFinish, const int TaskNum) { m_TaskFinish[TaskNum] = TaskFinish; }
	void SetChoiceSkill(const bool ChoiceSkill) { m_ChoiceSkill = ChoiceSkill; }
	void SetViewSkill(const bool ViewSkill) { m_ViewSkill = ViewSkill; }

private:
	static const int TASK_MAX = 5;
private:
	bool m_TaskFinish[TASK_MAX];

	struct TutorialParts {
		unique_ptr<IKESprite> text;
		unique_ptr<IKESprite> check;
		XMFLOAT2 pos;
		XMFLOAT2 Afterpos;
		XMFLOAT4 color = {1.0f,1.0f,1.0f,0.0f};
		XMFLOAT2 size = {};
		XMFLOAT2 aftersize = {50.0f,50.0f};
		float frame = {};
		float posframe = {};
	};

	TutorialParts tutorial[TASK_MAX];
	bool m_ChoiceSkill = false;
	bool m_ViewSkill = false;

	float m_Frame = {};
};