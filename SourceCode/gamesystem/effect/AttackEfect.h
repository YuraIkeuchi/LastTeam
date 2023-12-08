#pragma once
#include "GameObject/GameObject.h"
#include "ObjCommon.h"


class AttackEfect :
	public ObjCommon
{
public:
	//‰Šú‰»
	bool Initialize() override;
	//XV
	void Update() override;
	//•`‰æ
	void Draw(DirectXCommon* dxCommon) override;

};