#include "ResultSkill.h"
#include "Helper.h"
#include "ImageManager.h"
#include <Input.h>
#include <Easing.h>
#include <SkillManager.h>
#include <TutorialTask.h>
#include <windows.h>
#include <PassiveManager.h>
ResultSkill::ResultSkill() {
}

ResultSkill::~ResultSkill() {
}

void ResultSkill::Initialize(DirectXCommon* dxCommon) {
	backScreen = IKESprite::Create(ImageManager::RESULTBACKSCREEN, { 0.f,0.f }, { 1.f,1.f, 1.f, 1.0f });
	backScreen->SetSize({ 1280.f,720.f });
	selectFrame = IKESprite::Create(ImageManager::PASSIVE_FRAME, { 200.f,200.f });
	selectFrame->SetAnchorPoint({ 0.5f,0.5f });
	selectFrame->SetPosition(BasePos[2]);
	skillCheack = IKESprite::Create(ImageManager::RESULTNOWCHECK, { 1280.f,720.f });
	skillCheack->SetAnchorPoint({ 1.f,1.f });
	top_title = IKESprite::Create(ImageManager::PICKSKILLTOP, { 640.f,70.f }, { 1.f,1.f, 1.f, 1.f });
	top_title->SetSize({1280.f* 0.7f,128.f*0.7f});
	top_title->SetAnchorPoint({ 0.5f,0.5f });
	feedIn = IKESprite::Create(ImageManager::FEED, { 0.f,0.f }, { 1.f,1.f, 1.f, 1.0f });
	feedIn->SetSize({ 1280.f,720.f });
	expra = IKESprite::Create(ImageManager::EXPRE,{1280.f-210.f,10.f});
	
	StarInit();
	dxcommon = dxCommon;
}

void ResultSkill::Update() {
	if (!FeedOut()) { return; }
	ShineEffectUpdate();
	StarEffectUpdate();
	for (ResultUI& itr : choiceSkills) {
		if (!itr.isSkill) { continue; }
		if (itr.no == 2) {
			for (std::unique_ptr<ResultAreaUI>& pickAreas : itr.resultarea) {
				pickAreas->Update();
			}
		}
	}

}

void ResultSkill::Draw(DirectXCommon* dxCommon) {
	if (!isStart) { return; }

	IKESprite::PreDraw();
	backScreen->Draw();
	top_title->Draw();
	skillCheack->Draw();
	if (TutorialTask::GetInstance()->GetViewSkill()) {
		selectFrame->Draw();
		for (ResultUI& resultUI : choiceSkills) {
			resultUI.icon->Draw();
		}
		for (ResultUI& itr : choiceSkills) {
			if (itr.no == 2) {
				//itr.text_->TestDraw(dxCommon);
				if (!itr.isSkill) { 
					PassiveManager::GetInstance()->TextDraw(itr.ID);
					continue; }
				SkillManager::GetInstance()->TextDraw(itr.ID);
				for (std::unique_ptr<ResultAreaUI>& pickAreas : itr.resultarea) {
					pickAreas->Draw();
				}
			}
		}
	}
	IKESprite::PreDraw();
	expra->Draw();
	for (ShineEffect& shine : shines) {
		shine.tex->Draw();
	}
	for (StarEffect& star : stars) {
		star.tex->Draw();
	}
	feedIn->Draw();
	IKESprite::PostDraw();
}
void ResultSkill::ImGuiDraw() {
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
	// シャッフル
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(noDeck.begin(), noDeck.end(), engine);
	std::shuffle(noPassive.begin(), noPassive.end(), engine);

	if (isBattle) {
		//バトルステージ時スキルたくさん取ってたら
		if (noDeck.size() < 4) {
			for (int i = 0; i < noDeck.size(); i++) {
				ResultUI resultUI = CreateUI(true, noDeck[i], BasePos[nowPos]);
				choiceSkills.push_back(std::move(resultUI));
			}
			int balance = 5 - (int)noDeck.size();
			for (int i = 0; i < balance; i++) {
				ResultUI passiveUI = CreateUI(false, noPassive[i], BasePos[nowPos]);
				choiceSkills.push_back(std::move(passiveUI));
			}
		} else {
			for (int i = 0; i < 4; i++) {
				ResultUI resultUI = CreateUI(true, noDeck[i], BasePos[nowPos]);
				choiceSkills.push_back(std::move(resultUI));
			}
			//パッシブ
			ResultUI passiveUI = CreateUI(false, noPassive[0], BasePos[nowPos]);
			choiceSkills.push_back(std::move(passiveUI));
		}
	} else {
		//パッシブステージ時パッシブたくさん取ってたら
		if (noPassive.size() < 4) {
			for (int i = 0; i < noPassive.size(); i++) {
				ResultUI passiveUI = CreateUI(false, noPassive[i], BasePos[nowPos]);
				choiceSkills.push_back(std::move(passiveUI));
			}
			int balance = 5 - (int)noPassive.size();
			for (int i = 0; i < balance; i++) {
				//スキル
				ResultUI resultUI = CreateUI(true, noDeck[i], BasePos[nowPos]);
				choiceSkills.push_back(std::move(resultUI));
			}

		} else {
			for (int i = 0; i < 4; i++) {
				ResultUI passiveUI = CreateUI(false, noPassive[i], BasePos[nowPos]);
				choiceSkills.push_back(std::move(passiveUI));
			}
			//スキル
			ResultUI resultUI = CreateUI(true, noDeck[0], BasePos[nowPos]);
			choiceSkills.push_back(std::move(resultUI));
		}
	}

	for (int i = 0; i < 5; i++) {
		RandShineInit();
	}

	isStart = true;
}

