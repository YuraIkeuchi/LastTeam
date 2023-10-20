#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <string>
#include <stack>
#include <memory>
#include <future>

//�V�[���Ǘ�
class SceneManager
{
public:

public:
	static SceneManager* GetInstance();

	//�X�V
	void Update(DirectXCommon* dxCommon);
	/// �`��
	void Draw(DirectXCommon* dxCommon);
	//�J��
	void Finalize();

	//�@���V�[���\��(class�w��)
	template<class SceneClass>
	void ChangeScene(std::shared_ptr<SceneClass> sceneClass);
	// �V�[���j���\��
	void PopScene();

	// �񓯊����[�h
	void AsyncLoad();

public:
	//getter setter
	void SetLoad(bool Load) { this->m_Load = Load; }
	bool GetLoad() { return  m_Load; }

private:
	// �V�[���X�^�b�N
	std::stack<std::shared_ptr<BaseScene>> scene_stack_;

	//���̃V�[��
	std::shared_ptr<BaseScene> nextScene_ = nullptr;


	~SceneManager() = default;
	SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	void operator =(const SceneManager&) = delete;

	// �񓯊�����
	std::thread m_th = {};
	// ���[�h���
	int m_loadType = NoLoad;
	// ���[�h���Ă��邩
	bool m_Load = false;
	// �X���b�h�ԂŎg�p���鋤�L���\�[�X��r�����䂷��
	std::mutex isLoadedMutex = {};

	//���[�h�̃^�C�v
	enum LoadType
	{
		NoLoad,
		LoadStart,
		LoadEnd
	};

	// �V�[���J�ڊǗ��p
	enum class SceneChangeType
	{
		kNon,
		kPush,
		kPop
	}scene_change_type_;
};

template<class SceneClass>
inline void SceneManager::ChangeScene(std::shared_ptr<SceneClass> sceneClass)
{
	scene_change_type_ = SceneChangeType::kPush;
	nextScene_ = sceneClass;
}
