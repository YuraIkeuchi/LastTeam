#pragma once
#include "BaseScene.h"
#include <string>
#include <stack>
#include <memory>
#include <future>

//シーン管理
class SceneManager
{
public:

public:
	static SceneManager* GetInstance();

	//更新
	void Update(DirectXCommon* dxCommon);
	/// 描画
	void Draw(DirectXCommon* dxCommon);
	//開放
	void Finalize();

	/// <summary>
	///　次シーン予約(class指定)
	/// </summary>
	/// <typeparam name="SceneClass">追加したいシーン</typeparam>
	/// <typeparam name="...Parameter">引数など</typeparam>
	template<class SceneClass,class... Parameter>
	void ChangeScene(Parameter... pram);

	// シーン破棄予約
	void PopScene();

	// 非同期ロード
	void AsyncLoad();

	/// <summary>
	/// シーントップゲッタ
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<BaseScene> GetTopScene() { return scene_stack_.top(); }

public:
	//getter setter
	void SetLoad(bool Load) { this->m_Load = Load; }
	bool GetLoad() { return  m_Load; }

private:
	// シーンスタック
	std::stack<std::shared_ptr<BaseScene>> scene_stack_;

	//次のシーン
	std::shared_ptr<BaseScene> nextScene_ = nullptr;


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

	// シーン遷移管理用
	enum class SceneChangeType
	{
		kNon,
		kPush,
		kPop
	}scene_change_type_;
};

template<class SceneClass,class... Parameter>
inline void SceneManager::ChangeScene(Parameter ...pram)
{
	scene_change_type_ = SceneChangeType::kPush;
	nextScene_ = std::make_shared<SceneClass>(pram...);
}
