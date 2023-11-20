#include "ResultSkill.h"
#include "Helper.h"
#include "ImageManager.h"
#include <Input.h>
#include <Easing.h>
#include <SkillManager.h>
ResultSkill::ResultSkill() {
}

ResultSkill::~ResultSkill() {
}

void ResultSkill::Initialize(DirectXCommon* dxCommon) {
	backScreen = IKESprite::Create(ImageManager::RESULTBACKSCREEN, { 0.f,0.f }, { 1.f,1.f, 1.f, 1.0f });
	backScreen->SetSize({ 1280.f,720.f });
	selectFrame = IKESprite::Create(ImageManager::PASSIVE_FRAME, { 200.f,200.f });
	selectFrame->SetAnchorPoint({ 0.5f,0.5f });
	selectFrame->SetPosition(framePos);
	//���U���g�e�L�X�g
	resulttext = make_unique<TextManager>();
	resulttext->Initialize(dxCommon);
	resulttext->SetConversation(TextManager::RESULT, { -235.0f,80.0f });

}

void ResultSkill::Update() {
	Move();
	for (ResultUI& resultUI : choiceSkills) {
		if (resultUI.isSkill) {
			resultUI.DamageNumber[0]->Update();
			if (resultUI.Damage >= 10) {
				resultUI.DamageNumber[1]->Update();
			}
		}
	}
	ShineEffectUpdate();
}

void ResultSkill::Draw(DirectXCommon* dxCommon) {
	if (!isStart) { return; }

	IKESprite::PreDraw();
	backScreen->Draw();
	selectFrame->Draw();
	for (ResultUI& resultUI : choiceSkills) {
		resultUI.icon->Draw();
		if (resultUI.isSkill) {
			if (resultUI.Damage != 0) {
				resultUI.DamageNumber[0]->Draw();
			}
			if (resultUI.Damage >= 10) {
				resultUI.DamageNumber[1]->Draw();
			}
		}
	}
	resulttext->TestDraw(dxCommon);
	for (ResultUI& itr : choiceSkills) {
		if (itr.no == nowFrame) {
			if (!itr.isSkill) { continue; }
			for (std::unique_ptr<ResultAreaUI>& pickAreas : itr.resultarea) {
				pickAreas->Draw();
			}
		}
	}
	IKESprite::PreDraw();

	for (ShineEffect& shine : shines) {
		shine.tex->Draw();
	}
	IKESprite::PostDraw();
}
void ResultSkill::ImGuiDraw() {
	ImGui::Begin("Result");
	ImGui::Text("Frame:%d", nowFrame);
	ImGui::Text("OldFrame:%d", oldFrame);
	ImGui::Text("PosX:%f", framePos.x);
	ImGui::End();
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
		if (resultUI.ID==1) {
			player_->SetMaxHp(
			player_->GetMaxHp() * 1.3f);
		}
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
	// シャッフル
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(noDeck.begin(), noDeck.end(), engine);
	std::shuffle(noPassive.begin(), noPassive.end(), engine);

	
	//スキル
	ResultUI resultUI = CreateUI(true, noDeck[0], BasePos[nowPos]);
	choiceSkills.push_back(std::move(resultUI));
	if (isBattle) {
		ResultUI resultUI = CreateUI(true, noDeck[1], BasePos[nowPos]);
		choiceSkills.push_back(std::move(resultUI));
	}
	//パッシブ
	ResultUI passiveUI = CreateUI(false, noPassive[0], BasePos[nowPos]);
	choiceSkills.push_back(std::move(passiveUI));
	if (!isBattle) {
		ResultUI passiveUI = CreateUI(false, noPassive[1], BasePos[nowPos]);
		choiceSkills.push_back(std::move(passiveUI));
	}
	/*if (noDeck.size() < noPassive.size()) {
		if (noPassive.size() > 1) {
			ResultUI passiveUI2 = CreateUI(false, noPassive[1], BasePos[nowPos]);
			choiceSkills.push_back(std::move(passiveUI2));
		}
	} else {
		if (noDeck.size() > 1) {
			ResultUI passiveUI3 = CreateUI(true, noDeck[1], BasePos[nowPos]);
			choiceSkills.push_back(std::move(passiveUI3));
		}
	}*/
	resulttext->SetCreateSentence(baseSentence[0], baseSentence[1], baseSentence[2]);
	for (int i = 0; i < 5;i++) {
		RandShineInit();
	}
	
	isStart = true;
}

