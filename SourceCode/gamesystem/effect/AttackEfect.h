#pragma once
#include "GameObject/GameObject.h"
#include "ObjCommon.h"


class AttackEfect :
	public ObjCommon
{
public:
	//������
	bool Initialize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw(DirectXCommon* dxCommon) override;

};