#include "Framework.h"
#include "IKEFbxLoader.h"
#include "ParticleEmitter.h"
#include "AudioManager.h"
#include "Font.h"
#include "SceneSave.h"
#include <TextManager.h>

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
void Framework::Initialize(DirectXCommon* m_DirectXCommon)
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
	IKESprite::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	//// デバッグテキスト用テクスチャ読み込み
	//if (!IKESprite::LoadTexture(debugTextTexNumber, L"Resources/2d/debugfont.png")) {
	//	assert(0);
	//	return;
	//}
	// ライト静的初期化
	LightGroup::StaticInitialize(dxcommon->GetDev());
	// デバッグテキスト初期化

	IKEObject3d::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	// デバイスをセット
	IKEFBXObject3d::SetDevice(dxcommon->GetDev());
	// グラフィックスパイプライン生成
	IKEFBXObject3d::CreateGraphicsPipeline();

	IKETexture::StaticInitialize(dxcommon->GetDev(), dxcommon->GetCmdList(), WinApp::window_width, WinApp::window_height);
	
	// FBX関連静的初期化
	IKEFbxLoader::GetInstance()->Initialize(dxcommon->GetDev());
	//リソースマネージャーの読み込み
	ModelManager::GetInstance()->StartInitialize();
	ImageManager::GetInstance()->StartLoad2D();
	ImageManager::GetInstance()->StartLoadTex2D();
	AudioManager::GetInstance()->StartLoadAudio();
	//シャドウマップの共通初期化
	ShadowMap::ShadowMapCommon(dxcommon->GetDev(),dxcommon->GetCmdList());
	shadowmap = ShadowMap::Create();
	// パーティクルマネージャ初期化
	ParticleManager::CreateCommon(dxcommon->GetDev(), dxcommon->GetCmdList());
	//パーティクルエミッター初期化
	ParticleEmitter::GetInstance()->Initialize();
	Font::Initialize(dxcommon);
	//TextManager::GetInstance()->Initialize();
	//Font::StaticInitialize(dxcommon,dxcommon->GetDev(),dxcommon->GetCmdList());
	
	SceneSave::GetInstance()->AllReset();

}

void Framework::Finalize()
{
	ModelManager::GetInstance()->Finalize();
	IKESprite::Finalize();
	IKETexture::Finalize();
	LightGroup::Finalize();
	SceneManager::GetInstance()->Finalize();
	input->GetInstance()->Finalize();
	dxcommon->Finalize();
	//dxcommon->Reset();
	delete dxcommon;
	winApp->Finalize();
	delete winApp;
}

void Framework::Update(DirectXCommon* m_DirectXCommon)
{
	//ウィンドウメッセージ処理
	if (winApp->ProcessMessage()) {
		//ゲームループ終了
		endRequst = true;
		return;
	}

	input->Update();

	//シーン更新処理
	SceneManager::GetInstance()->Update(m_DirectXCommon);
}

void Framework::Draw(DirectXCommon* m_DirectXCommon)
{
	//m_DirectXCommon->PreDraw();

	//シーン描画
	SceneManager::GetInstance()->Draw(m_DirectXCommon);

	//m_DirectXCommon->PostDraw();
	//でバックテキストの描画
	//debugText->DrawAll();
}