#pragma once
#include "ParticleManager.h"

using namespace std;         //  名前空間指定
/// <summary>
/// パーティクルエミッター
/// </summary>
class ParticleEmitter
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: //シングルトン化
	//コンストラクタを隠蔽
	ParticleEmitter() = default;
	//デストラクタを隠蔽
	~ParticleEmitter() = default;
public: //メンバ関数
	static ParticleEmitter* GetInstance();

	//初期化
	void Initialize();

	//更新
	void Update();

	void IntroDraw();
	//描画
	void FlontDrawAll();

	//エフェクト
	void FireEffect(const int life,const XMFLOAT3& pos,const float startscale, const float endscale,const XMFLOAT4& startcolor,const XMFLOAT4& endcolor);
	//毒
	void PoisonEffect(const int life, const XMFLOAT3& pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor, const float Gravity, float divi);
	//ダメージエフェクト
	void Break(const int life, const XMFLOAT3& pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor, const float Gravity, float divi);
	//ヒールエフェクト
	void HealEffect(const int life, const XMFLOAT3& pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor);
	//煙エフェクト
	void SmokeEffect(const int life, const XMFLOAT3& pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor,const int number);
	//岩のエフェクト
	void RockEffect(const int life, const XMFLOAT3& pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor);
	//死亡時のエフェクト
	void DeathEffect(const int life, const XMFLOAT3& pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor, const int number);
	//爆発エフェクト
	void Explosion(const int life, const XMFLOAT3& pos, const float size, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor, const int ExploType);
	//ボス用ヒール詠唱エフェクト
	void ChantingHealEffect(const int life, const XMFLOAT3& pos, const float startscale, const float endscale, const XMFLOAT4& startcolor, const XMFLOAT4& endcolor);
	void AllDelete();
private: //メンバ関数
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture();

private: //メンバ変数
	//パーティクルマネージャー(丸)
	unique_ptr<ParticleManager> circleParticle;
	//パーティクルマネージャー(毒)
	unique_ptr<ParticleManager> poisonParticle;
	//パーティクルマネージャー(ヒール)
	unique_ptr<ParticleManager> healParticle;
	//パーティクルマネージャー(ヒール2)
	unique_ptr<ParticleManager> healParticleSecond;
	unique_ptr<ParticleManager> smokeParticle;
	//パーティクルマネージャー(岩のエフェクト)
	unique_ptr<ParticleManager> rockParticle;
	//パーティクルマネージャー(死亡)
	unique_ptr<ParticleManager> deathParticle;
	//パーティクルマネージャー(ボスの詠唱ヒール)
	unique_ptr<ParticleManager> chantingHealParticle;
private:
	int m_Timer = 0;
};
