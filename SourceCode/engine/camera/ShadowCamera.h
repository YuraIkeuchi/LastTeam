#pragma once
#include "Camera.h"
#include <DirectXMath.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include "DirectXCommon.h"


class ShadowCamera : public Camera
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const XMFLOAT3& eye, const XMFLOAT3& target = { 0, 0, 0 });

	/// <summary>
	/// 射影行列を更新
	/// </summary>
	void UpdateProjectionMatrix() override;
};