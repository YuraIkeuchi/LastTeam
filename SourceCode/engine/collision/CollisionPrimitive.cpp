#include "CollisionPrimitive.h"

using namespace DirectX;

void Triangle::ComputeNormal()
{
	XMVECTOR p0_p1 = p1 - p0;
	XMVECTOR p0_p2 = p2 - p0;

	//外積により、2辺に垂直なベクトルを算出する
	normal = XMVector3Cross(p0_p1, p0_p2);
	normal = XMVector3Normalize(normal);
}


void OBB::SetParam_Pos(XMMATRIX posMat)
{
	m_Pos.m128_f32[0] = posMat.r[3].m128_f32[0];
	m_Pos.m128_f32[1] = posMat.r[3].m128_f32[1];
	m_Pos.m128_f32[2] = posMat.r[3].m128_f32[2];
}

void OBB::SetParam_Pos(XMFLOAT3 pos)
{
	m_Pos.m128_f32[0] = pos.x;
	m_Pos.m128_f32[1] = pos.y;
	m_Pos.m128_f32[2] = pos.z;
}

void OBB::SetParam_Scl(XMFLOAT3 scl)
{
	m_length[0] = scl.x;
	m_length[1] = scl.y;
	m_length[2] = scl.z;
}

void OBB::SetParam_Rot(XMMATRIX rotMat)
{
	m_NormalDirect[0] = { rotMat.r[0].m128_f32[0], rotMat.r[0].m128_f32[1], rotMat.r[0].m128_f32[2] };
	m_NormalDirect[1] = { rotMat.r[1].m128_f32[0], rotMat.r[1].m128_f32[1], rotMat.r[1].m128_f32[2] };
	m_NormalDirect[2] = { rotMat.r[2].m128_f32[0], rotMat.r[2].m128_f32[1], rotMat.r[2].m128_f32[2] };
}
