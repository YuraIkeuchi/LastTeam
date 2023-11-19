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
private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
	bool _isPlayerDamage = false;
	bool _isEnemyDamage = false;

	enum BombType {
		Bomb_SET,
		Bomb_THROW,
		Bomb_END,
	}_BombType = Bomb_SET;
};
