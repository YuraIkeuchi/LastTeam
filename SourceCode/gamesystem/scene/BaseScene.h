#pragma once
#include <memory>
#include "DebugCamera.h"
#include "LightGroup.h"
#include "DirectXCommon.h"
#include "PostEffect.h"
#include "GameObject/GameObjectManager.h"
#include "ParticleManager.h"
#include "CameraWork.h"
#include "SceneChanger.h"
#include"Font.h"
#include<SceneSave.h>

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
	void BaseInitialize(DirectXCommon* dxCommon, XMFLOAT3 eye = { 2.0f, 45.0f, 2.0f }, XMFLOAT3 target = { 2.0f, 0.0f, 3.0f });

	weak_ptr<GameObjectManager>GetGameObjectManager() { return game_object_manager_; }

public:
	// ゲームオブジェクトマネージャ
	shared_ptr<GameObjectManager> game_object_manager_;
	///ゲームシーン用
	DebugCamera* camera = nullptr;
	//ポストエフェクトをかけるか
	bool m_PlayPostEffect = false;
	//ライト
	LightGroup* lightGroup = nullptr;


	float ambientColor0[3] = { 1,1,1 };
	// 光線方向初期値
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//点光源
	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 15.0f,15.0f,15.0f };
	float pointLightPower[3] = { 20.0f,20.0f,20.0f };
	//スポットライト
	XMFLOAT3 spotLightPos[4];
	XMFLOAT3 spotLightDir[4];
	XMFLOAT3 spotLightColor[4];
	float spotLightAtten[3] = { 0.0,0.0f,0.0f };
	float spotLightFactorAngle[2] = { 20.0f,30.0f };
	//丸影(プレイヤー)
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 1.0f, 1.0f };

	unique_ptr<PostEffect> postEffect = nullptr;
	unique_ptr<CameraWork> camerawork;
	bool PlayPostEffect = false;
	//ゲームループしたかどうか
	static bool s_GameLoop;
	//シーンでの遷移
	//SceneState m_SceneState = SceneState::IntroState;
};