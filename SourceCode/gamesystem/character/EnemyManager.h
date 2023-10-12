#pragma once
#include"InterEnemy.h"
#include<list>
#include<memory>

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
	void BattleUpdate();
	void Update();
	void Draw(DirectXCommon* dxCommon);

	void SetCount();

	void ImGuiDraw();
	//UIの描画
	void UIDraw();

	//敵の死亡処理
	bool BossDestroy();

	//カメラのデータをゲットする
	void GetCameraData(const XMMATRIX& matView, const XMMATRIX& matProjection, const XMMATRIX& matPort);
private:
	std::list<unique_ptr<InterEnemy>> enemys;
};

