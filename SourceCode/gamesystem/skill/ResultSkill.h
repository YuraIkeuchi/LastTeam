#pragma once
#include"IKESprite.h"
#include <vector>
#include <memory>
#include <ActionUI.h>
#include <Passive.h>
#include "DrawNumber.h"
#include <TextManager.h>
#include <ResultAreaUI.h>

class ResultSkill {
public:
	ResultSkill();
	~ResultSkill();

	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw(DirectXCommon* dxCommon);

	void InDeck(std::vector<int>& Deck);
	void InPassive(std::vector<int>& Passive);

	void CreateResult(std::vector<int>& notDeck,
		std::vector<int>& notPassives);

	bool GetIsVisible() { return isStart; }
private:
	void Move();
private:
	unique_ptr<TextManager> resulttext;
	struct ResultUI{
		std::unique_ptr<IKESprite> icon;
		std::unique_ptr<DrawNumber> number;
		XMFLOAT2 position = { 640.f,320.f };
		XMFLOAT2 size = { 128.f,128.f };
		int no = -1;
		int ID = -1;
		bool isSkill = true;
		list<std::unique_ptr<ResultAreaUI>> resultarea;
		vector<std::vector<int>> area;	//”ÍˆÍ
		int DisX = {};
		int DisY = {};
	};

	ResultUI CreateUI(bool isSkill,int id,XMFLOAT2 pos);
	void BirthArea(ResultUI& resultUI);
private:
	bool isStart = false;
	std::unique_ptr<IKESprite> backScreen;
	std::unique_ptr<IKESprite> selectFrame;
	std::list<ResultUI> choiceSkills;
	int nowPos = 0;
	XMFLOAT2 BasePos[3] = { {320.f,240.0f},{640.f,240.f},{960.f,240.f} };
	wchar_t* baseSentence[3] = {L"",L"",L""};
	bool isMove = false;
	int nowFrame = 0;
	int oldFrame = 0;
	XMFLOAT2 framePos = BasePos[0];
	std::list<ResultUI> pickSkills;

};

