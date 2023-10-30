#pragma once
#include "BaseScene.h"
#include <string>
#include <stack>
#include <memory>
#include <future>

//�V�[���Ǘ�
class SceneManager
{

public:
	static SceneManager* GetInstance();

	//�X�V
	void Update(DirectXCommon* dxCommon);
	/// �`��
	void Draw(DirectXCommon* dxCommon);
	//�J��
	void Finalize();

	/// <summary>
	///�@���V�[���\��(class�w��)
	/// </summary>
	/// <typeparam name="SceneClass">�ǉ��������V�[��</typeparam>
	/// <typeparam name="...Parameter">�����Ȃ�</typeparam>
	template<class SceneClass,class... Parameter>
	void ChangeScene(bool allClear = false,Parameter... pram);

	// �V�[���j���\��
	void PopScene(bool allClear = false);

	// �񓯊����[�h
	void AsyncLoad();

	/// <summary>
	/// �V�[���g�b�v�Q�b�^
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<BaseScene> GetTopScene() { return scene_stack_.top(); }

public:
	//getter setter
	void SetLoad(bool Load) { this->m_Load = Load; }
	bool GetLoad() { return  m_Load; }

private:
	// �V�[���X�^�b�N
	std::stack<std::shared_ptr<BaseScene>> scene_stack_;

	//���̃V�[��
	std::shared_ptr<BaseScene> nextScene_ = nullptr;

	// �S�V�[���j���t���O
	bool all_clear_ = false;


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

template<class SceneClass,class... Parameter>
inline void SceneManager::ChangeScene(bool allClear, Parameter ...pram)
{
	scene_change_type_ = SceneChangeType::kPush;
	nextScene_ = std::make_shared<SceneClass>(pram...);
	all_clear_ = allClear;
}
