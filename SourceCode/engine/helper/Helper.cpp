#include "Helper.h"
#include"WinApp.h"	
#include "Easing.h"
Helper* Helper::GetInstance()
{
	static Helper instance;

	return &instance;
}
bool Helper::FrameCheck(float& frame, const float addframe) {
	if (frame < m_FrameMax) {
		frame += addframe;
		Clamp(frame, 0.f, 1.0f);
	}
	else {
		return true;
	}

	return false;
}
//XMFLOAT3とfloatを加算する関数
Helper::XMFLOAT3 Helper::Float3AddFloat(const XMFLOAT3& Num, float Add) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x + Add;
	l_Result.y = Num.y + Add;
	l_Result.z = Num.z + Add;

	return l_Result;
}
//XMFLOAT3とXMFLOAT3を加算する関数
Helper::XMFLOAT3 Helper::Float3AddFloat3(const XMFLOAT3& Num, const XMFLOAT3& Add) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x + Add.x;
	l_Result.y = Num.y + Add.y;
	l_Result.z = Num.z + Add.z;

	return l_Result;
}
//XMFLOAT3とfloatを減算する関数
Helper::XMFLOAT3 Helper::Float3SubFloat(const XMFLOAT3& Num, float Sub) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x - Sub;
	l_Result.y = Num.y - Sub;
	l_Result.z = Num.z - Sub;

	return l_Result;
}
//XMFLOAT3とXMFLOAT3を減算する関数
Helper::XMFLOAT3 Helper::Float3SubFloat3(const XMFLOAT3& Num, const XMFLOAT3& Sub) {
	XMFLOAT3 l_Result;
	l_Result.x = Num.x - Sub.x;
	l_Result.y = Num.y - Sub.y;
	l_Result.z = Num.z - Sub.z;

	return l_Result;
}
//割合を返す
float Helper::GetPercent(const float amount, const float num)
{
	return num / amount * 100.00f;
}
//2D->3D変換に使う(スプライトの座標をプレイヤーのワールド座標に表示したりするのに使う)
//座標系が明確でない場合->行列から計算->W除算
Helper::XMVECTOR Helper::WDivision(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub)
{
	float x, y, z, w;

	x = (pos.m128_f32[0] * mat.r[0].m128_f32[0]) + (pos.m128_f32[1] * mat.r[1].m128_f32[0]) + (pos.m128_f32[2] * mat.r[2].m128_f32[0]) + (1.0f * mat.r[3].m128_f32[0]);
	y = (pos.m128_f32[0] * mat.r[0].m128_f32[1]) + (pos.m128_f32[1] * mat.r[1].m128_f32[1]) + (pos.m128_f32[2] * mat.r[2].m128_f32[1]) + (1.0f * mat.r[3].m128_f32[1]);
	z = (pos.m128_f32[0] * mat.r[0].m128_f32[2]) + (pos.m128_f32[1] * mat.r[1].m128_f32[2]) + (pos.m128_f32[2] * mat.r[2].m128_f32[2]) + (1.0f * mat.r[3].m128_f32[2]);

	w = pos.m128_f32[3];
	if (sub) {
		w = z;
	}

	x = x / w;
	y = y / w;
	z = z / w;

	return XMVECTOR{ x, y, z, w };
}
//座標系が明確
Helper::XMVECTOR Helper::WDivision(const XMVECTOR& pos, const bool sub)
{
	float x, y, z, w;
	x = pos.m128_f32[0];
	y = pos.m128_f32[1];
	z = pos.m128_f32[2];
	w = pos.m128_f32[3];

	if (sub)
	{
		w = z;
	}

	x = x / w;
	y = y / w;
	z = z / w;

	return XMVECTOR{ x, y, z, w };
}
//ワールド座標に変換
Helper::XMVECTOR Helper::PosDivi(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub)
{
	float x, y, z, w;

	x = (pos.m128_f32[0] * mat.r[0].m128_f32[0]) + (pos.m128_f32[1] * mat.r[1].m128_f32[0]) + (pos.m128_f32[2] * mat.r[2].m128_f32[0]) + (1.0f * mat.r[3].m128_f32[0]);
	y = (pos.m128_f32[0] * mat.r[0].m128_f32[1]) + (pos.m128_f32[1] * mat.r[1].m128_f32[1]) + (pos.m128_f32[2] * mat.r[2].m128_f32[1]) + (1.0f * mat.r[3].m128_f32[1]);
	z = (pos.m128_f32[0] * mat.r[0].m128_f32[2]) + (pos.m128_f32[1] * mat.r[1].m128_f32[2]) + (pos.m128_f32[2] * mat.r[2].m128_f32[2]) + (1.0f * mat.r[3].m128_f32[2]);

	w = 1.0f;

	if (sub)
	{
		w = z;
	}

	return XMVECTOR{ x, y, z, w };
}
Helper::XMVECTOR Helper::WorldDivision(const XMVECTOR& pos, const bool sub) {
	XMVECTOR l_Result;
	l_Result = PosDivi(l_Result, m_Camera->GetViewMatrix(), false);
	l_Result = PosDivi(l_Result, m_Camera->GetProjectionMatrix(), true);
	l_Result = WDivision(pos, true);
	l_Result = PosDivi(l_Result, m_Camera->GetViewPort(), false);

	return l_Result;
}
//追従
void Helper::FollowMove(XMFLOAT3& pos, const XMFLOAT3& pos2, const float vel) {
	XMFLOAT3 l_Distance;//二点の距離
	XMFLOAT3 l_AddVel;//加算される移動量

	//とりあえずX軸とZ軸のみ追従
	l_Distance.x = pos.x - pos2.x;
	l_Distance.z = pos.z - pos2.z;

	l_AddVel.x = float(sin(-atan2f(l_Distance.x, l_Distance.z)) * vel);
	l_AddVel.z = float(-cos(atan2f(l_Distance.x, l_Distance.z)) * vel);
	l_AddVel.y = 0.0f;
	pos.y += 0.0f;
	pos.x += l_AddVel.x;
	pos.z += l_AddVel.z;

	//まだ回転とかは考慮してないよ
	//return l_Result;
}
float Helper::ChechLength(const XMFLOAT3& pos, const XMFLOAT3& pos2) {
	float result=0.0f;
	result = sqrtf(powf((pos2.x - pos.x), 2) + powf((pos2.y - pos.y), 2) + powf((pos2.z - pos.z), 2));

	return result;
}
//円運動
Helper::XMFLOAT3 Helper::CircleMove(const XMFLOAT3& basepos, const float scale, const float speed) {
	float l_Radius;//
	XMFLOAT3 l_AddVel;//移動量
	XMFLOAT3 l_Result{};//返す値
	//円運動の処理(X軸とZ軸)
	l_Radius = speed * PI / PI_180;
	l_AddVel.x = cosf(l_Radius) * scale;
	l_AddVel.z = sinf(l_Radius) * scale;
	l_Result.x = l_AddVel.x + basepos.x;
	l_Result.z = l_AddVel.z + basepos.z;
	l_Result.y = basepos.y;
	return l_Result;
}
//値が範囲外にあるか
bool Helper::CheckNotValueRange(float& Num, const float Min, const float Max) {
	if ((Num > Max || Num < Min)) {
		return true;
	} else {
		return false;
	}

	return true;
}
//値が範囲内にあるか
bool Helper::CheckValueRange(float& Num, const float Min, const float Max) {
	if ((Num < Max && Num > Min)) {
		return true;
	} else {
		return false;
	}

	return true;
}
//スクリーン座標に変換
Helper::XMFLOAT2 Helper::WorldtoScreen(const XMVECTOR& pos, const XMMATRIX& mat) {
	const float kDistancePlayerTo3DReticle = 50.0f;
	XMVECTOR l_OffSet;
	XMMATRIX MatViewPort;
	l_OffSet = { 0.0,0.0,1.0f };
	l_OffSet = XMVector3TransformNormal(l_OffSet, mat);
	l_OffSet = m_Camera->CameraNormalize(l_OffSet, kDistancePlayerTo3DReticle);
	XMVECTOR PositionRet = pos;

	ChangeViewPort(MatViewPort, l_OffSet);

	XMMATRIX MatVP = MatViewPort;

	XMMATRIX View = m_Camera->GetViewMatrix();
	XMMATRIX Pro = m_Camera->GetProjectionMatrix();

	XMMATRIX MatViewProjectionViewport = View * Pro * MatVP;

	PositionRet = XMVector3TransformCoord(PositionRet, MatViewProjectionViewport);

	XMFLOAT2 get2dposition;
	get2dposition.x = PositionRet.m128_f32[0];
	get2dposition.y = PositionRet.m128_f32[1];

	return get2dposition;
}
//ビューポート変換
void Helper::ChangeViewPort(XMMATRIX& matviewport, const XMVECTOR& offset) {
	matviewport.r[0].m128_f32[0] = WinApp::window_width / 2;
	matviewport.r[0].m128_f32[1] = 0;
	matviewport.r[0].m128_f32[2] = 0;
	matviewport.r[0].m128_f32[3] = 0;

	matviewport.r[1].m128_f32[0] = 0;
	matviewport.r[1].m128_f32[1] = -(WinApp::window_height / 2);
	matviewport.r[1].m128_f32[2] = 0;
	matviewport.r[1].m128_f32[3] = 0;

	matviewport.r[2].m128_f32[0] = 0;
	matviewport.r[2].m128_f32[1] = 0;
	matviewport.r[2].m128_f32[2] = 1;
	matviewport.r[2].m128_f32[3] = 0;

	matviewport.r[3].m128_f32[0] = WinApp::window_width / 2 + offset.m128_f32[0];
	matviewport.r[3].m128_f32[1] = WinApp::window_height / 2 + offset.m128_f32[1];
	matviewport.r[3].m128_f32[2] = 0;
	matviewport.r[3].m128_f32[3] = 1;
}
float Helper::DirRotation(const XMFLOAT3& target, const XMFLOAT3& base, float margin){
	float itr{};
	XMFLOAT3 pos = base;
	XMFLOAT3 position{};
	position.x = (target.x - pos.x);
	position.z = (target.z - pos.z);
	itr = (atan2f(position.x, position.z) * (PI_180 / DirectX::XM_PI)) + margin; //- 90;// *(XM_PI / 180.0f);
	if (itr >= 0) {
		itr = (float)((int)itr % (int)PI_360);
	} else {
		itr += PI_360;
		itr = (float)((int)itr % (int)PI_360);
	}
	return itr;
}

