#pragma once
#include "DirectXMath.h"
#include <string>

enum class SkillType
{
	none = -1,
	damege,
	buff,
	debuff,
	max,
};

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
	void Create(const std::string name, int ID, int latency = 0, int invocatingTime = 0, int restraintTime = 0, int rarity = 1, int popRate = 1);

	virtual void ImGui_Origin() = 0;
	void ImGuiDraw();
public:
	//getter setter
	const int GetID() { return ID; }
	const int Getlatency() { return latency; }
	const bool GetBirth() { return m_Birth; }
	const SkillType GetSkillType() { return skillType; }
	void SetID(const int ID) { this->ID = ID; }
	void SetLatency(const int latency) { this->latency = latency; }
	void SetName(const std::string& name) { this->name = name; }
	void SetInvocatingTime(const int invocatingTime) { this->invocatingTime = invocatingTime; }
	void SetRestraintTime(const int restraintTime) { this->restraintTime = restraintTime; }
	void SetRarity(const int rarity) { this->rarity = rarity; }
	void SetPopRate(const int popRate) { this->popRate = popRate; }
	void SetBirth(const bool Birth) { this->m_Birth = Birth; }
	void SetSkillType(SkillType skillType) { this->skillType = skillType; }
protected:
	//�X�L����
	std::string name = "NoName";
	//ID
	int ID = 0;
	//�A�C�R��
	
	//�ҋ@����
	int latency = 0;
	//��������
	int invocatingTime = 0;
	//�S������
	int restraintTime = 0;
	//���A���e�B
	int rarity = 1;
	//�o�������N
	int popRate = 1;
	bool m_Birth = false;
	SkillType skillType;
};