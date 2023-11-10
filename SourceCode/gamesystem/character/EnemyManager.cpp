#include "EnemyManager.h"
#include "NormalEnemy.h"
#include "CanonEnemy.h"
#include <StagePanel.h>
#include <Player.h>
#include <GameStateManager.h>


EnemyManager::EnemyManager() {
}

EnemyManager::~EnemyManager() {
}

void EnemyManager::Initialize() {
	Spawn2Map();
	//unique_ptr<InterEnemy> enemy_ = std::make_unique<NormalEnemy>();
	//enemy_->SetPannelPos(4, 0);
	//enemys.push_back(std::move(enemy_));

	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->Initialize();
	}
}

void EnemyManager::BattleUpdate() {
	//すべての敵の行動が終わったr
	//for (unique_ptr<InterEnemy>& enemy : enemys) {
	//	if (enemy->GetState() != STATE_STANDBY) { break; }
	//	for (unique_ptr<InterEnemy>& enemy : enemys) {
	//		enemy->SetState(STATE_INTER);
	//	}
	//}

}

void EnemyManager::Update() {
	BattleUpdate();
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->Update();
	}

	for (unique_ptr<InterEnemy>& enemy : enemys) {
		if (enemy->GetState() == STATE_ATTACK) {			//一旦攻撃以外は取らない
			Player::GetInstance()->SetGrazePos(enemy->GetPosition());
		} else {
			Player::GetInstance()->SetGrazePos({ 1000.0f,0.0f,0.0f });
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
	ImGui::Begin("Enemys");
	ImGui::Text("size:%d", enemys.size());
	//unique_ptr<InterEnemy>& enemy = enemys.front();
	//ImGui::Text("POSX:%f", enemy.get()->GetPosition().x);
	//ImGui::Text("POSZ:%f", enemy.get()->GetPosition().z);
	ImGui::End();
	//for (unique_ptr<InterEnemy>& enemy : enemys) {
	//	enemy->ImGuiDraw();
	//}
}
//UIの描画
void EnemyManager::UIDraw() {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->UIDraw();
	}
}
//敵の死亡処置
bool EnemyManager::BossDestroy() {
	int num = (int)enemys.size();
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		if (enemy->GetHP() > 0.0f) {
			return false;
		} else {
			num--;
		}
	}
	if (num == 0) {
		return true;
	} else {
		return false;
	}
}

void EnemyManager::PoizonGauge() {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->SetPoizonLong(true);
	}
}

void EnemyManager::PoizonVenom() {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->SetPoizonVenom(true);
	}

}

void EnemyManager::Spawn2Map() {
	string csv_ = GameStateManager::GetInstance()->GetEnemySpawnText();
	std::string line;
	std::stringstream popcom;
	std::ifstream file;

	file.open(csv_);
	popcom << file.rdbuf();
	file.close();
	int height = 0;
	while (std::getline(popcom, line)) {
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');
		int width = 0;
		for (char& x : word) {
			if (x == ',') {
				break;
			}
			if (x == '0') {
				width++;
			} else if (x == '1') {
				unique_ptr<InterEnemy> enemy_ = std::make_unique<NormalEnemy>();
				enemy_->SetPosition(enemy_->SetPannelPos(4 + width, 3 - height));
				enemys.push_back(std::move(enemy_));
				width++;
			}
		}
		height++;

	}
}
//ライト
void EnemyManager::SetLight(LightGroup* light) {
	//ボス
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		light->SetCircleShadowDir(1, XMVECTOR({ BosscircleShadowDir[0], BosscircleShadowDir[1], BosscircleShadowDir[2], 0 }));
		light->SetCircleShadowCasterPos(1, XMFLOAT3({ enemy->GetPosition().x, 	0.5f, 	enemy->GetPosition().z }));
		light->SetCircleShadowAtten(1, XMFLOAT3(BosscircleShadowAtten));
		light->SetCircleShadowFactorAngle(1, XMFLOAT2(BosscircleShadowFactorAngle));
	}
}