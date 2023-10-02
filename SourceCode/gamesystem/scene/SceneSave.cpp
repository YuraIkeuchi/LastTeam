#include "SceneSave.h"
#include <cassert>
#include "imgui.h"
#include "Helper.h"
bool SceneSave::m_StageClear[(size_t)kMaxStage] = { };
bool SceneSave::m_StageLose[(size_t)kMaxStage] = { };

SceneSave* SceneSave::GetInstance() {
	static SceneSave instance;
	return &instance;
}

void SceneSave::AllReset() {
	for (int i = 0; i < kMaxStage; i++) {
		m_StageClear[(size_t)i] = false;
	}

	for (int i = 0; i < kMaxStage; i++) {
		m_StageLose[(size_t)i] = false;
	}
}

void SceneSave::SetClearFlag(SeceneCategory sceneCategory, const bool flag) {
	m_StageClear[(size_t)sceneCategory] = flag;
}

bool SceneSave::GetClearFlag(SeceneCategory sceneCategory) {
	return m_StageClear[(size_t)sceneCategory];
}

void SceneSave::SetLoseFlag(SeceneCategory sceneCategory, const bool flag) {
	m_StageLose[(size_t)sceneCategory] = flag;
}

void SceneSave::SetLoseReset()
{
	for(auto i=0;i<8;i++)
	{
		m_StageLose[i] = false;
	}
}


bool SceneSave::GetLoseFlag(SeceneCategory sceneCategory) {
	return m_StageLose[(size_t)sceneCategory];
}

//全ステージクリア
bool SceneSave::AllClear() {
	m_StageClear[kTutorialStage] = true;
	for (auto i = 0; i < kSevenStage; i++){
		if (m_StageClear[(size_t)i] && !m_ClearCheck[(size_t)i]) {
			m_ClearCount++;
			m_ClearCheck[(size_t)i] = true;
		}
	}

	if (m_ClearCount == 7) {
		return true;
	}
	else {
		return false;
	}
	//if (Helper::GetInstance()->All_OfF(temp, kFirstStage)) {
	//	
	//}
}

void SceneSave::ImGuiDraw() {
}