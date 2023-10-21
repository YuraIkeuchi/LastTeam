#pragma once

//ゲームの状態を管理するクラス
class GameStateManager {
public:
	static GameStateManager* GetInstance();
	//初期化
	void Initialize();
	//更新
	void Update();

	//ImGui
	void ImGuiDraw();

public:
	//gettersetter
	const bool GetCounter() { return m_Counter; }
	const float GetPosScore() { return m_PosScore; }
	const float GetGrazeScore() { return m_GrazeScore; }

	void SetCounter(const bool isCounter) { this->m_Counter = isCounter; }
	void SetPosScore(const float PosScore) { this->m_PosScore = PosScore; }
	void SetGrazeScore(const float GrazeScore) { this->m_GrazeScore = GrazeScore; }
private:
	//カウンター
	bool m_Counter = false;
	int m_CounterTimer = {};
	int m_CounterScore = {};

	//位置のスコア
	float m_PosScore = {};
	//グレイズのスコア(後々intにする)
	float m_GrazeScore = 0.0f;

	//全体スコア
	int m_AllScore = {};
};