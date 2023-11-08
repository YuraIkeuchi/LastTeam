#include "EnemyManager.h"
#include "NormalEnemy.h"
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
	//‚·‚×‚Ä‚Ì“G‚Ìs“®‚ªI‚í‚Á‚½r
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
		if (enemy->GetState() == STATE_ATTACK) {			//ˆê’UUŒ‚ˆÈŠO‚Íæ‚ç‚È‚¢
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
		} else {
			return false;
		}
	}
	return false;
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
			} else {
				unique_ptr<InterEnemy> enemy_ = std::make_unique<NormalEnemy>();
				enemy_->SetPosition(enemy_->SetPannelPos(4 + width, 3 - height));
				enemys.push_back(std::move(enemy_));
				width++;
			}
		}
		height++;

	}
}
//ƒ‰ƒCƒg
void EnemyManager::SetLight(LightGroup* light) {
	//ƒ{ƒX
	for (unique_ptr<InterEnemy>& enemy : enemys) {
		light->SetCircleShadowDir(1, XMVECTOR({ BosscircleShadowDir[0], BosscircleShadowDir[1], BosscircleShadowDir[2], 0 }));
		light->SetCircleShadowCasterPos(1, XMFLOAT3({ enemy->GetPosition().x, 	0.5f, 	enemy->GetPosition().z }));
		light->SetCircleShadowAtten(1, XMFLOAT3(BosscircleShadowAtten));
		light->SetCircleShadowFactorAngle(1, XMFLOAT2(BosscircleShadowFactorAngle));
	}
}