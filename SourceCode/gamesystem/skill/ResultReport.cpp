#include "ResultReport.h"
#include "ImageManager.h"
#include <Input.h>
#include <Helper.h>
#include <Easing.h>

//èÛë‘ëJà⁄
void (ResultReport::* ResultReport::stateTable[])() = {
	&ResultReport::InitUpdate,//ìÆÇ´ÇÃçáä‘
	&ResultReport::ScoreUpdate,//ìÆÇ´ÇÃçáä‘
	&ResultReport::StampUpdate,//ìÆÇ´ÇÃçáä‘
	&ResultReport::FinishUpdate,//èuä‘à⁄ìÆ
};
ResultReport::ResultReport() {
	backScreen = IKESprite::Create(ImageManager::RESULTREPORTBACK, { 0.f,0.f }, { 1.f,1.f, 1.f, 1.f });
	rate = IKESprite::Create(ImageManager::RESULTREPORTATTACK, { 630.f,650.f }, { 1.f,1.f, 1.f, 1.f });
	skip = IKESprite::Create(ImageManager::RESULTSKIP, { 10.f,10.f }, { 1.f,1.f, 1.f, 1.f });
	for (auto i = 0; i < DAMAGEMAX; i++) {
		damage_dealt[i] = make_unique<DrawNumber>(2.f);
		damage_dealt[i]->Initialize();
		damage_taken[i] = make_unique<DrawNumber>(2.f);
		damage_taken[i]->Initialize();
	}
	feed = make_unique<Feed>();
	Initialize();
}

ResultReport::~ResultReport() {
}

void ResultReport::Initialize() {
	skip->SetSize({ 512.f * 0.4f,128.f * 0.4f });
	rate->SetAnchorPoint({ 0.5f,0.5f });
	rate->SetRotation(-2.f);
	for (auto i = 0; i < DAMAGEMAX; i++) {
		damage_dealt[i]->SetPosition({ 850.0f + i * 80,250.f });
		damage_taken[i]->SetPosition({ 850.0f + i * 80,430.f });
		damage_dealt[i]->SetNumber(0);
		damage_taken[i]->SetNumber(0);
		damage_dealt[i]->SetColor({ 1.f,0.5f,0.f,1.0f });
		damage_taken[i]->SetColor({ 1.f,0.5f,0.f,1.0f });
	}
	isFinish = false;
	m_Feed = false;
	count = 10;
	frameInit = 0.f;
	numFrames[0] = {};
	numFrames[1] = {};
	frameStamp = 0.f;
	skip_alpha = 0.f;
}

void ResultReport::Update() {
	(this->*stateTable[state])();
	skip_alpha += 0.05f;
	skip->SetColor({ 1.f,1.f,1.f,abs(sinf(skip_alpha)) });
	for (auto i = 0; i < DAMAGEMAX; i++) {
		damage_dealt[i]->Update();
		damage_taken[i]->Update();
	}
	if (Input::GetInstance()->TriggerButton(Input::A)){
		if (state == FINISH) { return; }
		DamageIntNum(dealtDamage, dealNum);
		DamageIntNum(takenDamage, takeNum);
		for (auto i = 0; i < DAMAGEMAX; i++) {
			damage_taken[i]->SetNumber(takeNum[i]);
			damage_dealt[i]->SetNumber(dealNum[i]);
		}		
		rate->SetSize({1024.f,128.f});
		state = FINISH;
	}
	SmokeUpdate();
}

