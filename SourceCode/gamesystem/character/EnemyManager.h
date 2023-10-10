#pragma once
#include"InterEnemy.h"
#include<list>
#include<memory>

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void Initialize();
	void BattleUpdate();
	void Update();
	void Draw(DirectXCommon* dxCommon);

	void SetCount();

	void ImGuiDraw();

	//�G�̎��S����
	bool BossDestroy();
private:
	std::list<unique_ptr<InterEnemy>> enemys;
};

