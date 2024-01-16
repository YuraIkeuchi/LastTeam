#pragma once
#include"IKESprite.h"
#include <vector>
#include <memory>
#include <ActionUI.h>
#include <Passive.h>
#include "DrawNumber.h"
#include <TextManager.h>
#include "ResultAreaUI.h"
class HaveResultSkill {
public:
	HaveResultSkill();
	~HaveResultSkill();

	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw(DirectXCommon* dxCommon);
	void ImGuiDraw();
	void Move();
	//持っているスキル
	void HaveAttackSkill(std::vector<int> Deck,
		int DeckSize,DirectXCommon* dxCommon);

	//持っているパッシブ
	void HavePassiveSkill(std::vector<int> Passive,
		int PassiveSize, DirectXCommon* dxCommon);
	void CreateAttackSkill(const int num,const int id, DirectXCommon* dxCommon);
	void CreatePassiveSkill(const int num, const int id, DirectXCommon* dxCommon);
	void BirthArea(const int num);

private:
	void SetDeleteAfter(const int num);
	void SetPassiveDeleteAfter(const int num);
	void DeleteMove();
	bool DeleteCheack();
private:
	static const int S_DAMAGEMAX = 2;
private:

	struct HaveUI {
		std::unique_ptr<IKESprite> icon;
		std::unique_ptr<TextManager> text_;
		vector<std::unique_ptr<ResultAreaUI>> resultarea;
		XMFLOAT2 position = { 640.f,320.f };
		XMFLOAT2 afterpos = { 0.0f,320.0f };
		XMFLOAT2 size = { 128.f,128.f };
		int no = -1;
		int ID = -1;
		bool isSkill = true;
		vector<std::vector<int>> area;	//範囲
		int DisX = {};
		int DisY = {};
		int Delay = 0;
		wchar_t* baseSentence[3] = { L"",L"",L""};
		int Damage = {};
	};
	
private:
	std::unique_ptr<IKESprite> backScreen;
	std::unique_ptr<IKESprite> top_title;
	std::unique_ptr<IKESprite> skillCheack;
	std::unique_ptr<IKESprite> selectFrame;

	std::unique_ptr<IKESprite> black;
	std::unique_ptr<IKESprite> deleteDeck;
	std::unique_ptr<IKESprite> deleteDeckYes[2];
	std::unique_ptr<IKESprite> deleteDeckNo[2];


	std::vector<HaveUI> haveSkills;
	std::vector<HaveUI> havePassive;
	float m_AddPosX = {};
	float m_AfterAddPosX = {};
	int m_SelectCount = {};
	int m_OldCount = 0;
	int m_SelectTimer = {};
	bool m_isMove = false;

	XMFLOAT2 m_SelectPos = {};
	bool m_DeleteMove = false;



	bool m_DeleteStart = false;
	float deleteFrame = 0.f;
	bool m_DeleteCheack = false;
	int nowCheack = 0;
};

