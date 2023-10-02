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
	
	void ImGuiDraw();
public:
	//getter setter
	const int GetGameTurn() { return m_GameTurn; }

	void SetGameTurn(const int GameTurn) { m_GameTurn = GameTurn; }
private:
	int m_GameTurn = TURN_SET;
};