#pragma once
#include"BaseActor.h"

class MapSceneActor :public BaseActor {
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
	enum {
		Screen=0,
		StartMAP,
		NormalMAP,
		BossMAP,
		HealMAP,
		Max
	};
	struct UI {
		unique_ptr<IKESprite> sprite;
		XMFLOAT2 pos;
		XMFLOAT2 size;
		bool isPannel = false;
	};

	std::list<UI> UIs;
	XMFLOAT2 basePos = {340.f,360.f};
	XMFLOAT2 interbal = { 300.f,200.f };
	XMFLOAT2 scroll = { 0,0 };
};

