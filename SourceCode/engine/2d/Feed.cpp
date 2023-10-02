#include "Feed.h"
#include"Helper.h"
#include"ImageManager.h"
#define WindowW 1920
#define WindowH 1020
Feed::Feed() {
	//初期化
	FeedTex = IKESprite::Create(ImageManager::FEED, { 0,0 });
	FeedTex->SetAnchorPoint({ 0.f,0.f });
	FeedTex->SetSize({ WindowW,WindowH });

	Color_RGB = { 0,0,0 };
	m_Alpha = 0.f;
}

void Feed::FeedIn(FeedType type, float feedspeed, bool& feedf) {
	if (FeedTex == nullptr)return;
	//テク椅子茶色指定
	if (type == FeedType::BLACK)Color_RGB = { 0,0,0 };
	else Color_RGB = { 1,1,1 };

	//遷移
	constexpr int inStaymax = 60;
	switch (_phase) {
	case FeedPhase::NON:
		m_Alpha = 0.f;
		FeedInCounter = 0;
		if (feedf)_phase = FeedPhase::FEEDIN;
		break;

	case FeedPhase::FEEDIN:
		m_Alpha += feedspeed;
		if (m_Alpha >= 1.f)FeedInCounter++;
		//フェードイン語の待機時間
		if (FeedInCounter > inStaymax)_phase = FeedPhase::FEEDOUT;
		break;
	case FeedPhase::FEEDOUT:
		m_Alpha -= (feedspeed) * 2.0f;
		if (m_Alpha <= 0.f) {
			//各種初期化
			feedf = false;

			_phase = FeedPhase::END;
		}
		break;

	case FeedPhase::END:
		if (feedf)_phase = FeedPhase::NON;
		break;

	}

	FeedTex->SetColor({ Color_RGB.x,Color_RGB.y,Color_RGB.z,m_Alpha });
	//アルファ値の範囲指定
	Helper::GetInstance()->Clamp(m_Alpha, 0.f, 1.f);
}

void Feed::Draw() {
	if (FeedTex == nullptr)return;
	IKESprite::PreDraw();
	FeedTex->Draw();
	IKESprite::PostDraw();
}

bool Feed::GetFeedEnd(){ if (_phase == FeedPhase::FEEDOUT)return true; else return false; }

//
Feed2* Feed2::GetInstance()
{
	static Feed2 ins;

	return &ins;
}

Feed2::Feed2()
{
	//初期化
	FeedTex = IKESprite::Create(ImageManager::FEED, { 0,0 });
	FeedTex->SetAnchorPoint({ 0.f,0.f });
	FeedTex->SetSize({ WindowW,WindowH });

	Color_RGB = { 0,0,0 };
	m_Alpha = 0.f;
}

void Feed2::FeedIn2(FeedType2 type, float feedspeed, bool& feedf)
{
	if (FeedTex == nullptr)return;
	//テク椅子茶色指定
	if (type == FeedType2::BLACK)Color_RGB = { 0,0,0 };
	else Color_RGB = { 1,1,1 };

	//遷移
	constexpr int inStaymax = 60;
	switch (_phase)
	{
	case FeedPhase2::NON:
		m_Alpha = 0.f;
		FeedInCounter = 0;
		if (feedf)_phase = FeedPhase2::FEEDIN;
		break;

	case FeedPhase2::FEEDIN:
		m_Alpha += feedspeed;
		if (m_Alpha >= 1.f)FeedInCounter++;
		//フェードイン語の待機時間
		if (FeedInCounter > inStaymax)_phase = FeedPhase2::FEEDOUT;
		break;
	case FeedPhase2::FEEDOUT:
		m_Alpha -= (feedspeed);
		if (m_Alpha <= 0.f) {
			//各種初期化
			feedf = false;

			_phase = FeedPhase2::END;
		}
		break;

	case FeedPhase2::END:
		if (feedf)_phase = FeedPhase2::NON;
		break;

	}

	FeedTex->SetColor({ Color_RGB.x,Color_RGB.y,Color_RGB.z,m_Alpha });
	//アルファ値の範囲指定
	Helper::GetInstance()->Clamp(m_Alpha, 0.f, 1.f);
}

void Feed2::Draw2()
{
	if (FeedTex == nullptr)return;
	IKESprite::PreDraw();
	FeedTex->Draw();
	IKESprite::PostDraw();
}

bool Feed2::GetFeedEnd2() { if (_phase == FeedPhase2::FEEDOUT)return true; else return false; }
