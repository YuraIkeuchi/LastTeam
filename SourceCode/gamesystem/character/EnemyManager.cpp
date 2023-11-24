#include "EnemyManager.h"
#include "NormalEnemy.h"
#include "CanonEnemy.h"
#include "TackleEnemy.h"
#include "CreateBombEnemy.h"
#include "Bomb.h"
#include "BossEnemy.h"
#include "PoisonEnemy.h"
#include "TackleEnemy.h"
#include <StagePanel.h>
#include <GameStateManager.h>
#include <Helper.h>
Player* EnemyManager::player = nullptr;
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


void EnemyManager::Update() {
	for (unique_ptr<InterEnemy>& enemy : enemys) {

		if (enemy->GetState() == STATE_ATTACK) {			//��U�U���ȊO�͎��Ȃ�
			player->SetGrazePos(enemy->GetPosition());
		} else {
			player->SetGrazePos({ 1000.0f,0.0f,0.0f });
		}
		enemy->Update();
	}

	//敵の削除
	for (int i = 0; i < enemys.size(); i++) {
		if (enemys[i] == nullptr) {
			continue;
		}
		if (!enemys[i]->GetAlive()) {
			enemys.erase(cbegin(enemys) + i);
		}
	}

	if (enemys.size() == 1) {
		enemys[0]->SetLastEnemy(true);
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
	ImGui::Text("size:%d", m_DeathCount);
	ImGui::End();
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->ImGuiDraw();
	}
}
//UI描画
void EnemyManager::UIDraw() {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->UIDraw();
	}
}
//ステージのエネミー残存
bool EnemyManager::BossDestroy() {
	int num = (int)enemys.size();

	for (unique_ptr<InterEnemy>& enemy : enemys) {
		if (enemy->GetAlive()) {
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

void EnemyManager::DrainHealUp() {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->SetDrainUp(true);
	}
}

void EnemyManager::ReLoadDamage() {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->SimpleDamege(3.f);
	}
}

void EnemyManager::BombDamage() {
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		enemy->SimpleDamege(5.f);
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
		//数字による各エネミーの場合分け
		for (char& x : word) {
			if (x == ',') {
				break;
			}
			InterEnemy::SetPlayer(player);
			if (x == '0') {
				width++;
			} else if (x == '1') {
				unique_ptr<InterEnemy> enemy_ = std::make_unique<TackleEnemy>();
				//enemy_->SetPlayer(player);
				enemy_->SetPosition(enemy_->SetPannelPos(4 + width, 3 - height));
				enemys.push_back(std::move(enemy_));
				width++;
			}
			else if (x == '2') {
				unique_ptr<InterEnemy> enemy_ = std::make_unique<CanonEnemy>();
				//enemy_->SetPlayer(player);
				enemy_->SetPosition(enemy_->SetPannelPos(4 + width, 3 - height));
				enemys.push_back(std::move(enemy_));
				width++;
			}
			else if (x == '3') {
				unique_ptr<InterEnemy> enemy_ = std::make_unique<CreateBombEnemy>();
				//enemy_->SetPlayer(player);
				enemy_->SetPosition(enemy_->SetPannelPos(4 + width, 3 - height));
				enemys.push_back(std::move(enemy_));
				width++;
			}
			else if (x == '4') {
				unique_ptr<InterEnemy> enemy_ = std::make_unique<PoisonEnemy>();
				//enemy_->SetPlayer(player);
				enemy_->SetPosition(enemy_->SetPannelPos(4 + width, 3 - height));
				enemys.push_back(std::move(enemy_));
				width++;
			}
			else if (x == '5') {
				unique_ptr<InterEnemy> enemy_ = std::make_unique<BossEnemy>();
				//enemy_->SetPlayer(player);
				enemy_->SetPosition(enemy_->SetPannelPos(4 + width, 3 - height));
				enemys.push_back(std::move(enemy_));
				width++;
			}
		}
		height++;

	}
}
