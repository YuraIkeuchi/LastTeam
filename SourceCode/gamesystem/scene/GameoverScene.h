#pragma once
#include "IKESprite.h"
#include "BaseScene.h"
/// タイトルシーン
class GameoverScene : public BaseScene {
public:
	/// 初期化
	void Initialize(DirectXCommon* dxCommon) override;
	/// 終了
	void Finalize() override;
	/// 毎フレーム更新
	void Update(DirectXCommon* dxCommon) override;
	/// 描画
	void Draw(DirectXCommon* dxCommon) override;

private://メンバ変数
	unique_ptr<IKESprite> sprite;
};

