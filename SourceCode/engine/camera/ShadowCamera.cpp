#include "ShadowCamera.h"
#include "WinApp.h"
using namespace DirectX;

void ShadowCamera::Initialize(const XMFLOAT3& eye, const XMFLOAT3& target)
{
	//カメラ初期化
	Camera::UpdateViewMatrix();
	Camera::UpdateProjectionMatrix();

	//視点、注視点をセット
	this->eye = eye;
	this->target = target;
	up = { 1, 0, 0 };
}

void ShadowCamera::UpdateProjectionMatrix()
{
	matProjection = XMMatrixOrthographicOffCenterLH(
		-20, 20,
		-20, 20,
		0.1f, 2000.0f
	);
}