#include "CameraWork.h"
#include "VariableCommon.h"
#include <Easing.h>
#include "Player.h"
#include "Helper.h"
#include <GameStateManager.h>
#include <ImageManager.h>
CameraWork::CameraWork(XMFLOAT3 eye, XMFLOAT3 target) {
	syuutyuu = IKESprite::Create(ImageManager::SYUUTYUU, { 0.f,0.f });
}
void CameraWork::SplineSet() {

}
//XV
void CameraWork::Update(DebugCamera* camera) {
	camera->SetEye(eye_);
	camera->SetTarget(target_);

	camera->Update();
}

void CameraWork::Draw() {
	if (!m_DrawLine) { return; }
	IKESprite::PreDraw();
	syuutyuu->Draw();
	IKESprite::PostDraw();
}
void CameraWork::BossUpdate(DebugCamera* camera) {
	if (!GameStateManager::GetInstance()->GetBossCamera()) { return; }
	if (!GameStateManager::GetInstance()->GetGameStart()) { return; }
	float l_AddFrame = 1 / 30.0f;

	if (_CameraMove == CAMERA_BOSS) {
		l_AddFrame = 1 / 30.0f;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 1.0f;
			if (Helper::CheckMin(m_waitTimer, 50, 1)) {
				_CameraMove = CAMERA_UP;
				m_Frame = {};
				m_AfterEye = { 0.5f,1.5f,3.0f };
				m_waitTimer = {};
			}
		}
		else {
			eye_ = { Ease(In,Cubic,m_Frame,eye_.x,m_AfterEye.x),
			Ease(In,Cubic,m_Frame,eye_.y,m_AfterEye.y),
			Ease(In,Cubic,m_Frame,eye_.z,m_AfterEye.z), };

			target_ = { Ease(In,Cubic,m_Frame,target_.x,m_AfterTarget.x),
		Ease(In,Cubic,m_Frame,target_.y,m_AfterTarget.y),
		Ease(In,Cubic,m_Frame,target_.z,m_AfterTarget.z), };
		}
	}
	else if (_CameraMove == CAMERA_UP) {
		l_AddFrame = 1 / 30.0f;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 1.0f;
			if (Helper::CheckMin(m_waitTimer, 50, 1)) {
				m_DrawLine = false;
				_CameraMove = CAMERA_RETURN;
				m_Frame = {};
				m_AfterEye = { -1.5f,7.5f,-3.0f };
				m_AfterTarget = { -1.5f,2.5f,0.f };
			}
		}
		else {
			if (m_Frame >= 0.5f) {
				m_DrawLine = true;
			}
			eye_ = { Ease(In,Cubic,m_Frame,eye_.x,m_AfterEye.x),
			Ease(In,Cubic,m_Frame,eye_.y,m_AfterEye.y),
			Ease(In,Cubic,m_Frame,eye_.z,m_AfterEye.z), };

			target_ = { Ease(In,Cubic,m_Frame,target_.x,m_AfterTarget.x),
		Ease(In,Cubic,m_Frame,target_.y,m_AfterTarget.y),
		Ease(In,Cubic,m_Frame,target_.z,m_AfterTarget.z), };
		}
	}
	else {
		l_AddFrame = 1 / 60.0f;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			GameStateManager::GetInstance()->SetBossCamera(false);
		}
		else {
			eye_ = { Ease(In,Cubic,m_Frame,eye_.x,m_AfterEye.x),
			Ease(In,Cubic,m_Frame,eye_.y,m_AfterEye.y),
			Ease(In,Cubic,m_Frame,eye_.z,m_AfterEye.z), };

			target_ = { Ease(In,Cubic,m_Frame,target_.x,m_AfterTarget.x),
		Ease(In,Cubic,m_Frame,target_.y,m_AfterTarget.y),
		Ease(In,Cubic,m_Frame,target_.z,m_AfterTarget.z), };
		}
	}
}

