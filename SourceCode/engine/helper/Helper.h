#pragma once
#include "Camera.h"
#include "WinApp.h"
#include <random>
/*--�~���Ƃ����߂�p--*/
#define PI 3.14f
#define PI_90 90.00f
#define PI_180 180.00f
#define PI_360 360.00f
/*-----------------*/

//�⏕�N���X
class Helper
{
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static Camera* GetCamera() { return m_Camera; }
	static void SetCamera(Camera* camera) {
		m_Camera = camera;
	}
	//Clamp��Ԃ��֐�
	template<typename T>
	//�v�Z�n
	static bool CheckMax(T& Num, const T Max, const T Add);
	//Clamp��Ԃ��֐�
	template<typename T>
	static bool CheckMin(T& Num, const T Min, const T Add);
	static bool FrameCheck(float& frame, const float addframe);
	//�����_���֐�
	static int GetRanNum(int min, int max);
	//Clamp��Ԃ��֐�
	template<typename T>
	static void Clamp(T& Num, const T Min, const T Max);
	//XMFLOAT3��float�����Z����֐�
	static XMFLOAT3 Float3AddFloat(const XMFLOAT3& Num, const float Add);
	//XMFLOAT3��XMFLOAT3�����Z����֐�
	static XMFLOAT3 Float3AddFloat3(const XMFLOAT3& Num, const XMFLOAT3& Add);
	//XMFLOAT3��float�����Z����֐�
	static XMFLOAT3 Float3SubFloat(const XMFLOAT3& Num, const float Sub);
	//XMFLOAT3��XMFLOAT3�����Z����֐�
	static XMFLOAT3 Float3SubFloat3(const XMFLOAT3& Num, const XMFLOAT3& Sub);
	//���`��ԂŕԂ�
	static float Lerp(const float start, const float end, const int nowtime, const int targettime);
	//�Ǐ]�֐�
	static void FollowMove(XMFLOAT3& pos, const XMFLOAT3& pos2, const float vel);
	//Sin�g
	static void SinMove(XMFLOAT3& pos, const float angle, const float angle2);
	//�����𑪂�
	static float ChechLength(const XMFLOAT3& pos, const XMFLOAT3& pos2);
	//�~�^��
	static XMFLOAT3 CircleMove(const XMFLOAT3& basepos, const float scale, const float speed);
	//if���̔���̊֐�(�l���͈͊O�ɂ��邩)
	static bool CheckNotValueRange(float& Num, const float Min, const float Max);
	//�t�ɔ͈͓��ɂ��邩
	static bool CheckValueRange(float& Num, const float Min, const float Max);
public:
	//������Ԃ�
	static float GetPercent(const float amount, const float num);
	//�����̌�����Ԃ�
	static int GetDigit(int n);
	//���W�n�����m�ȏꍇ->���̂܂܌v�Z->W���Z
	static XMVECTOR WDivision(const XMVECTOR& pos, const bool sub);
	//2D->3D�ϊ��Ɏg��(�X�v���C�g�̍��W���v���C���[�̃��[���h���W�ɕ\�������肷��̂Ɏg��)
	//���W�n�����m�łȂ��ꍇ->�s�񂩂�v�Z->W���Z
	static XMVECTOR WDivision2(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub);
	//���[���h���W�ɕϊ�
	static XMVECTOR PosDivi(const XMVECTOR& pos, const XMMATRIX& mat, const bool sub);
	//���W�ϊ�
	static XMVECTOR WorldDivision(const XMVECTOR& pos, const bool sub);
	//3D���W��2D�ɕς���(���e�B�N���Ȃǂ̓����蔻��ȂǂɎg��
	static XMFLOAT2 WorldtoScreen(const XMVECTOR& pos, const XMMATRIX& mat);
	//XMFLOAT��XMVECTOR�ɕϊ�����
	static XMVECTOR ChangeFLOAT(const XMFLOAT3& pos);
	//XMFLOAT��XMVECTOR�ɕϊ�����
	static XMFLOAT3 ChangeVECTOR(const XMVECTOR& pos);
	static void ChangeViewPort(XMMATRIX& matviewport, const XMVECTOR& offset);

	static float DirRotation(const XMFLOAT3& target, const XMFLOAT3& base,float margin);
	//�z��̒��g���S��true��
	static bool All_Of(bool* flag, int size);
	static bool All_OfF(bool* flag, int size);
	static bool All_Of_float(float *list,int size,float standVal);
	//������Ԃ�
	static int getDigits(int value, int m, int n);
private:

	//�J����
	static Camera* m_Camera;

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

//float�^
//max min��bool���ŕԂ�
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
//Min������
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
//����
inline int Helper::GetRanNum(int min, int max)
{
	if (min > max) 
	{
		max = min;
	}
	// �����G���W���̃V�[�h�l
	std::random_device randev;
	// �����Z���k�E�c�C�X�^�[
	std::mt19937 eng(randev());
	// �͈�
	std::uniform_int_distribution<int> dist(min, max);

	return dist(eng);
}