#pragma once
#include"InterEnemy.h"
using namespace std;         //  名前空間指定
//普通の敵
class MobEnemy :public InterEnemy {
public:
	MobEnemy();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ClearAction()override;//クリア更新
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	
private:

protected:

private:

};

