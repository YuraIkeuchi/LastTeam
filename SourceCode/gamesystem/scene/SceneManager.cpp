#include "SceneManager.h"
#include "ImageManager.h"
#include "ModelManager.h"
#include "AudioManager.h"

#include<cassert>
void SceneManager::Finalize() {
	while (!scene_stack_.empty())
	{
		scene_stack_.top()->Finalize();
		scene_stack_.pop();
	}

	////最後のシーンの終了と開放
	//scene_->Finalize();
	//delete scene_;
}

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

void SceneManager::Update(DirectXCommon* dxCommon) {
	// シーン追加
	if (scene_change_type_ == SceneChangeType::kPush) {
		// シーンスタックにデータがあれば終了処理
		if (all_clear_)
		{
			while (!scene_stack_.empty())
			{
				scene_stack_.top()->Finalize();
				scene_stack_.pop();
			}
		}
		else
		{
			if (!scene_stack_.empty())
			{
				scene_stack_.top()->Finalize();
			}
		}


		// Scene追加
		scene_stack_.push(nextScene_);
		scene_stack_.top()->Initialize(dxCommon);
		// シーン遷移用処理初期化
		scene_change_type_ = SceneChangeType::kNon;
		nextScene_ = nullptr;
	}
	// シーン破棄
	else if (scene_change_type_ == SceneChangeType::kPop)
	{
		// シーン破棄
		if (all_clear_)
		{
			while (!scene_stack_.empty())
			{
				scene_stack_.top()->Finalize();
				scene_stack_.pop();
			}
		}
		else
		{
			scene_stack_.top()->Finalize();
			scene_stack_.pop();
		}

		// シーン遷移用処理初期化
		scene_change_type_ = SceneChangeType::kNon;
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
	scene_stack_.top()->Update(dxCommon);
	//scene_->Update(dxCommon);
}

void SceneManager::Draw(DirectXCommon* dxCommon) {
	scene_stack_.top()->Draw(dxCommon);
	//scene_->Draw(dxCommon);
}



void SceneManager::PopScene(bool allClear)
{
	scene_change_type_ = SceneChangeType::kPop;
	all_clear_ = allClear;
}

void SceneManager::AsyncLoad()
{
	std::thread t = std::thread([&] {
		ImageManager::GetInstance()->SecondLoad2D(), ImageManager::GetInstance()->SecondLoadTex2D(), ModelManager::GetInstance()->SecondInitialize(); });

	t.join();
	// ロード状態=ロード終了
	m_loadType = LoadEnd;
}
