#pragma once
//�q�b�g�X�g�b�v�悤�Ȃ̂Ń��\�[�X�͗v��Ȃ�
class Slow {
public:
	static Slow* GetInstance();
	void Initialize();
	void LoadCSV();
	//�X�V�����̂�
	void Update();
	void ImGuiDraw();
public:
	//getter setter
	bool GetSlow() { return  m_Slow; }
	void SetSlow(bool Slow) { this->m_Slow = Slow; }
private:
	bool m_Slow = false;
};