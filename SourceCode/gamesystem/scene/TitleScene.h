#pragma once
#include "BaseScene.h"
#include "Font.h"
#include<memory>

/// タイトルシーン
class TitleScene : public BaseScene {
public:
	/// 初期化
	void Initialize(DirectXCommon* dxCommon) override;
	/// 終了
	void Finalize() override;
	/// 毎フレーム更新
	void Update(DirectXCommon* dxCommon) override;
	/// 描画
	void Draw(DirectXCommon* dxCommon) override;

private:
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void FrontDraw(DirectXCommon* dxCommon);

private:
	std::array<unique_ptr<Font>,3> texts;
	std::array<bool, 3> m_Check;

	enum SceneType {
		PLAY,
		MAP,
		TUTORIAL
	}_SceneType = PLAY;
};