#pragma once
#include "DirectXCommon.h"
#include <memory>
#include <vector>

class GameObject;
class GameObjectManager
{
public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw(DirectXCommon* dxCommon);
	/// <summary>
	/// UI�`��
	/// </summary>
	void UIDraw();
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui�`��
	/// </summary>
	void ImGuiDraw();

	std::vector<std::shared_ptr<GameObject>> &GetContainer() { return game_objects_; }

private:
	// �I�u�W�F�N�g�R���e�i
	std::vector<std::shared_ptr<GameObject>> game_objects_;
};
