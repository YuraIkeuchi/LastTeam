#include "CounterBomb.h"
#include <StagePanel.h>
#include <ImageManager.h>
#include <ModelManager.h>
#include <GameStateManager.h>
#include <Helper.h>
#include <ParticleEmitter.h>
//読み込み
CounterBomb::CounterBomb() {
	m_Model = ModelManager::GetInstance()->GetModel(ModelManager::BULLET);
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(m_Model);
	panel.tex.reset(new IKETexture(ImageManager::AREA, {}, { 1.f,1.f,1.f }, { 1.f,0.4f,0.4f,1.f }));
	panel.tex->TextureCreate();
	panel.tex->Initialize();
	panel.tex->SetScale({ 0.0f,0.0f,0.0f });
	panel.tex->SetRotation({ 90.0f,0.0f,0.0f });
	panel.color = { 1.f,0.4f,0.4f,1.f };
	Initialize();
}
//初期化
bool CounterBomb::Initialize() {
	m_Damage = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/CounterBomb.csv", "BOMB_DAMAGE")));
	return true;
}

//ステータス初期化
void CounterBomb::InitState(const int width, const int height) {
	m_NowWidth = width, m_NowHeight = height;
	m_AfterPos = SetPanelPos(width, height);
	panel.position = { m_AfterPos.x,0.012f,m_AfterPos.z };
	//弾
	m_Alive = true;
	float baseScale = PANEL_SIZE * 0.1f;
	m_Scale = { baseScale,baseScale,baseScale };
	m_Color = { 1.0f,0.5f,0.5f,1.0f };
}

//更新
void CounterBomb::Update() {
	Obj_SetParam();
	Collide();
	Move();

	if (panel.predict) {
		panel.scale = Helper::Lerp(0.0f, PANEL_SIZE * 0.2f, m_Timer, m_TargetTimer);		//線形補間でチャージを表してる
	}
	panel.tex->SetScale({ panel.scale,panel.scale,panel.scale });
	panel.tex->SetPosition(panel.position);
	panel.tex->SetColor(panel.color);
	panel.tex->Update();
}
//描画
void CounterBomb::Draw(DirectXCommon* dxCommon) {
	IKETexture::PreDraw2(dxCommon, AlphaBlendType);
	if(panel.predict)
	panel.tex->Draw();
	IKETexture::PostDraw();
	if (m_Alive)
		Obj_Draw();
}
//ImGui
void CounterBomb::ImGuiDraw() {
}
//パネルの位置に置く
XMFLOAT3 CounterBomb::SetPanelPos(const int width, const int height) {
	return StagePanel::GetInstance()->SetPositon(width, height);;
}
void CounterBomb::Collide() {
	int l_PlayerWidth = player->GetNowWidth();
	int l_PlayerHeight = player->GetNowHeight();
	if (_BombState != BOMB_DELETE) { return; }
	if (m_Hit) { return; }
	//毒のマスとプレイヤーが一緒だとダメージを食らう
	if (m_NowWidth == l_PlayerWidth && m_NowHeight == l_PlayerHeight) {
		player->RecvDamage(m_Damage, "NORMAL");
		m_Hit = true;
	}
}
void CounterBomb::Move() {
	const float l_TargetPosY = 10.0f;
	const float l_ThrowSpeed = 0.4f;

	m_Timer++;
	m_TargetTimer = 100;
	if (_BombState == BOMB_THROW) {			//上に上げる
		panel.predict = true;
		if (Helper::CheckMin(m_Position.y, l_TargetPosY, l_ThrowSpeed)) {
			_BombState = BOMB_DROP;
			m_Position = { m_AfterPos.x,l_TargetPosY,m_AfterPos.z };
		}
	}
	else if (_BombState == BOMB_DROP) {		//落ちてくる
		if (Helper::CheckMax(m_Position.y, 0.0f, -l_ThrowSpeed)) {
			panel.scale = {};
			_BombState = BOMB_DELETE;
			panel.predict = false;
		}
	}
	else {
		BirthExplosionParticle();
		if (Helper::CheckMin(m_DeathTimer, 5, 1)) {
			m_Alive = false;
		}
	}
}
//爆発エフェクト
void CounterBomb::BirthExplosionParticle() {
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