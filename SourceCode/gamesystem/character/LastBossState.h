#pragma once

//ラスボスのステートを管理するクラス
class LastBossState {
public:
	static LastBossState* GetInstance();

public:
	//settergetter
	const int GetLastWidth() { return m_LastWidth; }
	const int GetLastHeight() { return m_LastHeight; }
	void SetBossSpace(const int width, const int height) { m_LastWidth = width, m_LastHeight = height; }
private:
	//ボスが居るマス
	int m_LastWidth = {};
	int m_LastHeight = {};
};