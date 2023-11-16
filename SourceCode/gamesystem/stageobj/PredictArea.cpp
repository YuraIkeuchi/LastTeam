#include "PredictArea.h"
#include "CsvLoader.h"
#include <ImageManager.h>
#include <Helper.h>

//���\�[�X�ǂݍ���
PredictArea::PredictArea(const string& name) {
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			if (name == "ENEMY") {
				panels[i][j].tex.reset(new IKETexture(ImageManager::ENEMYPREDICT, {}, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
			}
			else if(name == "PLAYER") {
				panels[i][j].tex.reset(new IKETexture(ImageManager::PLAYERPREDICT, {}, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
			}
			panels[i][j].tex->TextureCreate();
			panels[i][j].tex->Initialize();
			panels[i][j].tex->SetScale({ 0.2f,0.2f,0.2f });
			panels[i][j].tex->SetRotation({ 90.0f,0.0f,0.0f });
		}
	}

	this->name = name;
}
//������
bool PredictArea::Initialize() {
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			panels[i][j].position = { (2.0f * i) - (PREDICT_HEIGHT * 2.0f),0.02f,(2.0f * j) };
		}
	}
	//CSV�ǂݍ���
	return true;
}

//�X�V����
void PredictArea::Update() {
	//�v���C���[������}�X�����F���Ȃ�
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			panels[i][j].tex->Update();
			panels[i][j].tex->SetPosition(panels[i][j].position);
			panels[i][j].tex->SetColor(panels[i][j].color);
		}
	}

	//�G�̗\���G���A�̂ݏ���
	if (name == "ENEMY") {
		FlashArea();
	}
}

//�`��
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
//���Z�b�g
void PredictArea::ResetPredict() {
	for (int i = 0; i < PREDICT_WIDTH; i++) {
		for (int j = 0; j < PREDICT_HEIGHT; j++) {
			panels[i][j].predict = false;
		}
	}
	m_FlashStart = false;
}
//�\���G���A�̃t���b�V��
void PredictArea::FlashArea() {
	if (m_FlashStart) {
		m_AddAngle = Helper::GetInstance()->Lerp(10.0f, 20.0f,m_Timer, m_TargetTimer);		//���`��ԂŃ`���[�W��\���Ă�
		//sin�g�ɂ���ď㉺�ɓ���
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