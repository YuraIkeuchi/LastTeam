#pragma once
#include"IKESprite.h"


class ResultReport {
public:
	ResultReport();
	~ResultReport();

	void Initialize();
	void Update();
	void Draw(DirectXCommon* dxCommon);
private:
	std::unique_ptr<IKESprite> backScreen;
	std::unique_ptr<IKESprite> rate;

	//std::unique_ptr<IKESprite> backScreen;



};