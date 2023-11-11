#include "MobEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
//ÉÇÉfÉãì«Ç›çûÇ›
MobEnemy::MobEnemy() {
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
bool MobEnemy::Initialize() {
	m_Position = { 2.0f,0.1f,2.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };
	m_HP = 5.0f;
	m_MaxHP = m_HP;
	m_EnemyTag = "Mob";
	return true;
}

//çsìÆ
void MobEnemy::Action() {
	Obj_SetParam();
	//ìñÇΩÇËîªíË
	vector<AttackArea*> _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);
}
//ï`âÊ
void MobEnemy::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGuiï`âÊ
void MobEnemy::ImGui_Origin() {
	ImGui::Begin("Enemy");
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::Text("POSZ:%f", m_Position.z);
	ImGui::Text("Width:%d", m_NowWidth);
	ImGui::Text("Height:%d", m_NowHeight);
	ImGui::End();
}
//äJï˙
void MobEnemy::Finalize() {

}
