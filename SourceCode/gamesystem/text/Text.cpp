#include "Text.h"
#include"VariableCommon.h"
#include<locale.h>
#include<wchar.h>

const XMFLOAT2 kFirstRowPos{ 5.f,0.f };
const XMFLOAT2 kSecondRowPos{ 5.f,-40.f };
const XMFLOAT2 kThirdRowPos{ 5.f, -80.f };

void Text::Initialize(DirectXCommon* dxcommon, const XMVECTOR& color)
{
	//生成
	firstrow_ = make_unique<Font>();
	secondrow_ = make_unique<Font>();
	thirdrow_ = make_unique<Font>();
	//読み込み
	firstrow_->LoadFont(dxcommon);
	secondrow_->LoadFont(dxcommon);
	thirdrow_->LoadFont(dxcommon);
	//座標セット
	firstrow_->SetPos(kFirstRowPos);
	secondrow_->SetPos(kSecondRowPos);
	thirdrow_->SetPos(kThirdRowPos);
	//色
	firstrow_->SetColor(color);
	secondrow_->SetColor(color);
	thirdrow_->SetColor(color);
}

void Text::Update(wchar_t* firstrow, wchar_t* secondrow, wchar_t* thirdrow)
{
	firstrow_->SetString(firstrow);
	secondrow_->SetString(secondrow);
	thirdrow_->SetString(thirdrow);
}

void Text::Draw(DirectXCommon* dxcommon)
{
	firstrow_->Draw(dxcommon);
	secondrow_->Draw(dxcommon);
	thirdrow_->Draw(dxcommon);
}
