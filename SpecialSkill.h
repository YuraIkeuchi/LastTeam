#pragma once
class SpecialSkill
{

public:
	/// <summary>
	/// HP�z���U��
	/// </summary>
	/// <param name="damege">�_���[�W</param>
	/// <param name="ratio">�z������</param>
	/// <returns>�z����</returns>
	float HPAbsorption(const float damege, const float ratio);
	
	/// <summary>
	/// �����_���[�W
	/// </summary>
	/// <param name="damege">�_���[�W</param>
	/// <param name="ratio">��������</param>
	/// <returns>������</returns>
	float SelfScratch(const float damege, const float ratio);



};

