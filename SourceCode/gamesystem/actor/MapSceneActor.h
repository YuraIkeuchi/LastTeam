//#pragma once
//#include"BaseActor.h"
//
//class MapSceneActor :public BaseActor {
//public:
//	/// 初期化
//	void Initialize(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
//	/// 終了
//	void Finalize() override;
//	/// 毎フレーム更新
//	void Update(DirectXCommon* dxCommon, DebugCamera* camera, LightGroup* lightgroup) override;
//	/// 描画
//	void Draw(DirectXCommon* dxCommon) override;
//	void FrontDraw(DirectXCommon* dxCommon);
//	void BackDraw(DirectXCommon* dxCommon);
//private:
//	void IntroUpdate(DebugCamera* camera)override;		//登場シーン
//	void MainUpdate(DebugCamera* camera)override;		//バトルシーン
//	void FinishUpdate(DebugCamera* camera)override;		//撃破シーン
//	
//	struct UI {
//		unique_ptr<IKESprite> sprite = nullptr;
//		XMFLOAT2 pos = { 0,0 };
//		XMFLOAT2 size = { 0,0 };
//		int Tag = BATTLE;
//	};
//
//	UI  RandPannel();
//
//	void ImGuiDraw();
//private:
//	enum {
//		StartMAP=0,
//		NormalMAP,
//		BossMAP,
//		HealMAP,
//		Max
//	};
//	enum {
//		Tutorial = 0,
//		FirstChoice = 3,
//		SecondChoice = 8,
//		ThirdChoice = 15,
//		MaxCHOICE,
//	};
//	enum {
//		BATTLE=1,
//		BOSS,
//		HEAL,
//	};
//
//
//	unique_ptr<IKESprite>screen = nullptr;
//
//	array<UI, MaxCHOICE> UIs;
//	XMFLOAT2 basePos[5] = {
//		{340.f,360.f},
//		{640.f,160.f},
//		{940.f,-40.f},
//		{1240.0f,-240.f},
//		{1540.0f,-560.f}
//	};
//	XMFLOAT2 interbal = { 300.f,200.f };
//	XMFLOAT2 scroll = { 0,0 };
//};
//
