﻿#pragma once
#include <string>
#include "IKEModel.h"
#include <map>
#include <IKEFBXModel.h>

class ModelManager final {
private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager& r) = default;
	ModelManager& operator= (const ModelManager& r) = default;

public:
	//OBJの名前
	enum ModelName {
		PLAYERMODEL,
		PANEL,
		BULLET,
		THORN,
		CANNON,
		SQUID,
		MISSILE,
		BOMBER,
		THROW,
		BOOMERAN,
		FRONT,
		SICKLE,
		HEAL,
		ROCK,
		DOGO,
		DORO,
		ZASHU,
		GUSA,
		HATENA,
		KIRA,
		BYUNN,
		FIRST_BOSS,
		TORNADO,
		ROCKENEMY,
		SECOND_BOSS,
		LAST_BOSS,
		SUPPORT_ENEMY,
		SUPPORT_ENEMY2,
		BOSS_ARM,
		BOM,
	};
	//FBXの名前
	enum FBXName {
	
	};

public:
	static ModelManager* GetInstance();		//取得用
	void StartInitialize();
	void SecondInitialize();//初期化
	void Finalize();//解放
	//読み込み
	void LoadModel(const ModelName modelName, std::string fileName);
	void LoadFBXModel(const FBXName modelName, std::string fileName);
	IKEModel* GetModel(ModelName modelName) { return model[modelName]; }
	IKEFBXModel* GetFBXModel(FBXName FBXName) { return fbxModel[FBXName]; }
private:
	static std::map<ModelName, IKEModel*> model;	//モデル格納マップ
	static std::map<FBXName, IKEFBXModel*> fbxModel;
};