void ResultSkill::Move() {
	if (!m_FeedEnd) { return; }
	if (m_Choice) { return; }
	if (!TutorialTask::GetInstance()->GetViewSkill()) { return; }
	Input* input = Input::GetInstance();
	if (isMove) {
		static float frame = 0.f;
		static float addFrame = 1.f / 15.f;

		if (Helper::FrameCheck(frame, addFrame)) {
			isMove = false;
			frame = 0.f;
			for (ResultUI& itr : choiceSkills) {
				itr.oldNo = itr.no;
			}
			StarInit();
		} else {
			for (ResultUI& itr : choiceSkills) {
				if ((itr.no == 2 && itr.oldNo == 1) || (itr.no == 2 && itr.oldNo == 3)) {
					itr.size.x = Ease(In, Quad, 0.5f, itr.size.x, 128.f);
					itr.size.y = Ease(In, Quad, 0.5f, itr.size.y, 128.f);
				}
				if ((itr.no == 3 && itr.oldNo == 2) || (itr.no == 1 && itr.oldNo == 2)) {
					itr.size.x = Ease(In, Quad, 0.5f, itr.size.x, 128.f * 0.5f);
					itr.size.y = Ease(In, Quad, 0.5f, itr.size.y, 128.f * 0.5f);
				}

				itr.position.x = Ease(Out, Quad, frame, BasePos[itr.oldNo].x, BasePos[itr.no].x);
				itr.icon->SetPosition(itr.position);
				itr.icon->SetSize(itr.size);
			}
		}
	}

	if (input->TiltPushStick(input->L_LEFT) ||
		input->TiltPushStick(input->L_RIGHT) ||
		input->TriggerKey(DIK_A) ||
		input->TriggerKey(DIK_D) ||
		input->PushButton(input->LEFT) ||
		input->PushButton(input->RIGHT)) {
		if (isMove) { return; }
		if (input->TiltPushStick(input->L_RIGHT) ||
			input->TriggerKey(DIK_D) ||
			input->PushButton(input->RIGHT)) {
			for (ResultUI& itr : choiceSkills) {
				itr.no++;
				if (itr.no == 5) {
					itr.no = 0;
				}
			}
		} else {
			for (ResultUI& itr : choiceSkills) {
				itr.no--;
				if (itr.no == -1) {
					itr.no = 4;
				}
			}
		}
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Cursor.wav", 0.1f);
		for (ResultUI& itr : choiceSkills) {
			for (std::unique_ptr<ResultAreaUI>& pickAreas : itr.resultarea) {
				pickAreas->ResetTimer();
			}
		}
		isMove = true;
	}
	if ((input->TriggerButton(Input::B) ||
		input->TriggerKey(DIK_SPACE)) &&
		!isMove) {
		for (ResultUI& itr : choiceSkills) {
			if (itr.no == 2) {
				ResultUI n = CreateUI(itr.isSkill, itr.ID, itr.position);
				pickSkills.push_back(std::move(n));
			}
		}
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button.wav", 0.15f);
		m_Choice = true;
	}

}

bool ResultSkill::FeedOut() {
	if (Helper::FrameCheck(frameA, 1 / 45.f)) {
		feedIn->SetColor({ 1,1,1,0.f });
		m_FeedEnd = true;
		return true;
	} else {
		float alpha = Ease(In, Quad, frameA, 1.f, 0.f);
		feedIn->SetColor({ 1,1,1,alpha });
		return false;
	}
}

void ResultSkill::RandShineInit() {
	float posX = (float)Helper::GetRanNum(128, 1150);
	float posY = (float)Helper::GetRanNum(128, 360);
	float frame = (float)Helper::GetRanNum(10, 20);
	float rot = (float)Helper::GetRanNum(0, 5);
	ShineEffect itr;
	itr.tex = IKESprite::Create(ImageManager::SHINE, { posX,posY });
	itr.tex->SetAnchorPoint({ 0.5f,0.5f });
	itr.tex->SetSize(itr.size);
	//itr.tex->SetRotation(rot);
	itr.kFrame = frame;
	shines.push_back(std::move(itr));
}

