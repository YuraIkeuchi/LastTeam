#include"ModelManager.h"
#include "IKEFbxLoader.h"

std::map<ModelManager::ModelName, IKEModel*> ModelManager::model;
std::map<ModelManager::FBXName, IKEFBXModel*> ModelManager::fbxModel;

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instans;
	return &instans;
}

//モデル読み込み(最初から読み込むもの)
void ModelManager::StartInitialize()
{
	LoadModel(PLAYERMODEL, "pawn");
	LoadModel(PANEL, "box1x1x1");
	LoadModel(BULLET, "EnemyBullet");
	LoadModel(THORN, "Ice");
	LoadModel(CANNON, "cannon");
	LoadModel(SQUID, "squid");
	LoadModel(MISSILE, "missile");
	LoadModel(BOMBER, "bomber");
	LoadModel(THROW, "ThrowEnemy");
	LoadModel(BOOMERAN, "Boomeran");
	LoadModel(FRONT, "FrontEnemy");
	LoadModel(SICKLE, "Sickle");
	LoadModel(HEAL, "Heal");
	LoadModel(ROCK, "Rock");
	LoadModel(DOGO, "dogoxo");
	LoadModel(DORO, "doroxo");
	LoadModel(ZASHU, "zashu");
	LoadModel(GUSA, "gusa");
	LoadModel(HATENA, "Hatena");
	LoadModel(KIRA, "kira");
	LoadModel(FIRST_BOSS, "Golem");
	LoadModel(TORNADO, "Tornado");
	LoadModel(ROCKENEMY, "RockEnemy");
	LoadModel(SECOND_BOSS, "stick");
	LoadModel(LAST_BOSS, "LastBoss");
	LoadModel(SUPPORT_ENEMY, "Shield");
	LoadModel(SUPPORT_ENEMY2, "BomBear");
	LoadModel(BOSS_ARM, "punch");
}

//モデル読み込み(非同期処理で読み込むもの)
void ModelManager::SecondInitialize()
{
}

void ModelManager::Finalize()
{
	model.clear();
	fbxModel.clear();
}

void ModelManager::LoadModel(const ModelName modelName, std::string fileName)
{
	model[modelName] = new IKEModel();
	model[modelName] = IKEModel::LoadFromOBJ(fileName, true);
}

void ModelManager::LoadFBXModel(const FBXName modelName, std::string fileName)
{
	fbxModel[modelName] = new IKEFBXModel();
	fbxModel[modelName] = IKEFbxLoader::GetInstance()->LoadModelFromFile(fileName);
}
