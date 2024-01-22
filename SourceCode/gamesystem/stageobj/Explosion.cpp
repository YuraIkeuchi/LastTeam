#include "Explosion.h"
#include <ParticleEmitter.h>
#include <StagePanel.h>
#include <Helper.h>
#include <CsvLoader.h>
Explosion::Explosion() {
	Initialize();
}
bool Explosion::Initialize() {
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/SupportEnemy.csv", "EXPLO_DAMAGE")));
	return true;
}
//初期化
void Explosion::InitState(const int width, const int height) {
	m_Position = SetPanelPos(width, height);
	m_Position.y = -0.5f;
	m_NowWidth = width, m_NowHeight = height;
	m_Alive = true;
	m_Hit = false;
	m_AliveTimer = {};
}
//更新
void Explosion::Update() {
	Move();
	Collide();
}
void Explosion::ImGuiDraw() {
	ImGui::Begin("Explosion");
	ImGui::Text("Timer:%d", m_AliveTimer);
	ImGui::End();
}
//動き
void Explosion::Move() {
	const int l_TargetTimer = 5;
	if(Helper::CheckMin(m_AliveTimer,l_TargetTimer,1)){
		m_Alive = false;
	}

	BirthParticle();
}
//当たり判定
void Explosion::Collide() {
	if (m_Hit) { return; }

	int l_PlayerWidth = player->GetNowWidth();
	int l_PlayerHeight = player->GetNowHeight();
	if (m_NowWidth == l_PlayerWidth && m_NowHeight == l_PlayerHeight) {
		m_Hit = true;
		player->RecvDamage(m_Damage);
	}
}
//パネルの位置に置く
XMFLOAT3 Explosion::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}
//パーティクル
void Explosion::BirthParticle() {
	float l_AddSize = 0.4f;
	const float RandScale = 1.0f;
	float s_scale = 0.4f * l_AddSize;
	float e_scale = (4.0f + (float)rand() / RAND_MAX * RandScale - RandScale / 2.0f) * l_AddSize;

	//色
	const float RandRed = 0.2f;
	const float red = 0.2f + (float)rand() / RAND_MAX * RandRed;
	const XMFLOAT4 s_color = { 0.9f, red, 0.1f, 1.0f }; //濃い赤
	const XMFLOAT4 e_color = { 0, 0, 0, 1.0f }; //無色

	//乱数指定
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_Randlife(10, 30);
	int l_Life = int(l_Randlife(mt));

	for (int i = 0; i < 3; i++) {
		ParticleEmitter::GetInstance()->Explosion(l_Life, m_Position, l_AddSize, s_scale, e_scale, s_color, e_color, 0);
	}
}