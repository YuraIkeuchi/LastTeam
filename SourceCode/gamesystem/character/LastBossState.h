#pragma once

//���X�{�X�̃X�e�[�g���Ǘ�����N���X
class LastBossState {
public:
	static LastBossState* GetInstance();

public:
	//settergetter
	const int GetLastWidth() { return m_LastWidth; }
	const int GetLastHeight() { return m_LastHeight; }
	void SetBossSpace(const int width, const int height) { m_LastWidth = width, m_LastHeight = height; }
private:
	//�{�X������}�X
	int m_LastWidth = {};
	int m_LastHeight = {};
};