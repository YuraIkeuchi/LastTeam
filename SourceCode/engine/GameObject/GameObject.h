#pragma once

#include <fstream>
#include <string>

#include "scene/SceneManager.h"
#include "Property.h"


class BaseScene;
class GameObject
{
public:
	//===========================================
	//
	//		静的関数
	//
	//===========================================

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="GameObjectClass">生成したいオブジェクトクラス</typeparam>
	/// <typeparam name="...Parameter">パラメータなど</typeparam>
	/// <returns></returns>
	template<class GameObjectClass, class... Parameter>
	static std::shared_ptr<GameObjectClass> CreateObject(Parameter ...param);

	
	//===========================================
	//
	//		メンバ関数
	//
	//===========================================

	virtual bool Initialize() { return true; }
	virtual void Update() {};
	virtual void Draw() {};
	virtual void UIDraw() {};
	virtual void Finalize() {};

	// ImGuiデバッグ用
	virtual void ImGuiDraw() {};



	/// <summary>
	/// インスタンスIDを返す
	/// </summary>
	unsigned const int &GetInstanceID() { return instance_id_; }

	/// <summary>
	/// オブジェクト名 (AllAccess)
	/// </summary>
	yEngine::Property<std::string> name{ &name_ ,yEngine::AccessorType::AllAccess };

protected:
	GameObject(const std::string &name = "Object");
	virtual ~GameObject();

private:

	// 所属しているシーン
	std::weak_ptr<BaseScene> parent_scene_;


	// インスタンスIDz
	unsigned int instance_id_;
	// 静的カウンタ
	static  int id_counter_;
	// オブジェクト名
	std::string name_;

};


template<class GameObjectClass, class ...Parameter>
inline std::shared_ptr<GameObjectClass> GameObject::CreateObject(Parameter ...param)
{
	std::shared_ptr<GameObjectClass> object = std::make_shared<GameObjectClass>(param...);
	SceneManager::GetInstance()->GetTopScene()->GetGameObjectManager().lock()->GetContainer().push_back(object);
	return object;
}
