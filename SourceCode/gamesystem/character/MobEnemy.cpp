#include "MobEnemy.h"
#include <random>
#include "Player.h"
#include "Collision.h"
#include "CsvLoader.h"
#include "Helper.h"
#include "Easing.h"
#include "ImageManager.h"
#include <GameStateManager.h>
#include <StagePanel.h>
//モデル読み込み
MobEnemy::MobEnemy() {
	BaseInitialize(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	/*shadow_tex.reset(new IKETexture(ImageManager::SHADOW, m_Position, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	shadow_tex->TextureCreate();
	shadow_tex->Initialize();
	shadow_tex->SetRotation({ 90.0f,0.0f,0.0f });*/
}
//初期化
bool MobEnemy::Initialize() {
	m_Position = StagePanel::GetInstance()->SetPositon(4,1);
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.4f,0.4f,0.4f };
	m_HP = 15.0f;
	m_MaxHP = m_HP;
	m_EnemyTag = "Mob";
	m_ShadowScale = { 0.05f,0.05f,0.05f };
	for (auto i = 0; i < _drawPoisonnumber.size(); i++) {
		_drawPoisonnumber[i] = make_unique<DrawPoisonNumber>(0.5f);
		_drawPoisonnumber[i]->Initialize();
	}
	return true;
}

//行動
void MobEnemy::Action() {
	//StagePanel::GetInstance()->SetPanelSearch(m_Object.get(), m_NowWidth, m_NowHeight);
	Obj_SetParam();
	//Collide();		//当たり判定
	//当たり判定
	vector<unique_ptr<AttackArea>>& _AttackArea = GameStateManager::GetInstance()->GetAttackArea();
	Collide(_AttackArea);		//当たり判定

	m_ShadowPos = { m_Position.x,m_Position.y + 0.11f,m_Position.z };
	//shadow_tex->SetPosition(m_ShadowPos);
	//shadow_tex->SetScale(m_ShadowScale);
	//shadow_tex->Update();
}
//描画
void MobEnemy::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	//shadow_tex->Draw();
	if (m_SuperPoison) {poison_tex->Draw();}
	if (m_HealDamage) { healdamage_tex->Draw(); }
	counter_tex->Draw();
	IKETexture::PostDraw();
	Obj_Draw();
}
//ImGui描画
void MobEnemy::ImGui_Origin() {
	ImGui::Begin("Enemy");
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::Text("POSZ:%f", m_Position.z);
	ImGui::Text("Width:%d", m_NowWidth);
	ImGui::Text("Height:%d", m_NowHeight);
	ImGui::End();
}
//開放
void MobEnemy::Finalize() {

}
