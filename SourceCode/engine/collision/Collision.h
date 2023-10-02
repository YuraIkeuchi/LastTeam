#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "CollisionPrimitive.h"
class Collision {
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//円と円
	static bool CircleCollision(float X1, float Y1, float R1, float X2, float Y2, float R2);

	static void ConvertToNomalizeVector(XMFLOAT2& out, XMFLOAT2 in);

	static float CalculationVectorLength(const XMFLOAT2& vec01);

	static bool IsCollidingLineAndCircle(Line2D line, Point circle, float dis);

	static float GetLength(XMFLOAT3 position, XMFLOAT3 position2);

	static float GetLength(XMFLOAT2 position, XMFLOAT2 position2);

	//当たり判定関数
	//球と球
	static bool SphereCollision(const XMFLOAT3& pos, const float& R1, const XMFLOAT3& pos2, const float& R2);
	//球と球2
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);
	//平面と球
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, XMVECTOR* inter = nullptr);
	//点と三角形
	static void ClodestPtPoint2Triangle(const XMVECTOR& point, const Triangle& triangle, XMVECTOR* closest);
	//球と三角形
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);
	//レイと平面
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* m_Distance = nullptr, XMVECTOR* inter = nullptr);
	//レイと三角形
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* m_Distance = nullptr, XMVECTOR* inter = nullptr);
	//レイと球
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* m_Distance = nullptr, XMVECTOR* inter = nullptr);
	//箱と球
	static bool CheckSphere2Box(const Sphere& sphere, const Box& box);
	//OBBとOBBの当たり判定
	static bool OBBCollision(OBB& obbA, OBB& obbB);
	// 分離軸に投影された軸成分から投影線分長を算出
	static double LenSegOnSeparateAxis(XMVECTOR* Sep, XMVECTOR* e1, XMVECTOR* e2, XMVECTOR* e3 = 0);
};