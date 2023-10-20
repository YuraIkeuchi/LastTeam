#include "GameObjectManager.h"
#include "GameObject.h"

void GameObjectManager::Initialize()
{
	for (std::weak_ptr<GameObject> object : game_objects_)
	{
		object.lock()->Initialize();
	}
}

void GameObjectManager::Update()
{

	for (std::weak_ptr<GameObject> object : game_objects_)
	{
		object.lock()->Update();
	}
}

void GameObjectManager::Draw()
{
	// ��ŕ`�揇�ݒ�
	for (std::weak_ptr<GameObject> object : game_objects_)
	{
		object.lock()->Draw();
	}
}

void GameObjectManager::Finalize()
{
	for (std::weak_ptr<GameObject> object : game_objects_)
	{
		object.lock()->Finalize();
	}
}