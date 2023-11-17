#pragma once
#include "InterAction.h"

class SkillAction :
	public InterAction {
public:
	SkillAction(const int SkillID);
	bool Initialize() override;//������
	void Pause() override;//�|�[�Y

	void Draw(DirectXCommon* dxCommon) override;//�`��
public:

private:
	void Action() override;//�s��

	void ImGui_Origin() override;

	//�G�t�F�N�g
	void EffecttexDraw(DirectXCommon* dxCommon) override;

	void GetData();
private:
	XMMATRIX m_MatView = {};
	XMMATRIX m_MatProjection = {};
	XMMATRIX m_MatPort = {};
};