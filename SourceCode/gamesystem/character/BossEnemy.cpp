#include "BossEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
#include <StagePanel.h>
//ƒ‚ƒfƒ‹“Ç‚İ‚İ
BossEnemy::BossEnemy() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	m_Object->SetLightEffect(false);
	//HPII
	hptex = IKESprite::Create(ImageManager::ENEMYHPUI, { 0.0f,0.0f });

	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i] = make_unique<DrawNumber>();
		_drawnumber[i]->Initialize();
	}

	shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });
}
//‰Šú‰»
bool BossEnemy::Initialize() {
	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 0.0f,1.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5f };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/BossEnemy.csv", "hp")));
	m_MaxHP = m_HP;
	m_CheckPanel = true;
	m_ShadowScale = { 0.05f,0.05f,0.05f };
	CreateSkill(1);
	return true;
}
//ó‘Ô‘JˆÚ
void (BossEnemy::* BossEnemy::stateTable[])() = {
	&BossEnemy::Inter,//“®‚«‚Ì‡ŠÔ
	&BossEnemy::Attack,//“®‚«‚Ì‡ŠÔ
	&BossEnemy::Teleport,//uŠÔˆÚ“®
};
//UŒ‚‘JˆÚ
void (BossEnemy::* BossEnemy::attackTable[])() = {
	&BossEnemy::BulletAttack,//’e‚ğ‘Å‚ÂUŒ‚
	&BossEnemy::RowAttack,//—ñUŒ‚
};

//s“®
void BossEnemy::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	//“–‚½‚è”»’è
	vector<AttackArea*> _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//“–‚½‚è”»’è
	PoisonState();//“Å

	//“G‚Ì’e
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Update();
		}
	}

	//áŠQ•¨‚Ìíœ
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i] == nullptr) {
			continue;
		}

		if (!bullets[i]->GetAlive()) {
			bullets.erase(cbegin(bullets) + i);
		}
	}

	//UŒ‚ƒGƒŠƒA‚ÌXV(ÀÛ‚ÍƒXƒLƒ‹‚É‚È‚é‚Æv‚¤)
	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->Update();

		if (!attackarea[i]->GetAlive()) {
			attackarea.erase(cbegin(attackarea) + i);
		}
	}

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	shadow_tex->SetPosition(m_ShadowPos);
	shadow_tex->SetScale(m_ShadowScale);
	shadow_tex->Update();
}

//•`‰æ
void BossEnemy::Draw(DirectXCommon* dxCommon) {
	if (!m_Alive) { return; }
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	shadow_tex->Draw();
	IKETexture::PostDraw();
	//“G‚Ì’e
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->Draw(dxCommon);
		}
	}
	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->Draw(dxCommon);
	}
	Obj_Draw();
}
//ImGui•`‰æ
void BossEnemy::ImGui_Origin() {
	//“G‚Ì’e
	for (unique_ptr<EnemyBullet>& newbullet : bullets) {
		if (newbullet != nullptr) {
			newbullet->ImGuiDraw();
		}
	}
	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->ImGuiDraw();
	}
	ImGui::Begin("Area");
	//for (int i = 0; i < m_Area.size(); i++) {
	//	for (int j = 0; j < m_Area.size(); j++) {
	//		ImGui::Text("Area[%d][%d],%d", i, j, m_Area[i][j]);
	//	}
	//}
	ImGui::Text("AttackCount:%d", m_AttackCount);
	ImGui::Text("cool:%d", coolTimer);
	ImGui::End();
}
//ŠJ•ú
void BossEnemy::Finalize() {

}
//‘Ò‹@
void BossEnemy::Inter() {
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, kIntervalMax);
	if (coolTimer == kIntervalMax) {
		coolTimer = 0;
		_charaState = STATE_ATTACK;
	}
}
//UŒ‚
void BossEnemy::Attack() {
	(this->*attackTable[_AttackState])();
	PlayerCollide();
}

