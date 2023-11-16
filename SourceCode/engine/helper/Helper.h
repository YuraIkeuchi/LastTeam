#pragma once
#include "Camera.h"
#include "WinApp.h"
#include <random>
/*--円周とか求める用--*/
#define PI 3.14f
#define PI_90 90.00f
#define PI_180 180.00f
#define PI_360 360.00f
/*-----------------*/

//補助クラス
class Helper
{
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static Helper* GetInstance();
	Camera* GetCamera() { return m_Camera; }
	void SetCamera(Camera* camera) {
		m_Camera = camera;
	}
	//Clampを返す関数
	template<typename T>
	//計算系
	bool CheckMax(T& Num, const T Max, const T Add);
	//Clampを返す関数
	template<typename T>
	bool CheckMin(T& Num, const T Min, const T Add);
	bool FrameCheck(float& frame, const float addframe);
	//ランダム関数
	int GetRanNum(int min, int max);
	//Clampを返す関数
	template<typename T>
	void Clamp(T& Num, const T Min, const T Max);
	//XMFLOAT3とfloatを加算する関数
	XMFLOAT3 Float3AddFloat(const XMFLOAT3& Num, const float Add);
	//XMFLOAT3とXMFLOAT3を加算する関数
	XMFLOAT3 Float3AddFloat3(const XMFLOAT3& Num, const XMFLOAT3& Add);
	//XMFLOAT3とfloatを減算する関数
	XMFLOAT3 Float3SubFloat(const XMFLOAT3& Num, const float Sub);
	//XMFLOAT3とXMFLOAT3を減算する関数
	XMFLOAT3 Float3SubFloat3(const XMFLOAT3& Num, const XMFLOAT3& Sub);
	//線形補間で返す
	float Lerp(const float start, const float end, const int nowtime, const int targettime);
	//追従関数
	void FollowMove(XMFLOAT3& pos, const XMFLOAT3& pos2, const float vel);
	//Sin波
	void SinMove(XMFLOAT3& pos, const float angle, const float angle2);
	//距離を測る
	float ChechLength(const XMFLOAT3& pos, const XMFLOAT3& pos2);
	//円運動
	XMFLOAT3 CircleMove(const XMFLOAT3& basepos, const float scale, const float speed);
	//if文の判定の関数(値が範囲外にあるか)
	bool CheckNotValueRange(float& Num, const float Min, const float Max);
	//逆に範囲内にあるか
	bool CheckValueRange(float& Num, const float Min, const float Max);
public:
	//割合を返す
	float GetPercent(const float amount, const float num);
	//数字の桁数を返す
	int GetDigit(int n);
	//座標系が明確な場合->そのまま計算->W除算
	XMVECTOR WDivision(const XMVECTOR& pos, const bool sub);
	//2D->3D変換に使う(スプライトの座標をプレイヤーのワールド座標に表示したりするのに使う)
	//座標系が明確でない場合->行列から計算->W除算
	XMVECTOR WDivision2(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub);
	//ワールド座標に変換
	XMVECTOR PosDivi(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub);
	//座標変換
	XMVECTOR WorldDivision(const XMVECTOR& pos, const bool sub);
	//3D座標を2Dに変える(レティクルなどの当たり判定などに使う
	XMFLOAT2 WorldtoScreen(const XMVECTOR& pos, const XMMATRIX& mat);
	//XMFLOATをXMVECTORに変換する
	XMVECTOR ChangeFLOAT(const XMFLOAT3& pos);
	//XMFLOATをXMVECTORに変換する
	XMFLOAT3 ChangeVECTOR(const XMVECTOR& pos);
	void ChangeViewPort(XMMATRIX& matviewport, const XMVECTOR& offset);

	float DirRotation(const XMFLOAT3& target, const XMFLOAT3& base,float margin);
	//配列の中身が全部trueか
	bool All_Of(bool* flag, int size);
	bool All_OfF(bool* flag, int size);
	bool All_Of_float(float *list,int size,float standVal);
	//割合を返す
	int getDigits(int value, int m, int n);
private:

	//カメラ
	Camera* m_Camera;

};


namespace Easing
{
	inline int EaseIn(int t)
	{
		return t * t;
	}

	inline float EaseOut(float t, float start, float end)
	{
		return (end - start) * (1.0f - cosf(t * PI / 2.0f)) + start;
	}

	inline float EaseInOut(int& t);
	inline float EaseOutIn(int& t);
}

template<typename T>
inline void Helper::Clamp(T& Num, const T Min, const T Max) {
	Num = min(max(Num, Min), Max);
}

//float型
//max minをbool分で返す
template<typename T>
inline bool Helper::CheckMax(T& Num, const T Max, const T Add) {
	Num += Add;

	Num = max(Num, Max);


	if (Num <= Max) {
		return true;
	}
	else {
		return false;
	}

	return false;
}
//Minを検索
template<typename T>
inline bool Helper::CheckMin(T& Num, const T Min, const T Add) {
	Num += Add;
	Num = min(Num, Min);

	if (Num >= Min) {
		return true;
	}
	else {
		return false;
	}

	return false;
}
//乱数
inline int Helper::GetRanNum(int min, int max)
{
	if (min > max) 
	{
		max = min;
	}
	// 乱数エンジンのシード値
	std::random_device randev;
	// メルセンヌ・ツイスター
	std::mt19937 eng(randev());
	// 範囲
	std::uniform_int_distribution<int> dist(min, max);

	return dist(eng);
}