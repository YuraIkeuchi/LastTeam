#pragma once

//�Q�[���̏�Ԃ��Ǘ�����N���X
class GameStateManager {
public:
	static GameStateManager* GetInstance();
	//������
	void Initialize();
	//�X�V
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
	//�J�E���^�[
	bool m_Counter = false;
	int m_CounterTimer = {};
	int m_CounterScore = {};

	//�ʒu�̃X�R�A
	float m_PosScore = {};
	//�O���C�Y�̃X�R�A(��Xint�ɂ���)
	float m_GrazeScore = 0.0f;

	//�S�̃X�R�A
	int m_AllScore = {};
};