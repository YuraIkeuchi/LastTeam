#include "ParticleEmitter.h"
#include "VariableCommon.h"
#include "ImageManager.h"
#include "Helper.h"
#include <random>
ParticleEmitter* ParticleEmitter::GetInstance()
{
	static ParticleEmitter instance;

	return &instance;
}

void ParticleEmitter::Initialize()
{
	//テクスチャ全読み込み
	LoadTexture();
	//パーティクルマネージャー生成
	circleParticle.reset(ParticleManager::Create(ImageManager::Normal));
	poisonParticle.reset(ParticleManager::Create(ImageManager::Normal));
	healParticle.reset(ParticleManager::Create(ImageManager::Heal));
	healParticleSecond.reset(ParticleManager::Create(ImageManager::Plus));
	smokeParticle.reset(ParticleManager::Create(ImageManager::Smoke));
	rockParticle.reset(ParticleManager::Create(ImageManager::Heal));
	deathParticle.reset(ParticleManager::Create(ImageManager::Heal));
}

void ParticleEmitter::Update()
{
	//パーティクルマネージャー更新
	circleParticle->Update();
	poisonParticle->Update();
	healParticle->Update();
	healParticleSecond->Update();
	smokeParticle->Update();
	rockParticle->Update();
	deathParticle->Update();
}

void ParticleEmitter::IntroDraw() {
}
void ParticleEmitter::FlontDrawAll() {
	circleParticle->Draw(AddBlendType);
	poisonParticle->Draw(AddBlendType);
	healParticle->Draw(AddBlendType);
	healParticleSecond->Draw(AddBlendType);
	smokeParticle->Draw(AlphaBlendType);
	rockParticle->Draw(AddBlendType);
	deathParticle->Draw(AddBlendType);
}
//炎のパーティクル
void ParticleEmitter::FireEffect(const int life, const XMFLOAT3& l_pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor)
{
	XMFLOAT3 pos = l_pos;
	const float rnd_vel = 0.05f;
	XMFLOAT3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel * 2.0f;// -rnd_vel / 2.0f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	circleParticle->Add(life, { pos.x,pos.y,pos.z }, vel, {}, startscale, endscale, startcolor, endcolor,{});
}
//毒のパーティクル
void ParticleEmitter::PoisonEffect(const int life, const XMFLOAT3& l_pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor, const float Gravity, float divi)
{
	XMFLOAT3 pos = l_pos;
	const float rnd_vel = 0.05f;
	XMFLOAT3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel * 2.0f;// -rnd_vel / 2.0f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	poisonParticle->Add(life, { pos.x,pos.y,pos.z }, vel, {}, startscale, endscale, startcolor, endcolor, Gravity,divi);
}
//ダメージ時のエフェクト
void ParticleEmitter::Break(const int life, const XMFLOAT3& pos, const float startscale, const float endscale,
	const XMFLOAT4& startcolor, const XMFLOAT4& endcolor, const float Gravity, float divi) {
	const int l_Division = 80;
	float l_AddPowerY = 0.0f;
	//乱数の範囲(-50から50)
	mt19937 mt{ std::random_device{}() };
	uniform_int_distribution<int> l_bounddist(-50, 50);

	XMFLOAT3 l_pos = pos;

	XMFLOAT3 vel{};

	vel = {
		(float)(l_bounddist(mt)) / l_Division,
		(float)(l_bounddist(mt)) / l_Division,
		(float)(l_bounddist(mt)) / l_Division,
	};

	vel.y += l_AddPowerY;

	circleParticle->Add(life, l_pos, vel, {}, startscale, endscale, startcolor, endcolor, Gravity, divi);
}

