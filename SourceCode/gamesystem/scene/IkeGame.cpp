#include "IkeGame.h"
#include "SceneFactory.h"
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
	//基底クラスの初期化
	Framework::Initialize(dxCommon);
	//マネージャーセット
	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void IkeGame::Update(DirectXCommon* dxCommon)
{
	//基底クラスの更新処理
	Framework::Update(dxCommon);
}

void IkeGame::Draw(DirectXCommon* dxCommon)
{
	Framework::Draw(dxCommon);
}

void IkeGame::Finalize()
{
	//基底クラスの解放処理
	Framework::Finalize();
}