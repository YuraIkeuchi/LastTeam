#include "HaveResultSkill.h"
#include "Helper.h"
#include "ImageManager.h"
#include <Input.h>
#include <Easing.h>
#include <SkillManager.h>
HaveResultSkill::HaveResultSkill() {

}

HaveResultSkill::~HaveResultSkill() {
}

void HaveResultSkill::Initialize(DirectXCommon* dxCommon) {
	backScreen = IKESprite::Create(ImageManager::RESULTBACKSCREEN, { 0.f,0.f }, { 1.f,1.f, 1.f, 1.0f });
	backScreen->SetSize({ 1280.f,720.f });
	selectFrame = IKESprite::Create(ImageManager::PASSIVE_FRAME, { 200.f,200.f });
	selectFrame->SetAnchorPoint({ 0.5f,0.5f });
	selectFrame->SetSize({ 90.0f,90.0f });
	selectFrame->SetPosition({ 640.0f,150.0f });

	resulttext = make_unique<TextManager>();
	resulttext->Initialize(dxCommon);
}

void HaveResultSkill::Update() {
	for (HaveUI& resultUI : haveSkills) {
		if (resultUI.isSkill) {
			resultUI.number->Update();
		}
	}
	
	for (HaveUI& PassiveUI : havePassive) {
		if (PassiveUI.isSkill) {
			PassiveUI.number->Update();
		}
	}

	//動き
	Move();
}

void HaveResultSkill::Draw(DirectXCommon* dxCommon) {

	IKESprite::PreDraw();
	backScreen->Draw();
	selectFrame->Draw();
	for (HaveUI& resultUI : haveSkills) {
		resultUI.icon->Draw();
		if (resultUI.isSkill) {
			resultUI.number->Draw();
		}
	}
	
	for (HaveUI& PassiveUI : havePassive) {
		PassiveUI.icon->Draw();
	}

	resulttext->TestDraw(dxCommon);
	if (m_SelectCount < (int)(haveSkills.size())) {
		for (auto i = 0; i < haveSkills[m_SelectCount].resultarea.size(); i++) {
			haveSkills[m_SelectCount].resultarea[i]->Draw();
		}
	}

	IKESprite::PostDraw();
}
void HaveResultSkill::ImGuiDraw() {
	ImGui::Begin("Have");
	ImGui::Text("ID:%d", haveSkills[m_SelectCount].ID);
	ImGui::End();
}

//持っているスキルの検索
void HaveResultSkill::HaveAttackSkill(std::vector<int> Deck,
	int DeckSize) {
	haveSkills.resize(DeckSize);
	for (auto i = 0; i < haveSkills.size(); i++) {
		haveSkills[i].ID = Deck[i];
		CreateAttackSkill(i, haveSkills[i].ID);
		SkillManager::GetInstance()->HandResultData(Deck[i], haveSkills[i].area, haveSkills[i].DisX, haveSkills[i].DisY);//IDに応じた攻撃エリア、距離を取得する
		BirthArea(i);		//エリアを作成(持ってるスキル分)
	}
}
//持っているパッシブ
void HaveResultSkill::HavePassiveSkill(std::vector<int> Passive,
	int PassiveSize) {
	havePassive.resize(PassiveSize);
	for (auto i = 0; i < havePassive.size(); i++) {
		havePassive[i].ID = Passive[i];
		CreatePassiveSkill(i, havePassive[i].ID);
	}
}
//攻撃スキルの表示
void HaveResultSkill::CreateAttackSkill(const int num,const int id) {
	XMFLOAT2 l_BasePos = { 640.0f,150.0f };
	haveSkills[num].position = { l_BasePos.x + (num * 100.0f),l_BasePos.y };
	haveSkills[num].icon = IKESprite::Create(ImageManager::ACTIONUI, { 0.0f,0.0f });
	haveSkills[num].icon->SetSize({ 64.0f,64.0f });
	haveSkills[num].icon->SetAnchorPoint({ 0.5f,0.5f });
	haveSkills[num].icon->SetPosition(haveSkills[num].position);
	haveSkills[num].number = make_unique<DrawNumber>();
	haveSkills[num].number->Initialize();
	haveSkills[num].number->SetNumber(haveSkills[num].ID);
	haveSkills[num].number->SetPosition(haveSkills[num].position);
	m_SelectCount = {};
	m_AddPosX = {};

	baseSentence = resulttext->GetSkillSentence(haveSkills[0].ID);
	resulttext->SetCreateSentence(baseSentence);
}

