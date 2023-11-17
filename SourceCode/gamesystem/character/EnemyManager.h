#pragma once
#include"InterEnemy.h"
#include<list>
#include<memory>
#include "CsvLoader.h"
#include "Player.h"
#include "LightGroup.h"
class EnemyManager {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	EnemyManager();
	~EnemyManager();

	void Initialize();
	void Update();
	void Draw(DirectXCommon* dxCommon);

	void SetCount();

	void ImGuiDraw();
	//UIの描画
	void UIDraw();

	//敵の死亡処理
	bool BossDestroy();
	/// <summary>
	/// パッシブがあれば毒延長(バトルシーン)
	/// </summary>
	void PoizonGauge();
	/// <summary>
	/// パッシブがあれば猛毒(バトルシーン)
	/// </summary>
	void PoizonVenom();
	/// <summary>
	/// パッシブがあればドレイン上昇(バトルシーン)
	/// </summary>
	void DrainHealUp();
	/// <summary>
	/// 
	/// </summary>
	void ReLoadDamage();
private:
	/// <summary>
	/// csvでマップに敵をスポーン
	/// </summary>
	void Spawn2Map();
public:
	static void SetPlayer(Player* player) { EnemyManager::player = player; }

private:
	static Player* player;
	std::vector<unique_ptr<InterEnemy>> enemys;
	//丸影(ボス)
	float BosscircleShadowDir[3] = { 0,-1,0 };
	float BosscircleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float BosscircleShadowFactorAngle[2] = { 0.0f, 0.3f };
};

