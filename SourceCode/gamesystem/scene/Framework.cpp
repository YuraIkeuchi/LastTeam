#include "Framework.h"
#include "IKEFbxLoader.h"
#include "ParticleEmitter.h"
#include "AudioManager.h"
#include "Font.h"
#include "SceneSave.h"
#include "TextManager.h"
#include <SkillManager.h>
#include <PassiveManager.h>
void Framework::Run()
{
	Initialize(dxcommon);
	while (true) {
		if (FPSManager::GetInstance()->Run()) {
			//毎フレーム更新
			Update(dxcommon);

			//終了リクエストが来たら抜ける
			if (endRequst) {
				break;
			}

			//描画
			Draw(dxcommon);
		}
	}
	//解放処理
	Finalize();

}
void Framework::Initialize(DirectXCommon* dxCommon)
{
	winApp = new WinApp();
	winApp->Initialize();
	dxcommon = new DirectXCommon();
	dxcommon->Initialize(winApp);
	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize(winApp);

	audio = Audio::GetInstance();
	audio->Initialize();
	//FPS固定
	fpsManager = FPSManager::GetInstance();
	fpsManager->Init();
	// nullptrチェック
	assert(dxcommon);
	assert(input);
	assert(audio);
	const int debugTextTexNumber = 0;
	IKESprite::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	// デバッグテキスト用テクスチャ読み込み
	if (!IKESprite::LoadTexture(debugTextTexNumber, L"Resources/2d/debugfont.png")) {
		assert(0);
		return;
	}
	// ライト静的初期化
	LightGroup::StaticInitialize(dxcommon->GetDev());
	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	IKEObject3d::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	IKETexture::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);

	// FBX関連静的初期化
	IKEFbxLoader::GetInstance()->Initialize(dxcommon->GetDev());
	//リソースマネージャーの読み込み
	ModelManager::GetInstance()->StartInitialize();
	ImageManager::GetInstance()->StartLoad2D();
	ImageManager::GetInstance()->StartLoadTex2D();
	AudioManager::GetInstance()->StartLoadAudio();
	//シャドウマップの共通初期化
	ShadowMap::ShadowMapCommon(dxcommon->GetDev(), dxcommon->GetCmdList());
	shadowmap = ShadowMap::Create();
	// パーティクルマネージャ初期化
	ParticleManager::CreateCommon(dxcommon->GetDev(), dxcommon->GetCmdList());
	//パーティクルエミッター初期化
	ParticleEmitter::GetInstance()->Initialize();
	Font::Initialize(dxcommon);
	//TextManager::GetInstance()->Initialize();
	//Font::StaticInitialize(dxcommon,dxcommon->GetDev(),dxcommon->GetCmdList());

	SceneSave::GetInstance()->AllReset();
	//ポストエフェクトのファイル指定
	PostEffect::CreateGraphicsPipeline(L"Resources/Shaders/PostEffectTestVS.hlsl", L"Resources/Shaders/PostEffectTestPS.hlsl");
	SkillManager::GetInstance()->Initialize(dxcommon);
	PassiveManager::GetInstance()->Initialize(dxcommon);
}

void Framework::Finalize()
{
	ModelManager::GetInstance()->Finalize();
	IKESprite::Finalize();
	IKETexture::Finalize();
	LightGroup::Finalize();
	SceneManager::GetInstance()->Finalize();
	input->GetInstance()->Finalize();
	delete sceneFactory_;
	dxcommon->Finalize();
	//dxcommon->Reset();
	delete dxcommon;
	winApp->Finalize();
	delete winApp;
}

void Framework::Update(DirectXCommon* dxCommon)
{
	//ウィンドウメッセージ処理
	if (winApp->ProcessMessage()) {
		//ゲームループ終了
		endRequst = true;
		return;
	}

	input->Update();

	//シーン更新処理
	SceneManager::GetInstance()->Update(dxCommon);
}

void Framework::Draw(DirectXCommon* dxCommon)
{
	//dxCommon->PreDraw();

	//シーン描画
	SceneManager::GetInstance()->Draw(dxCommon);

	//dxCommon->PostDraw();
	//でバックテキストの描画
	//debugText->DrawAll();
}