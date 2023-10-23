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
	void SetExplain(const int Number,const XMFLOAT3& pos);
	//�ϊ��̏ꍇ�J�����f�[�^�����炤
	void GetCameraData(const XMMATRIX& matView, const XMMATRIX& matProjection, const XMMATRIX& matPort);
private:
private:
	static const int NUMBER_MAX = 10;
private:
	//Hp�̕\��(�Ƃ肠�����O��)
	array<unique_ptr<IKESprite>, NUMBER_MAX> _Number;
	XMFLOAT2 m_NumberPos = { 0.0f,0.0f };

	//�J�����f�[�^�̂���
	XMMATRIX m_MatView = {};
	XMMATRIX m_MatProjection = {};
	XMMATRIX m_MatPort = {};

	//�`�悷�鐔��
	int m_Number = {};
};