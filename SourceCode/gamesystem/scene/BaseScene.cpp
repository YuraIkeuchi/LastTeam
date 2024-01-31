#include "BaseScene.h"
#include "ImageManager.h"
#include "IKEObject3d.h"
#include "IKEFBXObject3d.h"
#include "ParticleEmitter.h"
#include "Helper.h"
#include <Player.h>
#include <StagePanel.h>
#include "GameStateManager.h"
bool BaseScene::s_GameLoop = false;
bool BaseScene::s_LastStage = false;
bool BaseScene::s_Countinue = false;
bool BaseScene::s_PlayBGM = true;
int BaseScene::nowHierarchy = 0;
int BaseScene::nowIndex = 1;
int BaseScene::clearHierarchy=1;

BaseScene::BaseScene()
{
	//game_object_manager_ = std::make_shared<GameObjectManager>();
}

//���ʂ̏�����
void BaseScene::BaseInitialize(DirectXCommon* dxCommon, XMFLOAT3 eye, XMFLOAT3 target) {
	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	IKETexture::SetCamera(camera);
	// 3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	IKEObject3d::SetCamera(camera);
	IKEFBXObject3d::SetCamera(camera);
	ParticleManager::SetCamera(camera);
	// ���C�g����
	lightGroup = LightGroup::Create();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	IKEObject3d::SetLightGroup(lightGroup);

	ParticleEmitter::GetInstance()->AllDelete();
	//�ۉe�̂��߂̂��
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	//�|�X�g�G�t�F�N�g�̏�����
	//(����)
	postEffect = make_unique<PostEffect>();
	postEffect->Initialize();
	
	camerawork = make_unique<CameraWork>(eye, target);
	Helper::SetCamera(camera);
}
