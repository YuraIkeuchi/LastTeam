#pragma once
#include"IKESprite.h"
#include <vector>
#include <memory>
#include <ActionUI.h>
#include <Passive.h>
#include "DrawNumber.h"
#include <TextManager.h>

class HaveResultSkill {
public:
	HaveResultSkill();
	~HaveResultSkill();

	void Initialize();
	void Update();
	void Draw();
	void ImGuiDraw();

	//持っているスキル
	void HaveAttackSkill(std::vector<int> Deck,
		int DeckSize);
	//持っているパッシブ
	void HavePassiveSkill(std::vector<int> Passive,
		int PassiveSize);
	void CreateAttackSkill(int num,int id);
	void CreatePassiveSkill(int num, int id);
private:
private:

	struct HaveUI {
		std::unique_ptr<IKESprite> icon;
		std::unique_ptr<DrawNumber> number;
		XMFLOAT2 position = { 640.f,320.f };
		XMFLOAT2 size = { 128.f,128.f };
		int no = -1;
		int ID = -1;
		bool isSkill = true;
	};

private:
	unique_ptr<TextManager> text_;
	std::unique_ptr<IKESprite> backScreen;
	std::unique_ptr<IKESprite> selectFrame;
	std::vector<HaveUI> haveSkills;
	std::vector<HaveUI> havePassive;
};