void ResultSkill::ShineEffectUpdate() {
	for (ShineEffect& shine : shines) {
		if (Helper::FrameCheck(shine.frame, 1 / shine.kFrame)) {
			if (Helper::FrameCheck(shine.frameA, 1.f / 10.f)) {
				RandShineInit();
				shine.isVanish = true;
			} else {
				shine.size.x = Ease(In, Circ, shine.frameA, 32.f, 0.f);
				shine.size.y = Ease(In, Circ, shine.frameA, 32.f, 0.f);
				shine.tex->SetSize(shine.size);
			}
		} else {
			shine.size.x = Ease(Out, Back, shine.frame, 0.f, 32.f);
			shine.size.y = Ease(Out, Back, shine.frame, 0.f, 32.f);
			shine.tex->SetSize(shine.size);
		}
	}
	shines.remove_if([](ShineEffect& shine) {
		return shine.isVanish; });
}

void ResultSkill::StarEffectUpdate() {
	for (StarEffect& star : stars) {
		if (Helper::FrameCheck(star.frame, 1 / star.kFrame)) {
			if (Helper::FrameCheck(star.frameA, 1.f / 10.f)) {
				star.isVanish = true;
			} else {
				float alpha = Ease(In, Quad, star.frameA, 1.0f, 0.f);
				float rot = Ease(In, Quad, star.frameA, 0.0f, 90.f);
				star.tex->SetColor({ 1,1,1,alpha });
				star.tex->SetRotation(rot);

			}
		} else {
			star.dia = Ease(Out, Exp, star.frame, 0.f, 150.f);
			star.tex->SetPosition({
				star.position.x + sinf(star.angle) * star.dia,
				star.position.y - cosf(star.angle) * star.dia
				});
			float size = Ease(Out, Back, star.frame, 0.f, 128.f);
			star.tex->SetSize({ size ,size });
		}
	}

	auto result = std::remove_if(stars.begin(), stars.end(),
		[](StarEffect& star) { return star.isVanish; });
	stars.erase(result, stars.end());
}

void ResultSkill::StarInit() {
	for (int i = 0; i < 5; i++) {
		StarEffect star;
		star.tex = IKESprite::Create(ImageManager::PLASHINE, { -100.f,0.f });
		star.tex->SetAnchorPoint({ 0.5f,0.5f });
		star.angle = (i * 72.f) * (XM_PI / 180.f);
		star.position = BasePos[2];
		stars.push_back(std::move(star));
	}
}

ResultSkill::ResultUI ResultSkill::CreateUI(bool isSkill, int id, XMFLOAT2 pos) {
	ResultUI resultUI;
	resultUI.ID = id;
	resultUI.position = pos;
	resultUI.isSkill = isSkill;
	if (resultUI.isSkill) {
		resultUI.icon = IKESprite::Create(ImageManager::ATTACK_0 + resultUI.ID, { 0.0f,0.0f });
		resultUI.icon->SetColor({ 1.3f,1.3f,1.3f,1.0f });
		SkillManager::GetInstance()->HandResultData(resultUI.ID, resultUI.area, resultUI.DisX, resultUI.DisY, resultUI.Damage);//IDに応じた攻撃エリア、距離、ダメージを取得する
		resultUI.Delay = SkillManager::GetInstance()->GetDelay(id);
		//桁数によって描画する桁数が違う
		BirthArea(resultUI);
		SkillManager::GetInstance()->LoadText(id);
	} else {
		PassiveManager::GetInstance()->LoadText(id);
		resultUI.icon = IKESprite::Create(ImageManager::PASSIVE_00 + resultUI.ID, { 0.0f,0.0f });
	}
	resultUI.icon->SetAnchorPoint({ 0.5f,0.5f });
	resultUI.icon->SetPosition(resultUI.position);
	resultUI.no = nowPos;
	if (resultUI.no == 2) {
		resultUI.size = { 128.f,128.f };
	} else {
		resultUI.size = { 128.f * 0.5f ,128.f * 0.5f };
	}
	resultUI.icon->SetSize(resultUI.size);
	resultUI.oldNo = resultUI.no;

	nowPos++;
	return resultUI;
}

void ResultSkill::BirthArea(ResultUI& resultUI) {

	for (auto i = 0; i < resultUI.area.size(); i++) {
		for (auto j = 0; j < resultUI.area.size(); j++) {
			if (resultUI.area[i][j] == 1) {		//マップチップ番号とタイルの最大数、最小数に応じて描画する
				std::unique_ptr<ResultAreaUI> newarea = std::make_unique<ResultAreaUI>();
				newarea->SetPanelNumber(i, j);
				newarea->SetDelay(resultUI.Delay);
				newarea->SetDistance(resultUI.DisX, resultUI.DisY);
				newarea->SetId(resultUI.ID);
				newarea->Initialize();
				resultUI.resultarea.push_back(std::move(newarea));
			}
		}
	}

}