void ParticleEmitter::HealEffect(const int life, const XMFLOAT3& l_pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor) {
	XMFLOAT3 pos = l_pos;
	const float rnd_vel = 0.05f;
	XMFLOAT3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel * 2.0f;// -rnd_vel / 2.0f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

	XMFLOAT3 vel_s = { 0.f,vel.y,0.f };
	const float rnd_pos = 1.0f;
	XMFLOAT3 r_pos{};
	r_pos.x = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
	r_pos.y = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;// -rnd_vel / 2.0f;
	r_pos.z = (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

	healParticle->Add(life, { pos.x,pos.y,pos.z }, vel, {}, startscale, endscale, startcolor, endcolor, {});
	healParticleSecond->Add(life, { pos.x + r_pos.x,pos.y + r_pos.y,pos.z + r_pos.z }, vel_s,{}, 0.3f, 0.3f, {1.f,1.f,1.f,1.f}, {1.f,1.f,1.f,0.f}, {});
}

void ParticleEmitter::SmokeEffect(const int life, const XMFLOAT3& l_pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor,const int number) {
	const float l_Velocity = 0.05f;
	XMFLOAT3 pos = l_pos;
	float angle = {};
	angle = number * 45.0f;
	XMFLOAT3 vel{};
	vel.x = l_Velocity * sinf(angle);
	vel.y = 0.0f;
	vel.z = l_Velocity * -cosf(angle);
	smokeParticle->Add(life, pos, vel, XMFLOAT3(), startscale, endscale, startcolor, endcolor,{});
}

void ParticleEmitter::DeathEffect(const int life, const XMFLOAT3& l_pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor, const int number) {
	const float l_Velocity = 0.08f;
	XMFLOAT3 pos = l_pos;
	float angle = {};
	angle = number * 45.0f;
	XMFLOAT3 vel{};
	vel.x = l_Velocity * sinf(angle);
	vel.y = 0.0f;
	vel.z = l_Velocity * -cosf(angle);
	deathParticle->Add(life, pos, vel, XMFLOAT3(), startscale, endscale, startcolor, endcolor, {});
}

void ParticleEmitter::AllDelete()
{
	//パーティクルマネージャー更新
	circleParticle->AllDelete();
	poisonParticle->AllDelete();
	healParticle->AllDelete();
	healParticleSecond->AllDelete();
	smokeParticle->AllDelete();
	rockParticle->AllDelete();
}

void ParticleEmitter::RockEffect(const int life, const XMFLOAT3& l_pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor) {
	XMFLOAT3 pos = l_pos;
	const float rnd_vel = 0.05f;
	XMFLOAT3 vel{};
	vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
	vel.y = (float)rand() / RAND_MAX * rnd_vel * 1.2f;// -rnd_vel / 2.0f;
	vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;


	rockParticle->Add(life, pos, vel, {}, startscale, endscale, startcolor, endcolor, {});
}
//爆発
void ParticleEmitter::Explosion(const int life, const XMFLOAT3& pos, const float size, const float startscale,
	const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor, const int ExploType) {
	float RandPos = {};
	//X,Y,Z全て[-0.8f, +0.8f]でランダムに分布
	float RandPos2 = {};
	//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
	float RandVel = {};
	//爆発の規模でエフェクトも変わる
	if (ExploType == 0) {
		RandPos = 2.0f;
		RandPos2 = 0.8f;
		RandVel = 0.05f;
	}
	else if (ExploType == 1) {
		RandPos = 9.0f;
		RandPos2 = 0.8f;
		RandVel = 0.05f;
	}
	else {
		RandPos = 12.0f;
		RandPos2 = 0.8f;
		RandVel = 0.05f;
	}
	for (int j = 0; j < 3; j++) {
		XMFLOAT3 l_pos = pos;
		l_pos.x += ((float)rand() / RAND_MAX * RandPos - RandPos / 2.0f) * size;
		l_pos.y += ((float)rand() / RAND_MAX * RandPos - RandPos / 2.0f) * size;
		l_pos.z += ((float)rand() / RAND_MAX * RandPos - RandPos / 2.0f) * size;

		for (int i = 0; i < 10; i++) {

			l_pos.x += ((float)rand() / RAND_MAX * RandPos2 - RandPos2 / 2.0f) * size;
			l_pos.y += ((float)rand() / RAND_MAX * RandPos2 - RandPos2 / 2.0f) * size;
			l_pos.z += ((float)rand() / RAND_MAX * RandPos2 - RandPos2 / 2.0f) * size;


			XMFLOAT3 vel{};
			vel.x = ((float)rand() / RAND_MAX * RandVel - RandVel / 2.0f) * size;
			vel.y = ((float)rand() / RAND_MAX * RandVel - RandVel / 2.0f) * size;
			vel.z = ((float)rand() / RAND_MAX * RandVel - RandVel / 2.0f) * size;


			//追加
			circleParticle->Add(life, l_pos, vel, {}, startscale, endscale, startcolor, endcolor, {});
		}
	}
}


void ParticleEmitter::LoadTexture() {
	ImageManager::GetInstance()->LoadParticle();
}