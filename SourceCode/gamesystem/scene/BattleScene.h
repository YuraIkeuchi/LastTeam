#pragma once
#include "BaseScene.h"
#include"BossText.h"
#include "MessageWindow.h"
#include "EnemyManager.h"

//ゲームプレイシーン
class BattleScene : public BaseScene
{
public:
	//初期化
	void Initialize(DirectXCommon* dxCommon) override;
	//更新
	void Update(DirectXCommon* dxCommon) override;
	//描画
	void Draw(DirectXCommon* dxCommon) override;
	//開放
	void Finalize() override;
	
private:
	void FrontDraw(DirectXCommon* dxCommon);
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw();
private:

	unique_ptr<EnemyManager> enemyManager = nullptr;

	unique_ptr<IKESprite> ui = nullptr;

	enum ChangeType {
		CHANGE_MAP,
		CHANGE_OVER,
	}_ChangeType = CHANGE_MAP;
};