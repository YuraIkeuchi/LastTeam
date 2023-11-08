#include "ResultSkill.h"
#include "Helper.h"
#include "ImageManager.h"
#include <Input.h>
#include <Easing.h>

ResultSkill::ResultSkill() {

}

ResultSkill::~ResultSkill() {
}

void ResultSkill::Initialize() {
	backScreen = IKESprite::Create(ImageManager::FEED, { 0.f,0.f }, { 0.f,0.f, 0.f, 0.5f });
	backScreen->SetSize({ 1280.f,720.f });
	selectFrame = IKESprite::Create(ImageManager::PASSIVE_FRAME, { 200.f,200.f });
	selectFrame->SetAnchorPoint({ 0.5f,0.5f });
	selectFrame->SetPosition(framePos);
}

void ResultSkill::Update() {
	Move();
	for (ResultUI& resultUI : choiceSkills) {
		if (resultUI.isSkill) {
			resultUI.number->Update();
		}
	}
}

void ResultSkill::Draw() {
	if (!isStart) { return; }

	IKESprite::PreDraw();
	backScreen->Draw();
	selectFrame->Draw();
	for (ResultUI& resultUI : choiceSkills) {
		resultUI.icon->Draw();
		if (resultUI.isSkill) {
			resultUI.number->Draw();
		}
	}
	IKESprite::PostDraw();
}

void ResultSkill::InDeck(std::vector<int>& Deck) {
	std::vector<int> itr = Deck;
	for (ResultUI& resultUI : pickSkills) {
		if (!resultUI.isSkill) { continue; }
		itr.push_back(resultUI.ID);
	}
	Deck.resize(itr.size());
	for (int i = 0; i < Deck.size(); i++) {
		Deck[i] = itr[i];
	}
}

void ResultSkill::InPassive(std::vector<int>& Passive) {
	std::vector<int> itr = Passive;
	for (ResultUI& resultUI : pickSkills) {
		if (resultUI.isSkill) { continue; }
		itr.push_back(resultUI.ID);
	}
	Passive.resize(itr.size());
	for (int i = 0; i < Passive.size(); i++) {
		Passive[i] = itr[i];
	}
}

void ResultSkill::CreateResult(std::vector<int>& notDeck, std::vector<int>& notPassives) {
	if (isStart) { return; }
	vector<int> noDeck = notDeck;
	vector<int> noPassive = notPassives;
	if (noDeck.size() == 0 || noPassive.size() == 0) {
		isStart = true;
		return;
	}
	// �V���b�t��
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(noDeck.begin(), noDeck.end(), engine);
	std::shuffle(noPassive.begin(), noPassive.end(), engine);

	//�X�L��
	ResultUI resultUI = CreateUI(true, noDeck[0], BasePos[nowPos]);
	choiceSkills.push_back(std::move(resultUI));
	//�p�b�V�u
	ResultUI passiveUI = CreateUI(false, noPassive[0], BasePos[nowPos]);
	choiceSkills.push_back(std::move(passiveUI));
	if (noDeck.size()< noPassive.size()) {
		if (noPassive.size() > 1) {
			ResultUI passiveUI2 = CreateUI(false, noPassive[1], BasePos[nowPos]);
			choiceSkills.push_back(std::move(passiveUI2));
		}
	} else {
		if (noDeck.size() > 1) {
			ResultUI passiveUI3 = CreateUI(true, noDeck[1], BasePos[nowPos]);
			choiceSkills.push_back(std::move(passiveUI3));
		}
	}
	isStart = true;
}

void ResultSkill::Move() {
	Input* input = Input::GetInstance();
	if (isMove) {
		static float frame = 0.f;
		static float addFrame = 1.f / 15.f;

		if (Helper::GetInstance()->FrameCheck(frame, addFrame)) {
			oldFrame = nowFrame;
			isMove = false;
			frame = 0.f;
		} else {
			framePos.x = Ease(InOut, Circ, frame, BasePos[oldFrame].x, BasePos[nowFrame].x);
			selectFrame->SetPosition(framePos);
		}
	}

	if (input->TiltPushStick(input->L_LEFT) ||
		input->TiltPushStick(input->L_RIGHT)) {
		if (isMove) { return; }
		if (input->TiltPushStick(input->L_RIGHT)) {
			if (nowFrame == 2) { return; }
			nowFrame++;
		} else {
			if (nowFrame == 0) { return; }
			nowFrame--;
		}
		isMove = true;
	}
	if (input->TriggerButton(Input::B)) {
		for (ResultUI& itr : choiceSkills) {
			if (itr.no == nowFrame) {
				ResultUI n = CreateUI(itr.isSkill, itr.ID, itr.position);
				pickSkills.push_back(std::move(n));
			}
		}
	}
}

ResultSkill::ResultUI ResultSkill::CreateUI(bool isSkill, int id, XMFLOAT2 pos) {
	ResultUI resultUI;
	resultUI.ID = id;
	resultUI.position = pos;
	resultUI.isSkill = isSkill;
	if (resultUI.isSkill) {
		resultUI.icon = IKESprite::Create(ImageManager::ACTIONUI, { 0.0f,0.0f });
		resultUI.number = make_unique<DrawNumber>();
		resultUI.number->Initialize();
		resultUI.number->SetNumber(resultUI.ID);
		resultUI.number->SetPosition(resultUI.position);
	} else {
		resultUI.icon = IKESprite::Create(ImageManager::PASSIVE_01 + resultUI.ID, { 0.0f,0.0f });
	}
	resultUI.icon->SetAnchorPoint({ 0.5f,0.5f });
	resultUI.icon->SetSize({ 128.f,128.f });
	resultUI.icon->SetPosition(resultUI.position);
	resultUI.no = nowPos;
	nowPos++;
	return resultUI;
}
