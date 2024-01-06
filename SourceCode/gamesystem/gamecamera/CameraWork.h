#pragma once
#include "DebugCamera.h"
#include "Shake.h"
#include <memory>
#include "Feed.h"
#include"Spline.h"
#include "IKESprite.h"
using namespace std;         //  名前空間指定

//ゲームのカメラの動き
class CameraWork {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	XMFLOAT3 eye_ = { -1.5f,7.f,-3.f };

	XMFLOAT3 target_ = { -1.5f,2.5f,0.f };
public:
	CameraWork(XMFLOAT3 eye = { 2.0f, 30.0f, 2.0f }, XMFLOAT3 target = { 2.0f, 0.0f, 3.0f });
	void SplineSet();
	void Update(DebugCamera* camera);//更新
	void BossUpdate(DebugCamera* camera);
	void ClearUpdate(DebugCamera* camera);
	void CameraSKip();
	void ImGuiDraw();
	void Draw();
private:
	XMFLOAT3 m_AfterEye = { -3.0f,1.0f,3.0f };
	XMFLOAT3 m_AfterTarget = { 3.0f,1.0f,3.0f };

	enum CameraMove {
		CAMERA_BOSS,
		CAMERA_UP,
		CAMERA_RETURN
	}_CameraMove = CAMERA_BOSS;

	float m_Frame = {};
	int m_waitTimer = {};

	unique_ptr<IKESprite> syuutyuu;
};