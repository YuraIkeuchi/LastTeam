#include "ImagePlayer.h"
#include <ModelManager.h>
#include <Helper.h>
ImagePlayer::ImagePlayer() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
}

bool ImagePlayer::Initialize() {
	m_Rotation = { 0.0f,90.0f,0.0f };
	m_Color = { 0.1f,0.1f,0.1f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_Alive = true;
	m_AddDisolve = {};
	return true;
}
//çXêV
void ImagePlayer::Update() {
	const float l_AddDisolve = 0.1f;
	if (Helper::CheckMin(m_AddDisolve, 2.0f, l_AddDisolve)) {
		m_Alive = false;
	}

	Obj_SetParam();
}
//ï`âÊ
void ImagePlayer::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}