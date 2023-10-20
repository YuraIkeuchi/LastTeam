#pragma once

#include <fstream>
#include <string>

#include "scene/SceneManager.h"
#include "scene/BaseScene.h"
#include "Property.h"


class GameObject
{
public:
	//===========================================
	//
	//		�ÓI�֐�
	//
	//===========================================

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="GameObjectClass">�����������I�u�W�F�N�g�N���X</typeparam>
	/// <typeparam name="...Parameter">�p�����[�^�Ȃ�</typeparam>
	/// <returns></returns>
	template<class GameObjectClass, class... Parameter>
	static std::shared_ptr<GameObjectClass> CreateObject(Parameter ...param);

	
	//===========================================
	//
	//		�����o�֐�
	//
	//===========================================

	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Draw() {};
	virtual void Finalize() {};

	// ImGui�f�o�b�O�p
	virtual void DrawData() {};



	/// <summary>
	/// �C���X�^���XID��Ԃ�
	/// </summary>
	unsigned const int &GetInstanceID() { return instance_id_; }

	/// <summary>
	/// �I�u�W�F�N�g�� (AllAccess)
	/// </summary>
	yEngine::Property<std::string> name{ &name_ ,yEngine::AccessorType::AllAccess };

protected:
	GameObject(const std::string &name = "Object");
	virtual ~GameObject();

private:

	// �������Ă���V�[��
	std::weak_ptr<BaseScene> parent_scene_;


	// �C���X�^���XIDz
	unsigned int instance_id_;
	// �ÓI�J�E���^
	static  int id_counter_;
	// �I�u�W�F�N�g��
	std::string name_;

};


template<class GameObjectClass, class ...Parameter>
inline std::shared_ptr<GameObjectClass> GameObject::CreateObject(Parameter ...param)
{
	return SceneManager::GetInstance()->GetTopScene()->GetGameObjectManager()->
		push_back(std::make_shared<GameObjectClass>(param))
}
