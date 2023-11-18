#include "PredictArea.h"
#include "CsvLoader.h"
#include <ImageManager.h>
#include <Helper.h>
#include "StagePanel.h"
//リソース読み込み
PredictArea::PredictArea(const string& name) {
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			if (name == "ENEMY") {
				panels[i][j].tex.reset(new IKETexture(ImageManager::AREA, {}, { 1.f,1.f,1.f }, { 1.f,0.4f,0.4f,1.f }));
				panels[i][j].color = { 1.f,0.4f,0.4f,1.f };
			}
			else if(name == "PLAYER") {
				panels[i][j].tex.reset(new IKETexture(ImageManager::PLAYERPREDICT, {}, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
			}
			panels[i][j].tex->TextureCreate();
			panels[i][j].tex->Initialize();
			float baseScale = PANEL_SIZE * 0.1f;
			panels[i][j].tex->SetScale({ baseScale,baseScale,baseScale });
			panels[i][j].tex->SetRotation({ 90.0f,0.0f,0.0f });
		}
	}

	this->name = name;
}
//初期化
bool PredictArea::Initialize() {
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			panels[i][j].position = { (PANEL_SIZE * i) - (PREDICT_HEIGHT * PANEL_SIZE),0.02f,(PANEL_SIZE * j) };
		}
	}
	//CSV読み込み
	return true;
}

//更新処理
void PredictArea::Update() {
	//プレイヤーが居るマスが黄色くなる
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			panels[i][j].tex->Update();
			panels[i][j].tex->SetPosition(panels[i][j].position);
			panels[i][j].tex->SetColor(panels[i][j].color);
		}
	}

	//敵の予測エリアのみ処理
	if (name == "ENEMY") {
		FlashArea();
	}
}

//描画
void PredictArea::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			if (panels[i][j].predict) {
				panels[i][j].tex->Draw();
			}
		}
	}
	IKETexture::PostDraw();
}

//ImGui
void PredictArea::ImGuiDraw() {
	ImGui::Begin("Pre");
	ImGui::Text("Start:%d", m_FlashStart);
	ImGui::Text("Angle:%f", m_SinAngle);
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			if (panels[i][j].predict) {
				ImGui::Text("Alpha[%d][%d]:%f", i, j, panels[i][j].color.w);
			}
		}
	}
	ImGui::End();
}
//リセット
void PredictArea::ResetPredict() {
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			panels[i][j].predict = false;
		}
	}
	m_FlashStart = false;
}
//予測エリアのフラッシュ
void PredictArea::FlashArea() {
	if (m_FlashStart) {
		m_AddAngle = Helper::Lerp(10.0f, 20.0f,m_Timer, m_TargetTimer);		//線形補間でチャージを表してる
		//sin波によって上下に動く
		m_SinAngle += m_AddAngle;
		m_SinAngle2 = m_SinAngle * (3.14f / 180.0f);
		for (int i = 0; i < PREDICT_WIDTH; i++) {
			for (int j = 0; j < PREDICT_HEIGHT; j++) {
				panels[i][j].color.w = (sin(m_SinAngle2) * 0.5f + 0.5f);
			}
		}
	}
	else {
		m_SinAngle = 0.0f;
	}
}