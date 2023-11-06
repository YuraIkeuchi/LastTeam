#include "TutorialEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
//モデル読み込み
TutorialEnemy::TutorialEnemy() {
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	//HPII
	hptex = IKESprite::Create(ImageManager::ENEMYHPUI, { 0.0f,0.0f });

	for (auto i = 0; i < _drawnumber.size(); i++) {
		_drawnumber[i] = make_unique<DrawNumber>();
		_drawnumber[i]->Initialize();
	}
}
//初期化
bool TutorialEnemy::Initialize() {

	m_Position = { 2.0f,1.0f,2.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };
	m_HP = 5.0f;
	m_MaxHP = m_HP;
	return true;
}

//行動
void TutorialEnemy::Action() {
	Obj_SetParam();
	//当たり判定
	vector<AttackArea*> _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);
}
//描画
void TutorialEnemy::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGui描画
void TutorialEnemy::ImGui_Origin() {
	ImGui::Begin("Enemy");
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::Text("POSZ:%f", m_Position.z);
	ImGui::Text("Width:%d", m_NowWidth);
	ImGui::Text("Height:%d", m_NowHeight);
	ImGui::End();
}
//開放
void TutorialEnemy::Finalize() {

}
