#pragma once
#include "SkillBase.h"
class NormalSkill : public SkillBase
{
public:
	/// <summary>
	/// ����
	/// </summary>
	/// <param name="ID">ID</param>
	/// <param name="latency">�ҋ@����</param>
	/// <param name="invocatingTime">��������</param>
	/// <param name="restraintTime">�S������</param>
	/// <param name="rarity">���A���e�B�\</param>
	/// <param name="popRate">�o�������N</param>
	/// <param name="damege">�_���[�W</param>
	/// <param name="range">�͈�</param>
	/// <param name="distance">����</param>
	void Create(
		const std::string name,
		int ID,
		float latency = 0.0f,
		float invocatingTime = 0.0f,
		float restraintTime = 0.0f,
		int rarity = 1,
		int popRate = 1,
		float damege = 0.0f,
		XMFLOAT2 range = { 1.0f,1.0f },
		XMFLOAT2 distance = { 1.0f, 0.0f });

	void ImGui_Origin()override;
protected:
	//�_���[�W
	float damege = 0.0f;
	//�͈�
	XMFLOAT2 range = { 1.0f,1.0f };
	//���� ���v���C���[�̈ʒu����͈͂̈�ԍ���
	XMFLOAT2 distance = { 0.0f, 1.0f };
};