void ResultReport::Draw(DirectXCommon* dxCommon) {
	if (isFinish) { return; }
	IKESprite::PreDraw();
	backScreen->Draw();
	if (state >= STAMP) {
		rate->Draw();
	}
	for (auto i = 0; i < DAMAGEMAX; i++) {
		if (state >= SCORE) {
			if (i != 2) {
				if (dealtDamage < 100) {
					if (damage_dealt[i]->GetNumber() == 0) {
						continue;
					}
				}
			}
		}
		damage_dealt[i]->Draw();
	}
	for (auto i = 0; i < DAMAGEMAX; i++) {
		if (state >= SCORE) {
			if (i != 2) {
				if (takenDamage < 100) {
					if (damage_taken[i]->GetNumber() == 0) {
						continue;
					}
				}
			}
		}
		damage_taken[i]->Draw();
	}
	for (SmokeEffect& smoke : smokes) {
		smoke.tex->Draw();
	}
	for (ShineEffect& shine : shines) {
		shine.tex->Draw();
	}
	if (state < FINISH) {
		skip->Draw();
	}
	if (m_Feed) {
		feed->Draw();
	}
	IKESprite::PostDraw();
}

void ResultReport::InitUpdate() {
	if (Helper::FrameCheck(frameInit, 1 / kFrameInitMax)) {
		for (auto i = 0; i < DAMAGEMAX; i++) {
			int r_num = Helper::GetRanNum(0, 9);
			int d_num = r_num;
			int t_num = r_num + (i - 2);
			Helper::Clamp(t_num, 0, 9);
			damage_dealt[i]->SetNumber(d_num);
			damage_taken[i]->SetNumber(t_num);
		}
		frameInit = 0.f;
		count--;
	}
	if (count <= 0) {
		count = 0;
		state = SCORE;
		DamageIntNum(dealtDamage, dealNum);
		DamageIntNum(takenDamage, takeNum);
	}

}
//Åö
void ResultReport::ScoreUpdate() {
	if (Helper::FrameCheck(numFrames[0], 1 / kFrameScoreMax)) {
		if (!isSecondNum) {
			SmokeInit({ 850.0f + 80.f,430.f });
		}
		isSecondNum = true;
		if (Helper::FrameCheck(numFrames[1], 1 / kFrameScoreMax)) {
			if (score >= 200) {
				rate = IKESprite::Create(ImageManager::RESULTREPORTATTACK, { 630.f,650.f }, { 1.f,1.f, 1.f, 1.f });
				rate->SetAnchorPoint({ 0.5f,0.5f });
				rate->SetRotation(-2.f);
			} else {
				rate = IKESprite::Create(ImageManager::RESULTREPORTDEFFENCE, { 630.f,650.f }, { 1.f,1.f, 1.f, 1.f });
				rate->SetAnchorPoint({ 0.5f,0.5f });
				rate->SetRotation(-2.f);

			}
			state = STAMP;
		} else {
			float size = Ease(Out, Elastic, numFrames[1], 48.f, 128.f);
			for (auto i = 0; i < DAMAGEMAX; i++) {
				damage_taken[i]->SetSize({ size,size });
				damage_taken[i]->SetNumber(takeNum[i]);
			}
		}
	} else {
		if (Helper::FrameCheck(frameInit, 1 / kFrameInitMax)) {
			if (!isFirstNum) {
				SmokeInit({ 850.0f + 80.f,250.f });
			}
			isFirstNum = true;
			for (auto i = 0; i < DAMAGEMAX; i++) {
				int r_num = Helper::GetRanNum(1, 9);
				damage_taken[i]->SetNumber(r_num);
			}
			frameInit = 0;
		}
		float size = Ease(Out, Elastic, numFrames[0], 48.f, 128.f);
		for (auto i = 0; i < DAMAGEMAX; i++) {
			damage_dealt[i]->SetSize({ size,size });
			damage_dealt[i]->SetNumber(dealNum[i]);
		}
	}
}

void ResultReport::StampUpdate() {
	if (!Helper::FrameCheck(frameStamp, 1 / kFrameStampMax)) {
		XMFLOAT2 size = {};
		size.x = Ease(Out, Elastic, frameStamp, 1024.f * 1.5f, 1024.f);
		size.y = Ease(Out, Elastic, frameStamp, 128.f * 1.5f, 128.f);
		rate->SetSize(size);

	} else {
		state = FINISH;
	}
}

