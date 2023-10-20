//#include "BaseActor.h"
//#include "VariableCommon.h"
//#include "Player.h"
//bool BaseActor::s_GameLoop = false;
//bool BaseActor::s_Skip = false;
//void (BaseActor::* BaseActor::stateTable[])(DebugCamera* camera) = {
//	&BaseActor::IntroUpdate,//要素0
//	&BaseActor::MainUpdate, //要素1
//	&BaseActor::FinishUpdate,
//};
//
//
////共通初期化
//void BaseActor::BaseInitialize(DirectXCommon* dxCommon, XMFLOAT3 eye, XMFLOAT3 target) {
//	//ポストエフェクトの初期化
//	//(普通)
//	postEffect = make_unique<PostEffect>();
//	postEffect->Initialize();
//	//ポストエフェクトのファイル指定
//	postEffect->CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/PostEffectTestPS.hlsl");
//
//	camerawork = make_unique<CameraWork>(eye, target);
//}
