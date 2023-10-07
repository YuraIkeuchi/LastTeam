#pragma once

enum GameTurn {
	TURN_SET,
	TURN_BATTLE,
};
class GameMode {
public:
	static GameMode* GetInstance();

public:
	//‰Šú‰»
	bool Initialize();
	//XV
	void Update();
	void ImGuiDraw();
public:
	//getter setter
	const int GetGameTurn() { return m_GameTurn; }
	void SetGameTurn(const int GameTurn) { m_GameTurn = GameTurn; }

	const int GetGameTimer() { return m_GameTimer; }
private:
	int m_GameTurn = TURN_SET;
	int m_GameF = 0;
	int m_GameTimer = 0;
};