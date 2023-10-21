#include <GameStateManager.h>
#include <Helper.h>
GameStateManager* GameStateManager::GetInstance() {
	static GameStateManager instance;
	return &instance;
}
//初期化
void GameStateManager::Initialize() {
	//カウンター
	m_Counter = false;
	m_CounterTimer = {};
	m_CounterScore = {};
	//位置のスコア
	m_PosScore = {};
	//グレイズのスコア(後々intにする)
	m_GrazeScore = 0.0f;

	//全体スコア
	m_AllScore = {};
}
//更新
void GameStateManager::Update() {
	const int l_AddCounterScore = 10;
	m_AllScore = m_CounterScore + (int)(m_PosScore) + (int)(m_GrazeScore);

	//カウンターの処理
	if (m_Counter) {
		if (m_CounterTimer == 0) {		//カウンターのスコアに加算
			m_CounterScore += l_AddCounterScore;
		}
		m_CounterTimer++;

		if (Helper::GetInstance()->CheckMin(m_CounterTimer, 20, 1)) {		//一定フレームでカウンター終了
			m_Counter = false;
			m_CounterTimer = {};
		}
	}
}
//描画
void GameStateManager::ImGuiDraw() {
	ImGui::Begin("GameState");
	ImGui::Text(m_Counter ? "CLEAR" : "NONE");
	ImGui::Text("CounterTimer:%d", m_CounterTimer);
	ImGui::Text("CounteScore:%d", m_CounterScore);
	ImGui::Text("PosScore:%d", (int)(m_PosScore));
	ImGui::Text("Graze:%d", (int)m_GrazeScore);
	ImGui::Text("All:%d", m_AllScore);
	ImGui::End();
}