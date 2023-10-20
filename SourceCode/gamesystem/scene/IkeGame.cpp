#include "IkeGame.h"
#include "SceneFactory.h"

#include "TitleScene.h"

using namespace DirectX;

IkeGame::IkeGame()
{
}

IkeGame::~IkeGame()
{
	//safe_delete(spriteBG);
	//safe_delete(objSkydome);
	//safe_delete(objGround);
	//safe_delete(objPlayer);
	//safe_delete(modelSkydome);
	//safe_delete(modelGround);
	//safe_delete(modelPlayer);
}

void IkeGame::Initialize(DirectXCommon* dxCommon)
{
	//���N���X�̏�����
	Framework::Initialize(dxCommon);
	//�}�l�[�W���[�Z�b�g
	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->ChangeScene(std::make_shared<TitleScene>());
}

void IkeGame::Update(DirectXCommon* dxCommon)
{
	//���N���X�̍X�V����
	Framework::Update(dxCommon);
}

void IkeGame::Draw(DirectXCommon* dxCommon)
{
	Framework::Draw(dxCommon);
}

void IkeGame::Finalize()
{
	//���N���X�̉������
	Framework::Finalize();
}