#include "EnemyManager.h"
#include "NormalEnemy.h"
#include <GameMode.h>
#include <StagePanel.h>


EnemyManager::EnemyManager() {
}

EnemyManager::~EnemyManager() {
}

void EnemyManager::Initialize() {

	unique_ptr<InterEnemy> enemy_ = std::make_unique<NormalEnemy>();
	enemys.push_back(std::move(enemy_));

	for (unique_ptr<InterEnemy> &enemy :enemys) {
		enemy->Initialize();
	}
}

void EnemyManager::BattleUpdate() {
	if (GameMode::GetInstance()->BattleStart()) {
		StagePanel::GetInstance()->RandomPanel(3);
		for (unique_ptr<InterEnemy>& enemy : enemys) {
			enemy->SetState(1);
		}
	}

}

void EnemyManager::Update() {
	BattleUpdate();


	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->Update();
	}
}

void EnemyManager::Draw(DirectXCommon* dxCommon) {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->Draw(dxCommon);
	}
}

void EnemyManager::SetCount() {
}
