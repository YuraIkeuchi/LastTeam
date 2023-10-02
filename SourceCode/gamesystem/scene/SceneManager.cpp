#include "SceneManager.h"
#include "ImageManager.h"
#include "ModelManager.h"
#include "AudioManager.h"

#include<cassert>
void SceneManager::Finalize() {
	//最後のシーンの終了と開放
	scene_->Finalize();
	delete scene_;
}

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

void SceneManager::Update(DirectXCommon* dxCommon) {
	//シーン切り替えがあるかどうか
	if (nextScene_) {
		//旧シーンの終了
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}
		scene_ = nextScene_;
		nextScene_ = nullptr;
		scene_->Initialize(dxCommon);
	}
	//ローディング
	if (m_Load == true) {
		switch (m_loadType)
		{
		case SceneManager::NoLoad://ロードしていないとき
			m_th = std::thread([&] {AsyncLoad(); });
			m_loadType = LoadStart;

			break;
		case SceneManager::LoadStart://ロードしているとき

			break;
		case SceneManager::LoadEnd://ロード終わったら
			m_th.join();
			m_loadType = NoLoad;
			m_Load = false;
			break;
		default:
			break;
		}
	}
	scene_->Update(dxCommon);
}

void SceneManager::Draw(DirectXCommon* dxCommon) {
	scene_->Draw(dxCommon);
}

void SceneManager::ChangeScene(const std::string& sceneName) {
	assert(sceneFactory_);
	//次のシーン生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}


void SceneManager::AsyncLoad()
{
	std::thread t = std::thread([&] {
		ImageManager::GetInstance()->SecondLoad2D(), ImageManager::GetInstance()->SecondLoadTex2D(), ModelManager::GetInstance()->SecondInitialize(); });

	t.join();
	// ロード状態=ロード終了
	m_loadType = LoadEnd;
}
