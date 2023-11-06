#include "EnemyManager.h"
#include "NormalEnemy.h"
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
	//���ׂĂ̓G�̍s�����I�����r
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		if (enemy->GetState() != STATE_STANDBY) { break; }
		for (unique_ptr<InterEnemy>& enemy : enemys) {
			enemy->SetState(STATE_INTER);
		}
	}

}

void EnemyManager::Update() {
	BattleUpdate();
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->Update();
	}

	for (unique_ptr<InterEnemy>& enemy : enemys) {
		if (enemy->GetState() == STATE_ATTACK) {			//��U�U���ȊO�͎��Ȃ�
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
//UI�̕`��
void EnemyManager::UIDraw() {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->UIDraw();
	}
}
//�G�̎��S���u
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
//���C�g
void EnemyManager::SetLight(LightGroup* light) {
	//�{�X
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		light->SetCircleShadowDir(1, XMVECTOR({ BosscircleShadowDir[0], BosscircleShadowDir[1], BosscircleShadowDir[2], 0 }));
		light->SetCircleShadowCasterPos(1, XMFLOAT3({ enemy->GetPosition().x, 	0.5f, 	enemy->GetPosition().z }));
		light->SetCircleShadowAtten(1, XMFLOAT3(BosscircleShadowAtten));
		light->SetCircleShadowFactorAngle(1, XMFLOAT2(BosscircleShadowFactorAngle));
	}
}