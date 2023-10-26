#include "BaseScene.h"
#include "ImageManager.h"
#include "VariableCommon.h"
#include "ParticleEmitter.h"
#include "ParticleEmitter2D.h"
#include "Helper.h"

BaseScene::BaseScene()
{
	game_object_manager_ = std::make_shared<GameObjectManager>();
}

//共通の初期化
void BaseScene::BaseInitialize(DirectXCommon* dxCommon, XMFLOAT3 eye, XMFLOAT3 target) {
	// デバイスをセット
	IKEFBXObject3d::SetDevice(dxCommon->GetDev());
	// グラフィックスパイプライン生成
	IKEFBXObject3d::CreateGraphicsPipeline();
	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	IKETexture::SetCamera(camera);
	// 3Dオブジェクトにカメラをセット
	IKEObject3d::SetCamera(camera);
	IKEFBXObject3d::SetCamera(camera);
	ParticleManager::SetCamera(camera);
	ParticleManager2D::SetCamera(camera);
	// ライト生成
	lightGroup = LightGroup::Create();
	// 3Dオブエクトにライトをセット
	IKEObject3d::SetLightGroup(lightGroup);

	ParticleEmitter::GetInstance()->AllDelete();
	//丸影のためのやつ
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	//ポストエフェクトの初期化
	//(普通)
	postEffect = make_unique<PostEffect>();
	postEffect->Initialize();
	//ポストエフェクトのファイル指定
	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/PostEffectTestPS.hlsl");

	camerawork = make_unique<CameraWork>(eye, target);
}
