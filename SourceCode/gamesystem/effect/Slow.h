#pragma once
//ヒットストップようなのでリソースは要らない
class Slow {
public:
	static Slow* GetInstance();
	void Initialize();
	void LoadCSV();
	//更新処理のみ
	void Update();
	void ImGuiDraw();
public:
	//getter setter
	bool GetSlow() { return  m_Slow; }
	void SetSlow(bool Slow) { this->m_Slow = Slow; }
private:
	bool m_Slow = false;
};