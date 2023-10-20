#pragma once
#include <vector>
#include <memory>

#include "DebugCamera.h"
#include "LightGroup.h"
#include "DirectXCommon.h"
#include "PostEffect.h"
#include "BaseActor.h"
#include "GameObject/GameObjectManager.h"

using namespace std;         //  名前空間指定
//前方宣言
class SceneManager;

//シーンインターフェース
class BaseScene {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;


public:
	// コンストラクタ
	BaseScene();
	//仮想デストラクタ
	virtual ~BaseScene() = default;

	//初期化
	virtual void Initialize(DirectXCommon* dxCommon) = 0;
	//開放
	virtual void Finalize() = 0;
	//更新
	virtual void Update(DirectXCommon* dxCommon) = 0;
	//描画
	virtual void Draw(DirectXCommon* dxCommon) = 0;
	//共通初期化
	void BaseInitialize(DirectXCommon* dxCommon);

	weak_ptr<GameObjectManager>GetGameObjectManager() { return game_object_manager_; }

public:
	// ゲームオブジェクトマネージャ
	shared_ptr<GameObjectManager> game_object_manager_;
	//アクタークラス
	unique_ptr<BaseActor> actor;
	///ゲームシーン用
	DebugCamera* camera = nullptr;
	//ポストエフェクトをかけるか
	bool m_PlayPostEffect = false;
	//ライト
	LightGroup* lightGroup = nullptr;
};