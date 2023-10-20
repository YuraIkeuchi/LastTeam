#pragma once

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
	void Draw();
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();


private:
	// オブジェクトコンテナ
	std::vector<std::shared_ptr<GameObject>> game_objects_;
};
