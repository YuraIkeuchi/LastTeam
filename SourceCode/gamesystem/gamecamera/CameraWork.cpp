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
	camera->SetEye(eye_);
	camera->SetTarget(target_);

	camera->Update();
}
void CameraWork::ImGuiDraw() {

	ImGui::Begin("camera");
	ImGui::InputFloat("eye_x",&eye_.x,-100.f,100.f);
	ImGui::InputFloat("eye_y",&eye_.y,-100.f,100.f);
	ImGui::InputFloat("eye_z",&eye_.z,-100.f,100.f);
	ImGui::InputFloat("target_x", &target_.x, -100.f, 100.f);
	ImGui::InputFloat("target_y", &target_.y, -100.f, 100.f);
	ImGui::InputFloat("target_z", &target_.z, -100.f, 100.f);
	ImGui::End();

}