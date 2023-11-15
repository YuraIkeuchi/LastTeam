#include "PredictArea.h"
#include "CsvLoader.h"
#include "ImageManager.h"

//リソース読み込み
PredictArea::PredictArea() {
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			panels[i][j].tex.reset(new IKETexture(ImageManager::AREA,{}, {1.f,1.f,1.f}, {1.f,1.f,1.f,1.f}));
			panels[i][j].tex->TextureCreate();
			panels[i][j].tex->Initialize();
			panels[i][j].tex->SetScale({ 0.2f,0.2f,0.2f });
			panels[i][j].tex->SetRotation({ 90.0f,0.0f,0.0f });
		}
	}
}
//初期化
bool PredictArea::Initialize() {
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			panels[i][j].position = { (2.0f * i) - (PREDICT_HEIGHT * 2.0f),0.02f,(2.0f * j) };
			panels[i][j].color = { 1.f,1.f,1.f,1.f };
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
			panels[i][j].color = { 1.0f,0.3f,0.0f,0.5f };
			panels[i][j].tex->Update();
			panels[i][j].tex->SetPosition(panels[i][j].position);
			panels[i][j].tex->SetColor(panels[i][j].color);
		}
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
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			ImGui::Text("Flag[%d][%d]:%d", i, j, panels[i][j].predict);
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
}