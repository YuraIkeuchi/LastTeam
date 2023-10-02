#pragma once
#include<DirectXMath.h>
#include<vector>

#include "DebugCamera.h"
using namespace DirectX;
class Spline
{
private:
	int pointsindex = 6;
	std::vector<XMFLOAT3> p = { {0.0f, 0.0f, 0.0f} };
	float elapsedTime = 0.0f;
	XMFLOAT3 pos1 = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 pos2 = { 0.0f, 0.0f, 0.0f };;
	size_t startindex = 1;
	std::vector<XMVECTOR> points = {};
	float startCount = 0.0f;
	float nowCount = 0.0f;
	float elapsedCount = 0.0f;

	float maxtime = 6.0f;
	float timerate = 0.0f;

	float time = 0.00000f;
	float j = time;

private:
	bool splinef = false;

public:
	void Init(std::vector<XMFLOAT3>points,int pointssize);
	void Upda(XMFLOAT3& Epos,float DiviSpeed=300.0f);
	XMVECTOR SplinePosition(const std::vector<XMVECTOR>& points, size_t startindex, float t);

	size_t GetIndex() { return startindex; }

	float GetTRate() { return startCount; }
};