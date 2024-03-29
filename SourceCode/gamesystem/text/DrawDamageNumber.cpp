#include "DrawDamageNumber.h"
#include <ImageManager.h>
#include <Helper.h>
#include <Easing.h>

DrawDamageNumber::DrawDamageNumber(const float scale, const bool move) {
	const int NumberCount = NUMBER_MAX;
	const float l_Width_Cut = 64.0f;
	const float l_Height_Cut = 64.0f;

	for (auto i = 0; i < NUMBER_MAX; i++) {
		//読み込み
		_Number[i] = IKESprite::Create(ImageManager::DAMAGENUMBER, { 0.0f,0.0f });
		int number_index_y = i / NumberCount;
		int number_index_x = i % NumberCount;
		_Number[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		_Number[i]->SetAnchorPoint({ 0.5f,0.5f });
		_Number[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		_Number[i]->SetScale(scale);
		_Number[i]->SetPosition({ -1000.0f,0.0f });
	}

	m_Move = move;
}

//初期化
void DrawDamageNumber::Initialize() {
	m_AfterPos = { m_Position.x,m_Position.y - 60.0f };
	m_Alive = true;
}
//更新
void DrawDamageNumber::Update() {
	for (int i = 0; i < _Number.size(); i++) {
		_Number[i]->SetPosition(m_Position);
		if (m_Move) {
			_Number[i]->SetSize(m_Size);
		}
	}

	if (m_Move) {
		NumberMove();
	}
}
//描画
void DrawDamageNumber::Draw() {
	_Number[m_Number]->Draw();
}
//ImGui描画
void DrawDamageNumber::ImGuiDraw() {
	ImGui::Begin("Damage");
	ImGui::Text("SizeX:%f", m_Size.x);
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::End();
}
//数字とか座標の設定
void DrawDamageNumber::SetExplain(const XMFLOAT3& pos) {
	//ワールド座標に変換する
	XMVECTOR texHPFirst;
	texHPFirst = { pos.x, pos.y, pos.z };
	texHPFirst = Helper::PosDivi(texHPFirst, m_MatView, false);
	texHPFirst = Helper::PosDivi(texHPFirst, m_MatProjection, true);
	texHPFirst = Helper::WDivision(texHPFirst, false);
	texHPFirst = Helper::PosDivi(texHPFirst, m_MatPort, false);

	m_Position = { texHPFirst.m128_f32[0],texHPFirst.m128_f32[1] };
}
//カメラ情報
void DrawDamageNumber::GetCameraData() {
	Camera* camera = Helper::GetCamera();
	m_MatView = camera->GetViewMatrix();
	m_MatProjection = camera->GetProjectionMatrix();
	m_MatPort = camera->GetViewPort();
}
//動き
void DrawDamageNumber::NumberMove() {
	static float addFrame = 1.f / 50.f;
	if (Helper::FrameCheck(m_Frame, addFrame)) {
		if (Helper::FrameCheck(m_FrameS, 1/10.f)) {
			m_Alive = false;
		} else {
			float alpha = Ease(In, Quad, m_FrameS, 1.0f,0.f);
			for (int i = 0; i < _Number.size(); i++) {
				_Number[i]->SetColor({1,1,1,alpha });
			}
		}
	} else {
		m_Position = { m_Position.x,
		Ease(Out,Back,m_Frame,m_Position.y,m_AfterPos.y) };
	}

}