//XMFLOATをXMVECTORに変換
Helper::XMVECTOR Helper::ChangeFLOAT(const XMFLOAT3& pos) {
	XMVECTOR l_Result;
	l_Result.m128_f32[0] = pos.x;
	l_Result.m128_f32[1] = pos.y;
	l_Result.m128_f32[2] = pos.z;

	return l_Result;
}

//XMFLOATをXMVECTORに変換
Helper::XMFLOAT3 Helper::ChangeVECTOR(const XMVECTOR& pos) {
	XMFLOAT3 l_Result;
	l_Result.x = pos.m128_f32[0];
	l_Result.y = pos.m128_f32[1];
	l_Result.z = pos.m128_f32[2];

	return l_Result;
}

bool Helper::All_Of(bool* flag, int size)
{
	for (auto i = 0; i < size;)
	{
		//真ならカウント進める
		if (flag[i])i++;
		else return false;

		//今の要素がtrueなら
		if (i == size && flag[size - 1])return true;
	}
	return false;
}
bool Helper::All_OfF(bool* flag, int size)
{
	for (auto i = 0; i < size;)
	{
		//真ならカウント進める
		if (!flag[i])i++;
		else return false;

		//今の要素がtrueなら
		if (i == size && !flag[size - 1])return true;
	}
	return false;
}

bool Helper::All_Of_float(float* list, int size, float standVal)
{
	for (auto i = 0; i < size;)
	{
		if (list[i] == standVal)i++;
		else return false;

		if (i == size && list[size - 1] == standVal)return true;
	}

	return false;
}

void Helper::SinMove(XMFLOAT3& pos, const float angle, const float angle2) {

}