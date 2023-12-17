#pragma once
#include"IKESprite.h"
#include <vector>
#include <memory>
#include <ActionUI.h>
#include <Passive.h>
#include "DrawNumber.h"
#include <TextManager.h>
#include <ResultAreaUI.h>
#include <Player.h>

class ResultSkill {
public:
	ResultSkill();
	~ResultSkill();

	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw(DirectXCommon* dxCommon);
	void ImGuiDraw();
	void InDeck(std::vector<int>& Deck);
	void InPassive(std::vector<int>& Passive);

	void CreateResult(std::vector<int>& notDeck,
		std::vector<int>& notPassives);

	bool GetIsVisible() { return isStart; }
private:
	void Move();
	bool FeedOut();
private:
	static const int S_DAMAGEMAX = 2;
private:
	struct ResultUI{
		std::unique_ptr<IKESprite> icon;
		std::unique_ptr<TextManager> text_;
		XMFLOAT2 position = { 640.f,320.f };
		XMFLOAT2 size = { 128.f,128.f };
		int no = -1;
		int oldNo = -1;
		int ID = -1;
		bool isSkill = true;
		list<std::unique_ptr<ResultAreaUI>> resultarea;
		vector<std::vector<int>> area;	//”ÍˆÍ
		int DisX = {};
		int DisY = {};
		int Damage = {};
		wchar_t* sentence[3] = { L"",L"",L"" };
	};

	struct ShineEffect {
		unique_ptr<IKESprite> tex;
		float frame = 0.f;
 		float frameA = 0.f;
		float kFrame = 30.f;
		XMFLOAT2 position = { 0.f,0.f };
		XMFLOAT2 size = { 0.f,0.f };
		bool isVanish = false;
	};

	struct StarEffect {
		unique_ptr<IKESprite> tex;
		float frame = 0.f;
		float frameA = 0.f;
		float kFrame = 20.f;
		float angle = 0.f;
		float dia = 0.f;
		XMFLOAT2 position = { 0.f,0.f };
		XMFLOAT2 size = { 32.f,32.f };
		bool isVanish = false;
	};
	void RandShineInit();
	void ShineEffectUpdate();
	void StarEffectUpdate();
	void StarInit();
	ResultUI CreateUI(bool isSkill,int id,XMFLOAT2 pos);
	void BirthArea(ResultUI& resultUI);
public:
	void SetPlayer(Player* player) { this->player_ = player; }
	void SetIsBattle(bool isBattle) { this->isBattle = isBattle; }
private:
	bool isStart = false;
	DirectXCommon* dxcommon = nullptr;
	Player* player_=nullptr;
	std::unique_ptr<IKESprite> backScreen;
	std::unique_ptr<IKESprite> skillCheack;
	std::unique_ptr<IKESprite> selectFrame;
	std::unique_ptr<IKESprite> feedIn;
	std::list<ResultUI> choiceSkills;
	int nowPos = 0;
	XMFLOAT2 BasePos[5] = { {200.f,240.f}, {420.f,240.0f},{640.f,240.f},{860.f,240.f},{1080.f,240.f} };
	bool isMove = false;
	float frameA = 0.f;
	XMFLOAT2 framePos = BasePos[2];
	std::list<ResultUI> pickSkills;
	std::list<ShineEffect> shines;
	std::vector<StarEffect> stars = {};
	bool m_Choice = false;
	bool isBattle = false;
};

