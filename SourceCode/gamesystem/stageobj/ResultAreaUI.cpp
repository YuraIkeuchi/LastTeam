#include "ResultAreaUI.h"
#include "CsvLoader.h"
#include "ImageManager.h"
#include <Helper.h>

//リソース読み込み
ResultAreaUI::ResultAreaUI() {
	panels.sprite = IKESprite::Create(ImageManager::RESULTATTACKAREA, { 0,0 });
	panels.sprite->SetAnchorPoint({ 0.5f,0.5f });
	panels.sprite->SetSize({ 64.0f,64.0f });

	//プレイヤー
	player_panel = IKESprite::Create(ImageManager::MAP_CHARA, { 0,0 });
	player_panel->SetAnchorPoint({ 0.5f,0.5f });
	player_panel->SetSize({ 64.0f,64.0f });
	for (int i = 0; i < 3;i++) {
		_DelayTimer[i] = make_unique<DrawNumber>(0.5f);
		_DelayTimer[i]->Initialize();
	}
	_DelayTimer[0]->SetPosition({ 180.0f,360.0f + 135.0f });
	_DelayTimer[1]->SetPosition({ 155.0f,360.0f + 135.0f });
	_DelayTimer[2]->SetPosition({ 130.0f,360.0f + 135.0f });

}
//初期化
bool ResultAreaUI::Initialize() {
	const XMFLOAT2 l_BasePos = { 220.0f,360.0f + 135.0f };
	panels.position = { l_BasePos.x + (60.0f * (m_Width + m_DistanceX)),l_BasePos.y + (60.0f * (m_Height - m_DistanceY)) };
	panels.color = { 1.0f,1.0f,1.0f,1.0f };
	panels.sprite->SetPosition(panels.position);
	player_panel->SetPosition(l_BasePos);
	//CSV読み込み
	return true;
}

//更新処理
void ResultAreaUI::Update() {
	for (auto i = 0; i < _DelayTimer.size(); i++) {
		_DelayTimer[i]->SetNumber(_Delays[i]);
		_DelayTimer[i]->Update();
		if (m_Delay <= m_Frame) {
			_DelayTimer[i]->SetColor({1.f,0.8f,0.f,1.f});
		} else {
			_DelayTimer[i]->SetColor({ 1.f,1.f,1.f,1.f });
		}
		_Delays[i] = Helper::getDigits(m_Frame, i, i);
	}
	if (!isJump) {
		m_Frame++;
		if (m_Delay <= m_Frame) {
			prePos = player_panel->GetPosition();
			XMFLOAT2 panel = player_panel->GetPosition();
			panel.y -= 10.f;
			player_panel->SetPosition(panel);
			isJump = true;
		}
	} else {
		float y_temp = player_panel->GetPosition().y;
		XMFLOAT2 panel= player_panel->GetPosition();
		panel.y += (panel.y - prePos.y) + 1;
		prePos.y = y_temp;
		if (495.0f <= panel.y) {
			if (Helper::FrameCheck(finishDelay,1.f/45.f)) {
				isJump = false;
				m_Frame = 0;
				finishDelay = 0.f;
			}
			panel.y = 495.0f;
		}
		player_panel->SetPosition(panel);
	}
}

//描画
void ResultAreaUI::Draw() {
	IKESprite::PreDraw();
	panels.sprite->Draw();
	player_panel->Draw();
	if (m_Frame >= 100) {
		_DelayTimer[2]->Draw();
	}
	if (m_Frame>=10) {
		_DelayTimer[1]->Draw();
	}
	_DelayTimer[0]->Draw();
	IKESprite::PostDraw();
}

//ImGui
void ResultAreaUI::ImGuiDraw() {
	
}

void ResultAreaUI::ResetTimer() {
	m_Frame = 0;
	isJump=false;
	finishDelay = 0.f;
	XMFLOAT2 panel = player_panel->GetPosition();
	panel.y = 495.0f;
	player_panel->SetPosition(panel);

	for (auto i = 0; i < _DelayTimer.size(); i++) {
		_DelayTimer[i]->SetNumber(_Delays[i]);
		_DelayTimer[i]->Update();
		if (m_Delay <= m_Frame) {
			_DelayTimer[i]->SetColor({ 1.f,0.8f,0.f,1.f });
		} else {
			_DelayTimer[i]->SetColor({ 1.f,1.f,1.f,1.f });
		}
		_Delays[i] = Helper::getDigits(m_Frame, i, i);
	}
}
