#pragma once
#include "BaseScene.h"
#include "TextManager.h"
#include<memory>
#include"InterEnemy.h"
#include "IKESprite.h"
#include <array>
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
	unique_ptr<TextManager> text_;

	enum SceneType {
		PLAY,
		MAP,
		TUTORIAL
	}_SceneType = PLAY;

	unique_ptr<InterEnemy> enemy = nullptr;

	unique_ptr<IKESprite> title_;
	unique_ptr<IKESprite> player;
	float rota = 0.f;
	float rota2 = 0.f;

	float rot = 0.f * XM_PI / 180.f;
	vector<unique_ptr<IKESprite>> passives;
	vector<unique_ptr<IKESprite>> skills;

};