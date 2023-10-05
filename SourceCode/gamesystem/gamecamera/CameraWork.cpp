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
	camera->SetEye({ 3.0f,10.0f,2.9f });
	camera->SetTarget({ 3.0f,0.0f,3.0f });
	camera->Update();
}
void CameraWork::ImGuiDraw() {

}