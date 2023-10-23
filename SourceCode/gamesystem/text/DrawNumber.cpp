#include "DrawNumber.h"
#include <ImageManager.h>
#include <Helper.h>

DrawNumber::DrawNumber() {
	const int NumberCount = NUMBER_MAX;
	const float l_Width_Cut = 64.0f;
	const float l_Height_Cut = 64.0f;


	for (int i = 0; i < _Number.size(); i++) {
		//読み込み
		_Number[i] = IKESprite::Create(ImageManager::HPNUMBERUI, { 0.0f,0.0f });
		int number_index_y = i / NumberCount;
		int number_index_x = i % NumberCount;
		_Number[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		_Number[i]->SetAnchorPoint({ 0.5f,0.5f });
		_Number[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		_Number[i]->SetScale(0.5f);
	}
}

//初期化
void DrawNumber::Initialize() {
	m_Number = {};
	m_NumberPos = {};
}
//更新
void DrawNumber::Update() {
	for (int i = 0; i < _Number.size(); i++) {
		_Number[i]->SetPosition(m_NumberPos);
	}
}
//描画
void DrawNumber::Draw() {
	_Number[m_Number]->Draw();
}
//数字とか座標の設定
void DrawNumber::SetExplain(const int Number,const XMFLOAT3& pos) {
	//ワールド座標に変換する
	XMVECTOR texHPFirst;
	texHPFirst = { pos.x, pos.y, pos.z };
	texHPFirst = Helper::GetInstance()->PosDivi(texHPFirst, m_MatView, false);
	texHPFirst = Helper::GetInstance()->PosDivi(texHPFirst, m_MatProjection, true);
	texHPFirst = Helper::GetInstance()->WDivision(texHPFirst, false);
	texHPFirst = Helper::GetInstance()->PosDivi(texHPFirst, m_MatPort, false);

	m_NumberPos = { texHPFirst.m128_f32[0],texHPFirst.m128_f32[1] };
	//描画する数字を指定
	m_Number = Number;
}
//カメラ情報
void DrawNumber::GetCameraData(const XMMATRIX& matView, const XMMATRIX& matProjection, const XMMATRIX& matPort) {
	m_MatView = matView;
	m_MatProjection = matProjection;
	m_MatPort = matPort;
}