void CameraWork::LastBossUpdate(DebugCamera* camera) {
	if (!GameStateManager::GetInstance()->GetBossCamera()) { return; }
	if (!GameStateManager::GetInstance()->GetGameStart()) { return; }
	float l_AddFrame = 1 / 30.0f;

	if (_CameraLastMove == CAMERA_LASTBOSS) {
		l_AddFrame = 1 / 30.0f;
		if (m_Frame == 0.0f) {
			m_AfterEye = { -6.0f,1.0f,3.0f };
			m_AfterTarget = { 3.0f,1.0f,3.0f };

		}
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 1.0f;
			if (Helper::CheckMin(m_waitTimer, 50, 1)) {
				_CameraLastMove = CAMERA_LASTSUPPORTUP;
				m_Frame = {};
				m_AfterEye = { -1.5f,1.0f,4.5f };
				m_AfterTarget = { 3.0f,1.0f,4.5f };
				m_waitTimer = {};
			}
		}
		else {
			eye_ = { Ease(In,Cubic,m_Frame,eye_.x,m_AfterEye.x),
			Ease(In,Cubic,m_Frame,eye_.y,m_AfterEye.y),
			Ease(In,Cubic,m_Frame,eye_.z,m_AfterEye.z), };

			target_ = { Ease(In,Cubic,m_Frame,target_.x,m_AfterTarget.x),
		Ease(In,Cubic,m_Frame,target_.y,m_AfterTarget.y),
		Ease(In,Cubic,m_Frame,target_.z,m_AfterTarget.z), };
		}
	}
	else if (_CameraLastMove == CAMERA_LASTSUPPORTUP) {
		l_AddFrame = 1 / 30.0f;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 1.0f;
			if (Helper::CheckMin(m_waitTimer, 50, 1)) {
				m_DrawLine = false;
				_CameraLastMove = CAMERA_LASTSUPPORTUP2;
				m_Frame = {};
				m_AfterEye = { -1.5f,1.0f,0.0f };
				m_AfterTarget = { 3.0f,1.0f,0.0f };
				m_waitTimer = {};
			}
		}
		else {
			if (m_Frame >= 0.5f) {
				m_DrawLine = true;
			}
			eye_ = { Ease(In,Cubic,m_Frame,eye_.x,m_AfterEye.x),
			Ease(In,Cubic,m_Frame,eye_.y,m_AfterEye.y),
			Ease(In,Cubic,m_Frame,eye_.z,m_AfterEye.z), };

			target_ = { Ease(In,Cubic,m_Frame,target_.x,m_AfterTarget.x),
		Ease(In,Cubic,m_Frame,target_.y,m_AfterTarget.y),
		Ease(In,Cubic,m_Frame,target_.z,m_AfterTarget.z), };
		}
	}
	else if (_CameraLastMove == CAMERA_LASTSUPPORTUP2) {
		l_AddFrame = 1 / 30.0f;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 1.0f;
			if (Helper::CheckMin(m_waitTimer, 50, 1)) {
				m_DrawLine = false;
				_CameraLastMove = CAMERA_LASTBOSSUP;
				m_Frame = {};
				m_AfterEye = { 0.5f,1.0f,3.0f };
				m_AfterTarget = { 3.0f,2.0f,3.0f };
				m_waitTimer = {};
			}
		}
		else {
			if (m_Frame >= 0.5f) {
				m_DrawLine = true;
			}
			eye_ = { Ease(In,Cubic,m_Frame,eye_.x,m_AfterEye.x),
			Ease(In,Cubic,m_Frame,eye_.y,m_AfterEye.y),
			Ease(In,Cubic,m_Frame,eye_.z,m_AfterEye.z), };

			target_ = { Ease(In,Cubic,m_Frame,target_.x,m_AfterTarget.x),
		Ease(In,Cubic,m_Frame,target_.y,m_AfterTarget.y),
		Ease(In,Cubic,m_Frame,target_.z,m_AfterTarget.z), };
		}
	}
	else if (_CameraLastMove == CAMERA_LASTBOSSUP) {
		l_AddFrame = 1 / 30.0f;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			m_Frame = 1.0f;
			if (Helper::CheckMin(m_waitTimer, 50, 1)) {
				m_DrawLine = false;
				_CameraLastMove = CAMERA_LASTRETURN;
				m_Frame = {};
				m_AfterEye = { -1.5f,7.5f,-3.0f };
				m_AfterTarget = { -1.5f,2.5f,0.f };
			}
		}
		else {
			if (m_Frame >= 0.5f) {
				m_DrawLine = true;
			}
			eye_ = { Ease(In,Cubic,m_Frame,eye_.x,m_AfterEye.x),
			Ease(In,Cubic,m_Frame,eye_.y,m_AfterEye.y),
			Ease(In,Cubic,m_Frame,eye_.z,m_AfterEye.z), };

			target_ = { Ease(In,Cubic,m_Frame,target_.x,m_AfterTarget.x),
		Ease(In,Cubic,m_Frame,target_.y,m_AfterTarget.y),
		Ease(In,Cubic,m_Frame,target_.z,m_AfterTarget.z), };
		}
	}
	else {
		l_AddFrame = 1 / 60.0f;
		if (Helper::FrameCheck(m_Frame, l_AddFrame)) {
			GameStateManager::GetInstance()->SetBossCamera(false);
		}
		else {
			eye_ = { Ease(In,Cubic,m_Frame,eye_.x,m_AfterEye.x),
			Ease(In,Cubic,m_Frame,eye_.y,m_AfterEye.y),
			Ease(In,Cubic,m_Frame,eye_.z,m_AfterEye.z), };

			target_ = { Ease(In,Cubic,m_Frame,target_.x,m_AfterTarget.x),
		Ease(In,Cubic,m_Frame,target_.y,m_AfterTarget.y),
		Ease(In,Cubic,m_Frame,target_.z,m_AfterTarget.z), };
		}
	}
}
void CameraWork::ImGuiDraw() {

	ImGui::Begin("camera");
	ImGui::InputFloat("eye_x",&eye_.x,-100.f,100.f);
	ImGui::InputFloat("eye_y",&eye_.y,-100.f,100.f);
	ImGui::InputFloat("eye_z",&eye_.z,-100.f,100.f);
	ImGui::InputFloat("target_x", &target_.x, -100.f, 100.f);
	ImGui::InputFloat("target_y", &target_.y, -100.f, 100.f);
	ImGui::InputFloat("target_z", &target_.z, -100.f, 100.f);
	ImGui::Text("Frame:%f", m_Frame);
	ImGui::Text("CameraMove:%d", _CameraLastMove);
	ImGui::End();
}
void CameraWork::CameraSKip() {
	eye_ = { -1.5f,7.5f,-3.0f };
	target_ = { -1.5f,2.5f,0.f };
	_CameraMove = CAMERA_BOSS;
	_CameraLastMove = CAMERA_LASTBOSS;
	m_DrawLine = false;
}
void CameraWork::ClearUpdate(DebugCamera* camera) {
	if (!m_ClearEnd) {
		eye_ = { -0.8f,4.f,-5.5f };
		target_ = { -0.8f,1.5f,0.f };
	}
	else {
		m_AddCameraVel = Ease(In, Cubic, 0.5f, m_AddCameraVel, 0.2f);
		eye_.y += m_AddCameraVel;
		eye_.z -= m_AddCameraVel;
	}
	camera->SetEye(eye_);
	camera->SetTarget(target_);

	camera->Update();
}
void CameraWork::GameOverUpdate(DebugCamera* camera) {
	eye_ = { 0.0f,3.f,-6.5f };
	target_ = { 0.0f,0.5f,3.0f };
}