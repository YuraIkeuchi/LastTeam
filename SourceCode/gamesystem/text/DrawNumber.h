#pragma once
#include "IKESprite.h"
#include <vector>
#include <DirectXMath.h>
#include <array>
#include <memory>
using namespace DirectX;
using namespace std;

/// �����`��N���X
class DrawNumber
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	DrawNumber();
	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	void Draw();
	//�����̎w��Ƃ����W�Ƃ�
	void SetExplain(const XMFLOAT3& pos);
	//�ϊ��̏ꍇ�J�����f�[�^�����炤
	void GetCameraData();
public:
	void SetPosition(const XMFLOAT2& Position) { m_Position = Position; }
	void SetNumber(const int Number) { m_Number = Number; }
private:
private:
	static const int NUMBER_MAX = 10;
private:
	//Hp�̕\��(�Ƃ肠�����O��)
	array<unique_ptr<IKESprite>, NUMBER_MAX> _Number;
	XMFLOAT2 m_Position = { 0.0f,0.0f };

	//�J�����f�[�^�̂���
	XMMATRIX m_MatView = {};
	XMMATRIX m_MatProjection = {};
	XMMATRIX m_MatPort = {};

	//�`�悷�鐔��
	int m_Number = {};
};