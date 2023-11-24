#include "StageBack.h"
#include <ImageManager.h>
StageBack* StageBack::GetInstance() {
	static StageBack instance;

	return &instance;
}

void StageBack::LoadResource() {
	_back = IKESprite::Create(ImageManager::STAGEBACK, { 0.f,0.f }, { 1.f,1.f, 1.f, 1.0f });
	_back->SetSize({ 1280.f,720.f });
}

void StageBack::Draw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	_back->Draw();
	IKESprite::PostDraw();
}
