#pragma once
#include "InterAction.h"

class AttackAction :
	public InterAction {
public:
	AttackAction();
	bool Initialize() override;//������
	void Pause() override;//�|�[�Y

	void Draw(DirectXCommon* dxCommon) override;//�`��
public:

private:
	void Action() override;//�s��

	void ImGui_Origin() override;

	//�G�t�F�N�g
	void EffecttexDraw(DirectXCommon* dxCommon) override;
private:
};