#include "NormalEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
//モデル読み込み
NormalEnemy::NormalEnemy() {
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
}
//初期化
bool NormalEnemy::Initialize() {

	m_Position = randPanelPos();
	m_Position.y = 0.1f;
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "hp")));
	m_MaxHP = m_HP;
	return true;
}

void (NormalEnemy::* NormalEnemy::stateTable[])() = {
	&NormalEnemy::Inter,//動きの合間
	&NormalEnemy::Attack,//動きの合間

};

//行動
void NormalEnemy::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	//当たり判定
	vector<AttackArea*> _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//当たり判定
	PoisonState();//毒
}
//描画
void NormalEnemy::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui描画
void NormalEnemy::ImGui_Origin() {
	ImGui::Begin("Enemy");
	ImGui::Text("Poison:%d", m_Poison);
	ImGui::Text("Timer:%d", m_PoisonTimer);
	ImGui::End();
}
//開放
void NormalEnemy::Finalize() {

}
//追従
//void NormalEnemy::Follow() {
//	Helper::GetInstance()->FollowMove(m_Position, Player::GetInstance()->GetPosition(), 0.05f);
//}

void NormalEnemy::Inter() {
	coolTimer++;
	coolTimer = clamp(coolTimer, 0, kIntervalMax);
	if (coolTimer == kIntervalMax) {
		//_charaState = STATE_FOLLOW;
		coolTimer = 0;
	}
}

void NormalEnemy::Attack() {
	m_Position.x -= 0.07f;
	if (m_Position.x < -10.f) {
		_charaState = STATE_STANDBY;
		m_Position = randPanelPos();
	}
}

void NormalEnemy::Standby() {
}
