#pragma once
#include "BaseActor.h"
#include "IKESprite.h"
#include "JsonLoader.h"
#include <map>
#include <array>
/// タイトルシーン
class LoadSceneActor : public BaseActor {
public:
	/// 初期化
	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 終了
	void Finalize() override;
	/// 毎フレーム更新
	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
	/// 描画
	void Draw(DirectXCommon* dxCommon) override;
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void SpriteDraw();
private:
	void IntroUpdate(DebugCamera* camera)override;
	void MainUpdate(DebugCamera* camera)override;
	void FinishUpdate(DebugCamera* camera)override;


private://メンバ変数

};