#include "CameraWork.h"
#include "VariableCommon.h"
#include <Easing.h>
#include "Player.h"
#include "Helper.h"
CameraWork::CameraWork(XMFLOAT3 eye, XMFLOAT3 target) {

}
void CameraWork::SplineSet() {

}
//XV
void CameraWork::Update(DebugCamera* camera) {
	camera->SetEye({ 0.0f,10.0f,-10.0f });
	camera->SetTarget({ 0.0f,0.0f,0.0f });
	camera->Update();
}
void CameraWork::ImGuiDraw() {

}