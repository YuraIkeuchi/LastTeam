#pragma once
#include"InterEnemy.h"
#include<list>
#include<memory>
#include "LightGroup.h"
class EnemyManager {
protected:
	// DirectX::ðÈª
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	EnemyManager();
	~EnemyManager();

	void Initialize();
	void BattleUpdate();
	void Update();
	void Draw(DirectXCommon* dxCommon);

	void SetCount();

	void ImGuiDraw();
	//UIÌ`æ
	void UIDraw();

	//GÌS
	bool BossDestroy();

	void SetLight(LightGroup* light);

private:
	std::list<unique_ptr<InterEnemy>> enemys;

	//Ûe({X)
	float BosscircleShadowDir[3] = { 0,-1,0 };
	float BosscircleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float BosscircleShadowFactorAngle[2] = { 0.0f, 0.3f };
};

