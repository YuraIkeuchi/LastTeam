#pragma once
#include"InterEnemy.h"

using namespace std;         //  名前空間指定
//普通の敵
class Bomb :public InterEnemy {
public:
	Bomb();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	//関数ポインタ
	static void(Bomb::* stateTable[])();

private:
	void Inter();//待機
	void Attack();//攻撃
	void ShockWave();
public:
	void SetTargetPos(const XMFLOAT3& TargetPos) { m_TargetPos = TargetPos; }
private:
	unique_ptr<IKETexture> shockWaveTex = nullptr;
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
	bool _isPlayerDamage = false;
	bool _isEnemyDamage = false;
	XMFLOAT4 m_shockWaveColor = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT3 m_shockWaveScale = { 0.0f,0.0f,0.0f };
	const float m_addShockWaveScale = 0.15f;
	int m_shockWaveTimer = 0;
	int m_maxShockWaveTimer = 30;
	float m_BaseScale = {};
	float m_Damage = 0.0f;
	float m_SinAngle = {};
	float m_SinAngle2 = {};
	bool m_FlashStart = false;
	float m_AddAngle = {};
	XMFLOAT3 m_TargetPos = {};
	enum BombState {
		BOMB_SET,
		BOMB_THROW,
	}_BombState = BOMB_SET;
	float m_Frame = {};
};
