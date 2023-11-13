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

void GameObjectManager::Draw(DirectXCommon* dxCommon)
{
	// ��ŕ`�揇�ݒ�
	for (std::weak_ptr<GameObject> object : game_objects_)
	{
		object.lock()->Draw(dxCommon);
	}
}

void GameObjectManager::UIDraw()
{
	// ��ŕ`�揇�ݒ�
	for (std::weak_ptr<GameObject> object : game_objects_)
	{
		object.lock()->UIDraw();
	}
}

void GameObjectManager::Finalize()
{
	for (std::weak_ptr<GameObject> object : game_objects_)
	{
		object.lock()->Finalize();
	}
}

void GameObjectManager::ImGuiDraw()
{
	for (std::weak_ptr<GameObject> object : game_objects_)
	{
		object.lock()->ImGuiDraw();
	}
}

