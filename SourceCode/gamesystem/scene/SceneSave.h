#pragma once

enum SeceneCategory {
	kTutorialStage=0,
	kFirstStage,
	kSecondStage,
	kThirdStage,
	kFourthStage,
	kFiveStage,
	kSixStage,
	kSevenStage,
	kMaxStage
};


class SceneSave {
public:
	static SceneSave* GetInstance();

	void AllReset();

	void SetClearFlag(SeceneCategory sceneCategory,const bool flag);

	bool GetClearFlag(SeceneCategory sceneCategory);

	void SetLoseFlag(SeceneCategory sceneCategory, const bool flag);

	void SetLoseReset();
	bool GetLoseFlag(SeceneCategory sceneCategory);

	bool AllClear();

	void ImGuiDraw();

	void SetTitF(bool f) { TitF = f; }
	bool GetTitF() { return TitF; }
public:
	bool GetEndRoll() { return m_EndRoll; }

	void SetEndRoll(const bool EndRoll) { m_EndRoll = EndRoll; }
private:
	bool TitF;

	static bool m_StageClear[(size_t)kMaxStage];

	static bool m_StageLose[(size_t)kMaxStage];

	bool m_ClearCheck[(size_t)kMaxStage] = { false };

	int m_ClearCount = {};

	bool m_EndRoll = false;
};

