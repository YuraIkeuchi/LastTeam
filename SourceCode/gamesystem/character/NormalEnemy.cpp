#include "NormalEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
#include <StagePanel.h>
//ÉÇÉfÉãì«Ç›çûÇ›
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
//èâä˙âª
bool NormalEnemy::Initialize() {

	//m_Position = randPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "hp")));
	m_MaxHP = m_HP;
	StagePanel::GetInstance()->EnemyHitReset();
	return true;
}

void (NormalEnemy::* NormalEnemy::stateTable[])() = {
	&NormalEnemy::Inter,//ìÆÇ´ÇÃçáä‘
	&NormalEnemy::Attack,//ìÆÇ´ÇÃçáä‘

};

//çsìÆ
void NormalEnemy::Action() {
	(this->*stateTable[_charaState])();
	m_Rotation.y += 2.0f;
	Obj_SetParam();
	//ìñÇΩÇËîªíË
	vector<AttackArea*> _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//ìñÇΩÇËîªíË
	PoisonState();//ì≈
}
//ï`âÊ
void NormalEnemy::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGuiï`âÊ
void NormalEnemy::ImGui_Origin() {
	ImGui::Begin("NormalEnemy");
	ImGui::Text("Height:%d,Width:%d", m_NowHeight,m_NowWidth);
	ImGui::Text("Timer:%d", m_PoisonTimer);
	ImGui::End();
}
//äJï˙
void NormalEnemy::Finalize() {

}
//í«è]
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
		_charaState = STATE_SPECIAL;
		m_Position = randPanelPos();
	}
}

void NormalEnemy::Standby() {
}
