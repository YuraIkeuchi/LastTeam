#include "ParticleEmitter2D.h"
#include "VariableCommon.h"
#include "ImageManager.h"
#include "Helper.h"
#include <random>
ParticleEmitter2D* ParticleEmitter2D::GetInstance()
{
	static ParticleEmitter2D instance;

	return &instance;
}

void ParticleEmitter2D::Initialize()
{
	//テクスチャ全読み込み
	LoadTexture();
	//パーティクルマネージャー生成
	circleParticle = ParticleManager2D::Create(ImageManager::Normal);
}

void ParticleEmitter2D::Update()
{
	//パーティクルマネージャー更新
	circleParticle->Update();
}

void ParticleEmitter2D::IntroDraw() {
}
void ParticleEmitter2D::FlontDrawAll() {
	circleParticle->Draw(AddBlendType);
}
//炎のパーティクル
void ParticleEmitter2D::FireEffect(const int life, const XMFLOAT2& l_pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor)
{
	XMFLOAT2 pos = l_pos;
	const float rnd_vel = 0.05f;
	XMFLOAT2 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel * 2.0f;// -rnd_vel / 2.0f;

	circleParticle->Add(life, { pos.x,pos.y }, vel, {}, startscale, endscale, startcolor, endcolor, {});
}

//ダメージ時のエフェクト
void ParticleEmitter2D::Break(const int life, const XMFLOAT2& pos, const float startscale, const float endscale,
	const XMFLOAT4& startcolor, const XMFLOAT4& endcolor, const float Gravity, float divi) {
	const int l_Division = 80;
	float l_AddPowerY = 0.0f;
	//乱数の範囲(-50から50)
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_bounddist(-50, 50);

	XMFLOAT2 l_pos = pos;

	XMFLOAT2 vel{};

	vel = {
		(float)(l_bounddist(mt)) / l_Division,
		(float)(l_bounddist(mt)) / l_Division,
	};

	vel.y += l_AddPowerY;

	circleParticle->Add(life, l_pos, vel, {}, startscale, endscale, startcolor, endcolor, Gravity, divi);
}

void ParticleEmitter2D::AllDelete()
{
	//パーティクルマネージャー更新
	circleParticle->AllDelete();
}

void ParticleEmitter2D::LoadTexture() {
	ImageManager::GetInstance()->LoadParticle();
}