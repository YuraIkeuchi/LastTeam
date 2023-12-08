#pragma once
#include "GameObject/GameObject.h"
#include "ObjCommon.h"


class AttackEfect :
	public ObjCommon
{
public:
	//初期化
	bool Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw(DirectXCommon* dxCommon) override;

};