//パッシブスキルの表示
void HaveResultSkill::CreatePassiveSkill(const int num, const int id) {
	XMFLOAT2 l_BasePos = { 640.0f,150.0f };
	havePassive[num].position = { l_BasePos.x + ((num + (int)haveSkills.size()) * 100.0f),l_BasePos.y };
	havePassive[num].icon = IKESprite::Create(ImageManager::PASSIVE_01 + havePassive[num].ID, {0.0f,0.0f});
	havePassive[num].icon->SetSize({ 64.0f,64.0f });
	havePassive[num].icon->SetAnchorPoint({ 0.5f,0.5f });
	havePassive[num].icon->SetPosition(havePassive[num].position);
	havePassive[num].number = make_unique<DrawNumber>();
	havePassive[num].number->Initialize();
	havePassive[num].number->SetNumber(havePassive[num].ID);
	havePassive[num].number->SetPosition(havePassive[num].position);
}
//移動
void HaveResultSkill::Move() {
	Input* input = Input::GetInstance();
	if (m_isMove) {
		static float frame = 0.f;
		static float addFrame = 1.f / 15.f;

		if (Helper::GetInstance()->FrameCheck(frame, addFrame)) {
			baseSentence = resulttext->GetSkillSentence(haveSkills[m_SelectCount].ID);
			resulttext->SetCreateSentence(baseSentence);
			m_isMove = false;
			frame = 0.f;
		}
		else {
			m_AddPosX = Ease(InOut, Circ, frame, m_AddPosX, 100.0f * m_SelectCount);
		}

		for (auto i = 0; i < haveSkills.size(); i++) {
			haveSkills[i].icon->SetPosition({ haveSkills[i].position.x - m_AddPosX,haveSkills[i].position.y });
			haveSkills[i].number->SetPosition({ haveSkills[i].position.x - m_AddPosX,haveSkills[i].position.y });
		}
		for (auto i = 0; i < havePassive.size(); i++) {
			havePassive[i].icon->SetPosition({ havePassive[i].position.x - m_AddPosX,havePassive[i].position.y });
			havePassive[i].number->SetPosition({ havePassive[i].position.x - m_AddPosX,havePassive[i].position.y });
		}
	}

	if (input->TiltPushStick(input->L_LEFT) ||
		input->TiltPushStick(input->L_RIGHT)) {
		if (m_isMove) { return; }
		if (input->TiltPushStick(input->L_RIGHT)) {
			if (m_SelectCount == ((int)(haveSkills.size()) + (int)(havePassive.size())) - 1) { return; }
			m_SelectCount++;
		}
		else {
			if (m_SelectCount == 0) { return; }
			m_SelectCount--;
		}
		m_isMove = true;
	}
}
//エリアの生成
void HaveResultSkill::BirthArea(const int Area) {
	
	for (auto i = 0; i < haveSkills[Area].area.size(); i++) {
		for (auto j = 0; j < haveSkills[Area].area.size(); j++) {		
			if (haveSkills[Area].area[i][j] == 1) {		//マップチップ番号とタイルの最大数、最小数に応じて描画する
				std::unique_ptr<ResultAreaUI> newarea = std::make_unique<ResultAreaUI>();
				newarea->SetPanelNumber(i, j);
				newarea->SetDistance(haveSkills[Area].DisX, haveSkills[Area].DisY);
				newarea->Initialize();
				haveSkills[Area].resultarea.push_back(std::move(newarea));
			}
		}
	}
}