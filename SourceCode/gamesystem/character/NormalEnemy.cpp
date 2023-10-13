#include "NormalEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
//モデル読み込み
NormalEnemy::NormalEnemy() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	//HPII
	hptex = IKESprite::Create(ImageManager::ENEMYHPUI, { 0.0f,0.0f });
	const int NumberCount = NUMBER_MAX;
	const float l_Width_Cut = 64.0f;
	const float l_Height_Cut = 64.0f;

	for (int i = 0; i < HP_First.size(); i++) {
		//ひとけた目
		HP_First[i] = IKESprite::Create(ImageManager::HPNUMBERUI, { 0.0f,0.0f });
		int number_index_y = i / NumberCount;
		int number_index_x = i % NumberCount;
		HP_First[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		HP_First[i]->SetAnchorPoint({ 0.5f,0.5f });
		HP_First[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		HP_First[i]->SetScale(0.3f);
		//二桁め
		HP_Second[i] = IKESprite::Create(ImageManager::HPNUMBERUI, { 0.0f,0.0f });
		HP_Second[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		HP_Second[i]->SetAnchorPoint({ 0.5f,0.5f });
		HP_Second[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		HP_Second[i]->SetScale(0.3f);
		//三桁目
		HP_Third[i] = IKESprite::Create(ImageManager::HPNUMBERUI, { 0.0f,0.0f });
		HP_Third[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		HP_Third[i]->SetAnchorPoint({ 0.5f,0.5f });
		HP_Third[i]->SetSize({ l_Width_Cut,l_Height_Cut });
		HP_Third[i]->SetScale(0.3f);
	}
}
//初期化
bool NormalEnemy::Initialize() {

	m_Position = randPanelPos();
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
	Collide();
	Player::GetInstance()->AttackTarget(m_Position);
}
//描画
void NormalEnemy::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui描画
void NormalEnemy::ImGui_Origin() {
	ImGui::Begin("Enemy");
	ImGui::Text("HP:%d", m_InterHP);
	ImGui::Text("DamegeTimer:%d", m_DamegeTimer);
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
