#pragma once
#include"InterEnemy.h"
#include "EnemyBullet.h"

using namespace std;         //  名前空間指定
//普通の敵
class BossEnemy :public InterEnemy {
public:
	BossEnemy();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	//関数ポインタ
	static void(BossEnemy::* stateTable[])();
	static void(BossEnemy::* attackTable[])();
private:
	void Inter();//待機
	void Attack();//攻撃
	void Teleport();//移動
	void BulletAttack();//弾の攻撃
	void BirthBullet();//
private:
	static const int BULLET_NUM = 5;
private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
	vector<unique_ptr<EnemyBullet>> bullets;//ポルターガイスト

	enum BossType {
		Boss_SET,
		Boss_THROW,
		Boss_END,
	}_BossType = Boss_SET;

	enum AttackState {
		ATTACK_BULLET,
	}_AttackState = ATTACK_BULLET;
};

