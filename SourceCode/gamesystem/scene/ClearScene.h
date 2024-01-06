#pragma once
#include "IKESprite.h"
#include "BaseScene.h"
#include "EnemyManager.h"
/// タイトルシーン
class ClearScene : public BaseScene {
public:
	~ClearScene() override {};
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
private://メンバ変数
	unique_ptr<IKESprite> sprite;
	struct ShineEffect {
		unique_ptr<IKESprite> tex;
		float frame = 0.f;
		float kFrame = 30.f;
		XMFLOAT2 position = { 0.f,0.f };
		XMFLOAT2 size = { 0.f,0.f };
		bool isVanish = false;
	};

	void RandShineInit();
	void ShineEffectUpdate();
	std::list<ShineEffect> shines;
	unique_ptr<EnemyManager> enemyManager = nullptr;
};