void ResultSkill::Move() {
	if (m_Choice) { return; }
	Input* input = Input::GetInstance();
	if (isMove) {
		static float frame = 0.f;
		static float addFrame = 1.f / 15.f;

		if (Helper::FrameCheck(frame, addFrame)) {
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
		m_Choice = true;
	}

}

void ResultSkill::RandShineInit() {
	float posX = (float)Helper::GetRanNum(128, 1150);
	float posY = (float)Helper::GetRanNum(128, 360);
	float frame = (float)Helper::GetRanNum(30, 45);
	ShineEffect itr;
	itr.tex = IKESprite::Create(ImageManager::SHINE, { posX,posY });
	itr.tex->SetAnchorPoint({ 0.5f,0.5f });
	itr.tex->SetSize(itr.size);
	itr.kFrame = frame;
	shines.push_back(std::move(itr));
}

void ResultSkill::ShineEffectUpdate() {
	for (ShineEffect& shine : shines) {
		if (Helper::FrameCheck(shine.frame, 1 / shine.kFrame)) {
			RandShineInit();
			shine.isVanish = true;
		} else {
			shine.size.x = Ease(Out, Quad, shine.frame, 0.f, 32.f);
			shine.size.y = Ease(Out, Quad, shine.frame, 0.f, 32.f);
			shine.tex->SetSize(shine.size);
		}
	}
	shines.remove_if([](ShineEffect& shine) {
		return shine.isVanish; });
}

ResultSkill::ResultUI ResultSkill::CreateUI(bool isSkill, int id, XMFLOAT2 pos) {
	ResultUI resultUI;
	resultUI.ID = id;
	resultUI.position = pos;
	resultUI.isSkill = isSkill;
	if (resultUI.isSkill) {
		resultUI.icon = IKESprite::Create(ImageManager::ATTACK_0 + resultUI.ID, { 0.0f,0.0f });
		resultUI.icon->SetColor({ 1.3f,1.3f,1.3f,1.0f });
		SkillManager::GetInstance()->HandResultData(resultUI.ID, resultUI.area, resultUI.DisX, resultUI.DisY,resultUI.Damage);//IDに応じた攻撃エリア、距離、ダメージを取得する
			//桁数によって描画する桁数が違う
		if (resultUI.Damage < 10) {
			resultUI.DamageNumber[0] = make_unique<DrawNumber>(0.5f);
			resultUI.DamageNumber[0]->Initialize();
			resultUI.DamageNumber[0]->SetNumber(resultUI.Damage);
			resultUI.DamageNumber[0]->SetPosition({ resultUI.position.x + 10.0f,resultUI.position.y });
		}
		else {
			int l_DightDamage[S_DAMAGEMAX];
			for (auto i = 0; i < S_DAMAGEMAX; i++) {
				resultUI.DamageNumber[i] = make_unique<DrawNumber>(0.5f);
				resultUI.DamageNumber[i]->Initialize();
				l_DightDamage[i] = Helper::getDigits(resultUI.Damage, i, i);
				resultUI.DamageNumber[i]->SetNumber(l_DightDamage[i]);
			}

			resultUI.DamageNumber[0]->SetPosition({ resultUI.position.x + 10.0f,resultUI.position.y });
			resultUI.DamageNumber[1]->SetPosition({ resultUI.position.x - 10.0f,resultUI.position.y });
		}
		BirthArea(resultUI);
		baseSentence[nowPos] = resulttext->GetSkillSentence(resultUI.ID);
	} else {
		resultUI.icon = IKESprite::Create(ImageManager::PASSIVE_01 + resultUI.ID, { 0.0f,0.0f });
		baseSentence[nowPos] = resulttext->GetPasiveSentence(resultUI.ID);
	}
	resultUI.icon->SetAnchorPoint({ 0.5f,0.5f });
	resultUI.icon->SetSize({ 128.f,128.f });
	resultUI.icon->SetPosition(resultUI.position);
	resultUI.no = nowPos;
	nowPos++;
	return resultUI;
}

void ResultSkill::BirthArea(ResultUI& resultUI) {

	for (auto i = 0; i < resultUI.area.size(); i++) {
		for (auto j = 0; j < resultUI.area.size(); j++) {
			if (resultUI.area[i][j] == 1) {		//マップチップ番号とタイルの最大数、最小数に応じて描画する
				std::unique_ptr<ResultAreaUI> newarea = std::make_unique<ResultAreaUI>();
				newarea->SetPanelNumber(i, j);
				newarea->SetDistance(resultUI.DisX, resultUI.DisY);
				newarea->Initialize();
				resultUI.resultarea.push_back(std::move(newarea));
			}
		}
	}

}
