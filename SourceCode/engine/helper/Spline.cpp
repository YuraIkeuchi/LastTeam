#include "Spline.h"
#include <sysinfoapi.h>


void Spline::Init(std::vector<XMFLOAT3>point, int pointssize)
{
	points.push_back({ point[0].x, point[0].y, point[0].z });
	points.push_back({ point[0].x, point[0].y, point[0].z });
	for (int i = 1; i < pointssize- 1; i++)
		points.push_back({ point[i].x, point[i].y, point[i].z });
	
	int quantity_end = pointssize - 1;
	points.push_back({ point[quantity_end].x,  point[quantity_end].y, point[quantity_end].z });
	points.push_back({ point[quantity_end].x,  point[quantity_end].y,  point[quantity_end].z });

	startCount = static_cast<float>(GetTickCount());

}

XMVECTOR Spline::SplinePosition(const std::vector<XMVECTOR>& points, size_t startindex, float t)
{
	size_t n = points.size() - 2;
	if (startindex > n)
	{
		return points[n];
	}
	if (startindex < 1)
	{
		return points[1];
	}

	XMVECTOR p0 = points[startindex - 1];
	XMVECTOR p1 = points[startindex];
	XMVECTOR p2 = points[startindex + 1];
	XMVECTOR p3 = points[startindex + 2];

	XMVECTOR position = {
		2 * p1 + (-p0 + p2) * t +
		(2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) +
		(-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t)
	};
	position *= 0.5f;
	return position;
}


void Spline::Upda(XMFLOAT3& Epos, float DiviSpeed)
{
	nowCount = static_cast<float>(GetTickCount());
	elapsedCount = nowCount - startCount;
	elapsedTime = elapsedCount / DiviSpeed;

	timerate = elapsedTime / maxtime;
	if (timerate >= 1)
	{
		if (startindex < points.size() - 3)
		{
			startindex++;
			timerate -= 1;

			startCount = static_cast<float>(GetTickCount());
		} else
		{
			timerate = 1;
		}
	}

	Epos={
		SplinePosition(points, startindex, timerate).m128_f32[0],
		SplinePosition(points, startindex, timerate).m128_f32[1],
		SplinePosition(points, startindex, timerate).m128_f32[2]
		};
}

