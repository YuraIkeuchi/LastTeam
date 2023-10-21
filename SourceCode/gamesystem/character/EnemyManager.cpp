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
	//ƒoƒgƒ‹‚©‚ç2•bŒã‚Éƒpƒlƒ‹‚Æ“G‚Ìs“®ŠJn
	if (GameMode::GetInstance()->BattleStart()) {
		for (unique_ptr<InterEnemy>& enemy : enemys) {
			enemy->SetState(STATE_ATTACK);
		}
	}
	//‚·‚×‚Ä‚Ì“G‚Ìs“®‚ªI‚í‚Á‚½r
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

	for (unique_ptr<InterEnemy>& enemy : enemys) {
		if (enemy->GetState() == STATE_ATTACK) {			//ˆê’UUŒ‚ˆÈŠO‚Íæ‚ç‚È‚¢
			Player::GetInstance()->SetGrazePos(enemy->GetPosition());
		}
		else {
			Player::GetInstance()->SetGrazePos({1000.0f,0.0f,0.0f});
		}
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
//UI‚Ì•`‰æ
void EnemyManager::UIDraw() {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->UIDraw();
	}
}
//“G‚Ì€–Sˆ’u
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