//ƒ[ƒv
void BossEnemy::Teleport() {
	const int l_TargetTimer = 200;
	XMFLOAT3 l_RandPos = {};
	l_RandPos = StagePanel::GetInstance()->EnemySetPanel();
	if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
		//m_Position = randPanelPos();
		_charaState = STATE_INTER;
		coolTimer = {};
		m_Position = l_RandPos;
		StagePanel::GetInstance()->EnemyHitReset();
	}
}
//’e‚Ì¶¬
void BossEnemy::BirthBullet() {
	//áŠQ•¨‚Ì”­¶
	EnemyBullet* newbullet;
	newbullet = new EnemyBullet();
	newbullet->Initialize();

	newbullet->SetPolterType(TYPE_FOLLOW);
	newbullet->SetPosition({ m_Position.x,m_Position.y + 1.0f,m_Position.z });
	bullets.emplace_back(newbullet);
}
//UŒ‚‘JˆÚ
//’e
void BossEnemy::BulletAttack() {
	const int l_TargetTimer = 150;

	if (_BossType == Boss_SET) {
		if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
			coolTimer = {};
			_BossType = Boss_THROW;
		}
	}
	else if (_BossType == Boss_THROW) {
		m_AttackCount++;
		BirthBullet();
		if (m_AttackCount != 5) {
			_BossType = Boss_SET;
		}
		else {
			_BossType = Boss_END;
		}
	}
	else {
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
		coolTimer = {};
		_BossType = Boss_SET;
		StagePanel::GetInstance()->EnemyHitReset();
	}
}
void BossEnemy::RowAttack() {
	const int l_TargetTimer = 120;
	if (m_AttackCount != 4) {
		if (Helper::GetInstance()->CheckMin(coolTimer, l_TargetTimer, 1)) {
			BirthArea(m_AttackCount);
			coolTimer = {};
			m_AttackCount++;
		}
	}
	else {
		StagePanel::GetInstance()->EnemyHitReset();
		m_CheckPanel = true;
		m_AttackCount = {};
		_charaState = STATE_SPECIAL;
	}
}
//UŒ‚ƒGƒŠƒA
void BossEnemy::BirthArea(const int Height) {
	int AreaX = {};
	int AreaY = {};
	for (auto i = 0; i < m_Area.size(); i++) {
		if (m_Area[i][Height] == 1) {		//ƒ}ƒbƒvƒ`ƒbƒv”Ô†‚Æƒ^ƒCƒ‹‚ÌÅ‘å”AÅ¬”‚É‰‚¶‚Ä•`‰æ‚·‚é
			AttackArea* newarea = nullptr;
			newarea = new AttackArea();
			newarea->Initialize();
			newarea->InitState(i, Height);
			newarea->SetDamage(20.0f);
			newarea->SetName("Enemy");
			attackarea.emplace_back(newarea);
		}
	}
}
//—\‘ªƒGƒŠƒA
void BossEnemy::BirthPredict(const int Height) {

}
//ƒXƒLƒ‹‚ÌCSV‚ğ“Ç‚İæ‚é
void BossEnemy::LoadCsvSkill(std::string& FileName, const int id) {

	std::ifstream file;
	std::stringstream popcom;

	file.open(FileName);
	popcom << file.rdbuf();
	file.close();

	std::string line;
	//ƒAƒ^ƒbƒNƒGƒŠƒA—p
	std::vector<std::vector<int> > MyVector;

	while (std::getline(popcom, line)) {
		std::istringstream line_stream(line);
		std::string word;
		std::getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		}

		if (word.find("AttackArea") == 0) {
			while (std::getline(line_stream, word)) {
				std::vector<int> row;

				for (char& x : word) {
					int X = x - '0';
					if (x != ' ')
						row.push_back(X);
				}
				MyVector.push_back(row);
			}
		}
		else if (word.find("AttackAreA") == 0) {
			while (std::getline(line_stream, word)) {
				std::vector<int> row;

				for (char& x : word) {
					int X = x - '0';
					if (x != ' ')
						row.push_back(X);
				}
				MyVector.push_back(row);
			}

			m_Area = MyVector;
			break;
		}
	}

}

bool BossEnemy::CreateSkill(int id) {

	std::string directory = "Resources/csv/chara/enemy/AttackArea/Boss1/AttackArea";

	std::stringstream ss;
	if (id >= 10) {
		ss << directory << id << ".csv";

	}
	else {
		ss << directory << "0" << id << ".csv";
	}
	std::string csv_ = ss.str();

	LoadCsvSkill(csv_, id);

	//Player::GetInstance()->SetDelayTimer(m_Delay);
	return true;
}
//ƒGƒŠƒAUŒ‚‚Ì”»’è
void BossEnemy::PlayerCollide() {
	auto player_data = GameStateManager::GetInstance()->GetPlayer().lock();
	int l_PlayerWidth = player_data->GetNowWidth();
	int l_PlayerHeight = player_data->GetNowHeight();
	for (unique_ptr<AttackArea>& newarea : attackarea) {
		if (newarea != nullptr) {
			if ((newarea->GetNowHeight() == l_PlayerHeight && newarea->GetNowWidth() == l_PlayerWidth) &&
				!newarea->GetHit() && (newarea->GetName() == "Enemy")) {
				player_data->RecvDamage(20.0f);
				newarea->SetHit(true);
				break;
			}
		}
	}
}