#pragma once

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
	void Draw();
	/// <summary>
	/// �I��
	/// </summary>
	void Finalize();


private:
	// �I�u�W�F�N�g�R���e�i
	std::vector<std::shared_ptr<GameObject>> game_objects_;
};
