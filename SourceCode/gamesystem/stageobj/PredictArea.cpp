#include "PredictArea.h"
#include "CsvLoader.h"
#include "ModelManager.h"

//リソース読み込み
PredictArea::PredictArea() {
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			panels[i][j].object.reset(new IKEObject3d());
			panels[i][j].object->Initialize();
			panels[i][j].object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PANEL));
			panels[i][j].object->SetScale({ 2.f,0.1f,2.f });
		}
	}
}
//初期化
bool PredictArea::Initialize() {
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			panels[i][j].position = { (2.0f * i) - (PREDICT_HEIGHT * 2.0f),0.01f,(2.0f * j) };
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
			panels[i][j].object->Update();
			panels[i][j].object->SetPosition(panels[i][j].position);
			panels[i][j].object->SetColor(panels[i][j].color);
		}
	}
}

//描画
void PredictArea::Draw(DirectXCommon* dxCommon) {
	IKEObject3d::PreDraw();
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			if (panels[i][j].predict) {
				panels[i][j].object->Draw();
			}
		}
	}
	IKEObject3d::PostDraw();
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