void ResultReport::FinishUpdate() {

	if ((Input::GetInstance()->TriggerButton(Input::B) ||
		Input::GetInstance()->TriggerKey(DIK_SPACE))&&
		!m_Feed) {
		m_Feed = true;
	}

	if (m_Feed) {
		feed->FeedIn(Feed::FeedType::WHITE, 1.0f / 30.0f, m_Feed);
	}
	if (feed->GetFeedEnd()) {
		isFinish = true;
	}
}

void ResultReport::SmokeInit(XMFLOAT2 pos) {
	for (int i = 0; i < 4;i++) {
		int r_Rot = Helper::GetRanNum(0,360);
		SmokeEffect itr;
		itr.tex = IKESprite::Create(ImageManager::SMOKE, {});
		itr.tex->SetAnchorPoint({ 0.5f,0.5f });
		itr.tex->SetRotation((float)r_Rot);
		itr.size = {64.f,64.f};
		itr.pos = { pos.x + margin[i].x,pos.y + margin[i].y };
		itr.tex->SetSize(itr.size);
		itr.kFrameMax = 30.f;
		smokes.push_back(std::move(itr));
	}
	for (int i = 0; i < 4; i++) {
		ShineEffect shine;
		shine.tex = IKESprite::Create(ImageManager::SHINE_S, { -100.f,0.f });
		shine.tex->SetAnchorPoint({ 0.5f,0.5f });
		shine.tex->SetSize({ 64.f,64.f });
		shine.angle = ((i + 1) * 90.f) * (XM_PI / 180.f);
		shine.position = pos;
		shines.push_back(std::move(shine));
	}
}

void ResultReport::SmokeUpdate() {
	for (SmokeEffect& smoke : smokes) {
			if (Helper::FrameCheck(smoke.frame, 1 / smoke.kFrameMax)) {
				smoke.isVanish = true;
			} else {
				smoke.size.x = Ease(Out, Quad, smoke.frame, 64.f, 86.f);
				smoke.size.y = Ease(Out, Quad, smoke.frame, 64.f, 86.f);
				smoke.tex->SetSize(smoke.size);
				XMFLOAT2 pos = {};
				pos.x = Ease(In, Back, smoke.frame, smoke.pos.x, smoke.pos.x);
				pos.y = Ease(In, Back, smoke.frame, smoke.pos.y, smoke.pos.y+180.f);
				smoke.tex->SetPosition(pos);
			}
		}
	smokes.remove_if([](SmokeEffect& shine) {
			return shine.isVanish; });
	for (ShineEffect& shine : shines) {
		if (Helper::FrameCheck(shine.frame, 1 / shine.kFrame)) {
			shine.isVanish = true;
		} else {
			shine.dia = Ease(Out, Exp, shine.frame, 0.f, 100.f);
			shine.tex->SetPosition({
				shine.position.x + sinf(shine.angle) * shine.dia,
				shine.position.y - cosf(shine.angle) * shine.dia
				});
			float rot = Ease(In, Quad, shine.frame, 0.0f, 180.f);
			shine.tex->SetRotation(rot);
			float alpha = Ease(In, Quad, shine.frame, 1.0f, 0.f);
			shine.tex->SetColor({ 1,1,1,alpha });
		}
	}
	shines.remove_if([](ShineEffect& shine) {
		return shine.isVanish; });

}

void ResultReport::DamageIntNum(int num, vector<int>& nums) {
	int number = num;
	nums.resize(3);
	if (number >= 100) {
		int h_num = number / 100;
		nums[0] = h_num;
		number -= h_num * 100;
	}
	if (number >= 10) {
		int t_num = number / 10;
		nums[1] = t_num;
		number -= t_num * 10;
	} 
	if (number >= 0) {
		nums[2] = number;
	}
}
