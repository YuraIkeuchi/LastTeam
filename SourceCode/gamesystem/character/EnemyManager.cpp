#include "EnemyManager.h"
#include "NormalEnemy.h"
#include <GameMode.h>
#include <StagePanel.h>
#include <Player.h>


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
	//バトルから2秒後にパネルと敵の行動開始
	if (GameMode::GetInstance()->BattleStart()) {
		for (unique_ptr<InterEnemy>& enemy : enemys) {
			enemy->SetState(STATE_ATTACK);
		}
	}
	//すべての敵の行動が終わったr
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		if (enemy->GetState() != STATE_STANDBY) { break; }
		for (unique_ptr<InterEnemy>& enemy : enemys) {
			enemy->SetState(STATE_INTER);
			Player::GetInstance()->AttackTarget(enemy->GetPosition());
		}
		GameMode::GetInstance()->SetGameTurn(TURN_ATTACK);
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

void EnemyManager::ImGuiDraw() {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->ImGuiDraw();
	}
}
//UIの描画
void EnemyManager::UIDraw() {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->UIDraw();
	}
}
//敵の死亡処置
bool EnemyManager::BossDestroy() {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		if (enemy->GetHP() <= 0.0f) {
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

void EnemyManager::GetCameraData(const XMMATRIX& matView, const XMMATRIX& matProjection, const XMMATRIX& matPort) {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->GetData(matView, matProjection, matPort);
	}
}