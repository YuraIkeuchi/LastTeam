#pragma once

#include <DirectXMath.h>

using namespace DirectX;
//球
struct Sphere
{
	//中心座標
	XMVECTOR center = { 0,0,0,1 };
	//半径
	float radius = 1.0f;
};

struct Box //箱
{
	//中心座標
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//半径
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
};

//平面
struct Plane
{
	//法線ベクトル
	XMVECTOR normal = { 0,1,0,0 };
	//原点(0,0,0)からの距離
	float m_Distance = 0.0f;
};

//法線付き三角形
class Triangle
{
public:
	//頂点座標　3つ
	XMVECTOR p0;
	XMVECTOR p1;
	XMVECTOR p2;

	//法線ベクトル
	XMVECTOR normal;

	//法線の計算	
	void ComputeNormal();
};

//レイ
class Ray
{
public:
	//始点座標
	XMVECTOR start = { 0,0,0,1 };
	//方向
	XMVECTOR dir = { 1,0,0,0 };
};
struct Point
{
	float x;
	float y;
};

struct Line2D
{
	DirectX::XMFLOAT2 start;
	DirectX::XMFLOAT2 end;
};

//OBB
class OBB
{
private:
	XMVECTOR m_Pos;//座標
	XMVECTOR m_NormalDirect[3];//方向ベクトル	
	float m_length[3];//各軸方向の長さ

public:
	DirectX::XMVECTOR GetDirect(int elem) { return m_NormalDirect[elem]; } // 指定軸番号の方向ベクトルを取得
	float GetLen_W(int elem) { return m_length[elem]; } // 指定軸方向の長さを取得
	DirectX::XMVECTOR GetPos_W() { return m_Pos; }//位置を取得


	void SetParam_Pos(XMMATRIX posMat);
	void SetParam_Pos(XMFLOAT3 pos);
	void SetParam_Scl(XMFLOAT3 scl);
	void SetParam_Rot(XMMATRIX rotMat);
};

