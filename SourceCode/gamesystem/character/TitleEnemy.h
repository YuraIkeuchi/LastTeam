#pragma once
#include"InterEnemy.h"
using namespace std;         //  名前空間指定
//タイトルの敵
class TitleEnemy :public InterEnemy {
public:
	TitleEnemy();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//描画

private:

};

