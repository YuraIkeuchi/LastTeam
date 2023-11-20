#pragma once
#include "IKESprite.h"
#include <memory>
class StageBack {
public:
	static StageBack* GetInstance();
private:

public:
	void LoadResource();
	//•`‰æ
	void Draw(DirectXCommon* dxCommon);

private:
	std::unique_ptr<IKESprite> _back;
};