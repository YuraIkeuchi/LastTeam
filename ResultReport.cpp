#include "ResultReport.h"
#include "ImageManager.h"
#include <Input.h>

ResultReport::ResultReport() {
	Initialize();
}

ResultReport::~ResultReport() {
}

void ResultReport::Initialize() {
	backScreen = IKESprite::Create(ImageManager::RESULTREPORTBACK, { 0.f,0.f }, { 1.f,1.f, 1.f, 1.0f });
	rate =IKESprite::Create(ImageManager::RESULTREPORTATTACK, { 630.f,650.f }, { 1.f,1.f, 1.f, 1.0f });
	rate->SetAnchorPoint({0.5f,0.5f});

}

void ResultReport::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isFinish = true;
	}
}

void ResultReport::Draw(DirectXCommon* dxCommon) {
	if (isFinish) { return; }
	IKESprite::PreDraw();
	backScreen->Draw();
	rate->Draw();
	IKESprite::PostDraw();
}
