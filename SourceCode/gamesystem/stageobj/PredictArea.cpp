#include "PredictArea.h"
#include "CsvLoader.h"
#include <ImageManager.h>
#include <Helper.h>
#include "StagePanel.h"
#include <Easing.h>
//リソース読み込み
PredictArea::PredictArea(const string& name) {
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			//普通の
			if (name == "ENEMY") {
				panels[i][j].tex[PREDICT_ATTACK].reset(new IKETexture(ImageManager::AREA, {}, {1.f,1.f,1.f}, {1.f,0.4f,0.4f,1.f}));
				panels[i][j].color = { 1.f,0.4f,0.4f,1.f };
				
			}
			else if(name == "PLAYER") {
				panels[i][j].tex[PREDICT_ATTACK].reset(new IKETexture(ImageManager::PLAYERPREDICT, {}, {1.f,1.f,1.f}, {1.f,1.f,1.f,1.f}));
			}
			panels[i][j].tex[PREDICT_ATTACK]->TextureCreate();
			panels[i][j].tex[PREDICT_ATTACK]->Initialize();
			float baseScale = PANEL_SIZE * 0.1f;
			panels[i][j].tex[PREDICT_ATTACK]->SetScale({ baseScale,baseScale,baseScale });
			panels[i][j].tex[PREDICT_ATTACK]->SetRotation({ 90.0f,0.0f,0.0f });

			//バフ
			panels[i][j].tex[PREDICT_BUFF].reset(new IKETexture(ImageManager::BUFFAREA, {}, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
			panels[i][j].tex[PREDICT_BUFF]->TextureCreate();
			panels[i][j].tex[PREDICT_BUFF]->Initialize();
			panels[i][j].tex[PREDICT_BUFF]->SetScale({ baseScale,baseScale,baseScale });
			panels[i][j].tex[PREDICT_BUFF]->SetRotation({ 90.0f,0.0f,0.0f });

			//はてな
			panels[i][j].tex[PREDICT_HATENA].reset(new IKETexture(ImageManager::HATENAAREA, {}, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
			panels[i][j].tex[PREDICT_HATENA]->TextureCreate();
			panels[i][j].tex[PREDICT_HATENA]->Initialize();
			panels[i][j].tex[PREDICT_HATENA]->SetScale({ baseScale,baseScale,baseScale });
			panels[i][j].tex[PREDICT_HATENA]->SetRotation({ 90.0f,0.0f,0.0f });

			//回復
			panels[i][j].tex[PREDICT_HEAL].reset(new IKETexture(ImageManager::HEALAREA, {}, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
			panels[i][j].tex[PREDICT_HEAL]->TextureCreate();
			panels[i][j].tex[PREDICT_HEAL]->Initialize();
			panels[i][j].tex[PREDICT_HEAL]->SetScale({ baseScale,baseScale,baseScale });
			panels[i][j].tex[PREDICT_HEAL]->SetRotation({ 90.0f,0.0f,0.0f });
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
			if (panels[i][j].predict) {
				panels[i][j].tex[m_DrawDype]->Update();
			}
			panels[i][j].tex[m_DrawDype]->SetPosition(panels[i][j].position);
			panels[i][j].tex[m_DrawDype]->SetColor(panels[i][j].color);
		}
	}

	//敵の予測エリアのみ処理
	if (name == "ENEMY") {
		FlashArea();
		for (int i = 0; i < PREDICT_WIDTH; i++) {
			for (int j = 0; j < PREDICT_HEIGHT; j++) {
				if (panels[i][j].isVerse) {
					if (Helper::FrameCheck(panels[i][j].frame, 1.f / 10.f)) {
						panels[i][j].frame = 0.f;
						panels[i][j].isVerse = false;
					} else {
						panels[i][j].position = {
							Ease(In,Quad,panels[i][j].frame,panels[i][j].beforePos.x,panels[i][j].afterPos.x),
							Ease(In,Quad,panels[i][j].frame,panels[i][j].beforePos.y,panels[i][j].afterPos.y),
							Ease(In,Quad,panels[i][j].frame,panels[i][j].beforePos.z,panels[i][j].afterPos.z),
						};
						panels[i][j].scale = {
							Ease(Out,Back,panels[i][j].frame,0.f,PANEL_SIZE) * 0.1f,
							Ease(Out,Back,panels[i][j].frame,0.f,PANEL_SIZE) * 0.1f,
							Ease(Out,Back,panels[i][j].frame,0.f,PANEL_SIZE) * 0.1f
						};
					}
					panels[i][j].tex[m_DrawDype]->SetScale(panels[i][j].scale);
					panels[i][j].tex[m_DrawDype]->SetPosition(panels[i][j].position);
					panels[i][j].tex[m_DrawDype]->Update();
				}
				if (panels[i][j].isVanish) {
					if (Helper::FrameCheck(panels[i][j].frame, 1.f / 10.f)) {
						panels[i][j].frame = 0.f;
						panels[i][j].isVanish = false;
						panels[i][j].predict = false;
					} else {
						panels[i][j].scale = {
							Ease(In,Circ,panels[i][j].frame,PANEL_SIZE,0.f) * 0.1f,
							Ease(In,Circ,panels[i][j].frame,PANEL_SIZE,0.f) * 0.1f,
							Ease(In,Circ,panels[i][j].frame,PANEL_SIZE,0.f) * 0.1f
						};
					}
					panels[i][j].tex[m_DrawDype]->SetScale(panels[i][j].scale);
					panels[i][j].tex[m_DrawDype]->SetPosition(panels[i][j].position);
					panels[i][j].tex[m_DrawDype]->Update();
				}
			}
		}
	}
}

//描画
void PredictArea::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			if (panels[i][j].predict) {
				panels[i][j].tex[m_DrawDype]->Draw();
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
		m_AddAngle = Helper::Lerp(10.0f, 30.0f,m_Timer, m_TargetTimer);		//線形補間でチャージを表してる
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

void PredictArea::VersePredict(int width, int height) {
	if (name != "ENEMY") { assert(0); }
	XMFLOAT3 pos_ = { (PANEL_SIZE * width) - (PREDICT_HEIGHT * PANEL_SIZE),0.02f,(PANEL_SIZE * height) };
	panels[width][height].beforePos = { pos_.x,pos_.y + 1.0f,pos_.z };
	panels[width][height].afterPos = pos_;
	panels[width][height].scale = {};
	panels[width][height].predict = true;
	panels[width][height].isVerse = true;
}

void PredictArea::VanishPredict(int width, int height) {

	panels[width][height].isVanish = true;

}