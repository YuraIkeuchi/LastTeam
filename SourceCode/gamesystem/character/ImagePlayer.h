#pragma once
#include "ObjCommon.h"

using namespace DirectX;

class ImagePlayer :
	public ObjCommon
{
public:
	ImagePlayer();
	//������
	bool Initialize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw(DirectXCommon* dxCommon) override;

public:
	//gettersetter
	const bool GetAlive() { return m_Alive; }
private:
	bool m_Alive = false;
};
