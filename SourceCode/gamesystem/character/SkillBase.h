#pragma once
#include "DirectXMath.h"
#include <string>

class SkillBase
{
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="name">���O</param>
	/// <param name="ID">ID</param>
	/// <param name="latency">�ҋ@����</param>
	/// <param name="invocatingTime">��������</param>
	/// <param name="restraintTime">�S������</param>
	/// <param name="rarity">���A���e�B</param>
	/// <param name="popRate">�o�������N</param>
	void Create(const std::string name, int ID, float latency = 0.0f, float invocatingTime = 0.0f, float restraintTime = 0.0f, int rarity = 1, int popRate = 1);

protected:
	//�X�L����
	std::string name = "NoName";
	//ID
	int ID = 0;
	//�A�C�R��
	
	//�ҋ@����
	float latency = 0.0f;
	//��������
	float invocatingTime = 0.0f;
	//�S������
	float restraintTime = 0.0f;
	//���A���e�B
	int rarity = 1;
	//�o�������N
	int popRate = 1;
};

