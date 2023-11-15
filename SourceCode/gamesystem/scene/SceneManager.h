#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <string>
#include <memory>
#include <future>
//シーン管理
class SceneManager
{
public:
	static SceneManager* GetInstance();
	//更新
	void Update(DirectXCommon* dxCommon);
	/// 描画
	void Draw(DirectXCommon* dxCommon);
	//開放
	void Finalize();

public:
	//次シーン予約
	void ChangeScene(const std::string& sceneName);

	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

	// 非同期ロード
	void AsyncLoad();

public:
	//getter setter
	void SetLoad(bool Load) { this->m_Load = Load; }
	bool GetLoad() { return  m_Load; }

private:
	//今のシーン
	BaseScene* scene_ = nullptr;
	//次のシーン
	BaseScene* nextScene_ = nullptr;

	AbstractSceneFactory* sceneFactory_ = nullptr;

	~SceneManager() = default;
	SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	void operator =(const SceneManager&) = delete;

	// 非同期処理
	std::thread m_th = {};
	// ロード状態
	int m_loadType = NoLoad;
	// ロードしているか
	bool m_Load = false;
	// スレッド間で使用する共有リソースを排他制御する
	std::mutex isLoadedMutex = {};
	//ロードのタイプ
	enum LoadType
	{
		NoLoad,
		LoadStart,
		LoadEnd
	};
};