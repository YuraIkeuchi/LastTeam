#pragma once
#include"InterEnemy.h"
#include<list>
#include<memory>

class EnemyManager {
protected:
	// DirectX::‚ğÈ—ª
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
	//UI‚Ì•`‰æ
	void UIDraw();

	//“G‚Ì€–Sˆ—
	bool BossDestroy();

private:
	std::list<unique_ptr<InterEnemy>> enemys;
};

