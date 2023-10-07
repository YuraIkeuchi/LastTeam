#pragma once
#include"InterEnemy.h"


class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void Initialize();
	void Update();
	void Draw();

	void SetCount();
private:
	//unique_ptr<>
};

