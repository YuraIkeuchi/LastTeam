#pragma once
#include <memory>
#include "DebugCamera.h"
#include "LightGroup.h"
#include "DirectXCommon.h"
#include "PostEffect.h"
#include "GameObject/GameObjectManager.h"
#include "ParticleManager.h"
#include "CameraWork.h"
#include "SceneChanger.h"
#include"Font.h"
#include<SceneSave.h>

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
	void BaseInitialize(DirectXCommon* dxCommon, XMFLOAT3 eye = { 2.0f, 45.0f, 2.0f }, XMFLOAT3 target = { 2.0f, 0.0f, 3.0f });

	weak_ptr<GameObjectManager>GetGameObjectManager() { return game_object_manager_; }

public:
	// �Q�[���I�u�W�F�N�g�}�l�[�W��
	shared_ptr<GameObjectManager> game_object_manager_;
	///�Q�[���V�[���p
	DebugCamera* camera = nullptr;
	//�|�X�g�G�t�F�N�g�������邩
	bool m_PlayPostEffect = false;
	//���C�g
	LightGroup* lightGroup = nullptr;


	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//�_����
	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 15.0f,15.0f,15.0f };
	float pointLightPower[3] = { 20.0f,20.0f,20.0f };
	//�X�|�b�g���C�g
	XMFLOAT3 spotLightPos[4];
	XMFLOAT3 spotLightDir[4];
	XMFLOAT3 spotLightColor[4];
	float spotLightAtten[3] = { 0.0,0.0f,0.0f };
	float spotLightFactorAngle[2] = { 20.0f,30.0f };
	//�ۉe(�v���C���[)
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 1.0f, 1.0f };

	unique_ptr<PostEffect> postEffect = nullptr;
	unique_ptr<CameraWork> camerawork;
	bool PlayPostEffect = false;
	//�Q�[�����[�v�������ǂ���
	static bool s_GameLoop;
	//�V�[���ł̑J��
	//SceneState m_SceneState = SceneState::IntroState;
};