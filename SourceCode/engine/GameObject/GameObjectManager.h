#pragma once
#include "DirectXCommon.h"
#include <memory>
#include <vector>

class GameObject;
class GameObjectManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon);
	/// <summary>
	/// UI描画
	/// </summary>
	void UIDraw();
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

	std::vector<std::shared_ptr<GameObject>> &GetContainer() { return game_objects_; }

private:
	// オブジェクトコンテナ
	std::vector<std::shared_ptr<GameObject>> game_objects_;
};
