#pragma once
#include "IKESprite.h"
#include "BaseScene.h"
#include "EnemyManager.h"
/// タイトルシーン
class GameoverScene : public BaseScene {
public:
	~GameoverScene() override {};
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

	void GameOverMove();
private:
	static const int ATTACH_MAX = 2;
	static const int OVER_MAX = 2;
private://メンバ変数
	unique_ptr<EnemyManager> enemyManager = nullptr;
	int m_Timer = {};
	//パネル
	struct GameOverAttach {
		unique_ptr<IKEObject3d> object = nullptr;
		unique_ptr<IKETexture> line = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		float frame = {};
	};

	//タイトルのパーツに必要なもの
	array<float, OVER_MAX> m_Angle = { 0.0f,170.0f };
	array<float, OVER_MAX> m_Angle2;

	unique_ptr<IKESprite> gameover[OVER_MAX];
	unique_ptr<IKESprite> select[OVER_MAX];
	unique_ptr<IKESprite> stick;
	unique_ptr<IKESprite> check;

	array<XMFLOAT2,OVER_MAX> m_OverPos;
	array<XMFLOAT2,OVER_MAX> m_SelectPos;
	array<float, OVER_MAX> m_SelectRot;
	array<XMFLOAT2,OVER_MAX> m_OverSize;
	array<float, OVER_MAX> m_AddPower;
	array<GameOverAttach,ATTACH_MAX> attach;
	XMFLOAT2 m_StickPos = { 640.0f,900.0f };
	XMFLOAT2 m_CheckPos = { 0.0f,0.0f };

	array<XMFLOAT2, OVER_MAX> m_AfterOverPos;
	array<XMFLOAT2, OVER_MAX> m_AfterSelectPos;

	enum OverParts {
		PARTS_OVER,
		PARTS_COUNTINUE
	};

	enum SelectParts {
		SELECT_YES,
		SELECT_NO
	};

	enum OverMove {
		MOVE_NONE,
		MOVE_OVER,
		MOVE_COUNTINUE,
		MOVE_PANEL,
		MOVE_YES_SELECT,
		MOVE_NO_SELECT
	}_OverType = MOVE_NONE;

	array<float,OVER_MAX> m_Frame = {};
	//重力加速度
	float m_Gravity = 1.0f;
};

