#pragma once
#include <vector>
#include <memory>

#include "DebugCamera.h"
#include "LightGroup.h"
#include "DirectXCommon.h"
#include "PostEffect.h"
#include "BaseActor.h"
#include "GameObject/GameObjectManager.h"

using namespace std;         //  ���O��Ԏw��
//�O���錾
class SceneManager;

//�V�[���C���^�[�t�F�[�X
class BaseScene {
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;


public:
	// �R���X�g���N�^
	BaseScene();
	//���z�f�X�g���N�^
	virtual ~BaseScene() = default;

	//������
	virtual void Initialize(DirectXCommon* dxCommon) = 0;
	//�J��
	virtual void Finalize() = 0;
	//�X�V
	virtual void Update(DirectXCommon* dxCommon) = 0;
	//�`��
	virtual void Draw(DirectXCommon* dxCommon) = 0;
	//���ʏ�����
	void BaseInitialize(DirectXCommon* dxCommon);

	weak_ptr<GameObjectManager>GetGameObjectManager() { return game_object_manager_; }

public:
	// �Q�[���I�u�W�F�N�g�}�l�[�W��
	shared_ptr<GameObjectManager> game_object_manager_;
	//�A�N�^�[�N���X
	unique_ptr<BaseActor> actor;
	///�Q�[���V�[���p
	DebugCamera* camera = nullptr;
	//�|�X�g�G�t�F�N�g�������邩
	bool m_PlayPostEffect = false;
	//���C�g
	LightGroup* lightGroup = nullptr;
};