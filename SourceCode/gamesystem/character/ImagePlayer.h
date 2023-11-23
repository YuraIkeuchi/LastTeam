#pragma once
#include "ObjCommon.h"

using namespace DirectX;

class ImagePlayer :
	public ObjCommon
{
public:
	ImagePlayer();
	//初期化
	bool Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw(DirectXCommon* dxCommon) override;

public:
	//gettersetter
	const bool GetAlive() { return m_Alive; }
private:
	bool m_Alive = false;
};
