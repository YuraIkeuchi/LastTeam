﻿#pragma once
#include"BaseActor.h"
#include"BossText.h"
#include "MessageWindow.h"
#include "EnemyManager.h"
class FirstStageActor :public BaseActor
{
public:
	/// 初期化
	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 終了
	void Finalize() override;
	/// 毎フレーム更新
	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 描画
	void Draw(DirectXCommon* dxCommon) override;
	void FrontDraw(DirectXCommon* dxCommon);
	void BackDraw(DirectXCommon* dxCommon);
private:
	void IntroUpdate(DebugCamera* camera)override;		//登場シーン
	void MainUpdate(DebugCamera* camera)override;		//バトルシーン
	void FinishUpdate(DebugCamera* camera)override;		//撃破シーン

	void ImGuiDraw();
private:
	XMFLOAT2 m_AddOffset = {};


	unique_ptr<EnemyManager> enemyManager = nullptr;

	unique_ptr<IKESprite> ui=nullptr;
};

