#include "TitleEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
//ÉÇÉfÉãì«Ç›çûÇ›(ñàâÒÉâÉìÉ_ÉÄÇ≈ìGïœÇÌÇ¡ÇΩÇÁÇ¢Ç¢Ç∆évÇ§)
TitleEnemy::TitleEnemy() {
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
//èâä˙âª
bool TitleEnemy::Initialize() {

	m_Position = { 2.0f,1.0f,2.0f };
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };
	m_HP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "hp")));
	m_MaxHP = m_HP;
	return true;
}

//çsìÆ
void TitleEnemy::Action() {
	m_Rotation.y += 2.0f;
	Obj_SetParam();
}
//ï`âÊ
void TitleEnemy::Draw(DirectXCommon* dxCommon) {
	Obj_Draw();
}
//ImGuiï`âÊ
void TitleEnemy::ImGui_Origin() {
	ImGui::Begin("Enemy");
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::Text("POSZ:%f", m_Position.z);
	ImGui::Text("Width:%d", m_NowWidth);
	ImGui::Text("Height:%d", m_NowHeight);
	ImGui::End();
}
//äJï˙
void TitleEnemy::Finalize() {

}
