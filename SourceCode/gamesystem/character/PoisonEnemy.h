#pragma once
#include"InterEnemy.h"
#include "PoisonArea.h"

using namespace std;         //  名前空間指定
//普通の敵
class PoisonEnemy :public InterEnemy {
public:
	PoisonEnemy();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	//関数ポインタ
	static void(PoisonEnemy::* stateTable[])();

private:
	void Inter();//待機
	void Attack();//攻撃
	void Teleport();//移動
	void BirthPoison();//
	void WarpEnemy();//敵のワープ処理

	//魔法陣
	void BirthMagic();
private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;

	enum PoisonType {
		Poison_SET,
		Poison_THROW,
		Poison_END,
	}_PoisonType = Poison_SET;

	//魔法陣系
	enum MagicState {
		MAGIC_BIRTH,
		MAGIC_VANISH,
	};

	struct Magic {
		unique_ptr<IKETexture> tex;
		float Frame = {};
		float Scale = {};
		float AfterScale = {};
		XMFLOAT3 Pos = {};
		bool Alive = false;
		int State = {};
		int Timer = {};
	};
	Magic magic;

	bool m_Warp = false;

	enum WarpState {
		WARP_START,
		WARP_END,
	};

	struct EnemyWarp {
		float Frame = {};
		float Scale = {};
		float AfterScale = 0.5f;
		int State = {};
	};

	EnemyWarp enemywarp;

	std::vector<unique_ptr<PoisonArea>> poisonarea;

	//インターバルとか
	vector<int>m_Limit;
	int m_BulletNum = {};
};

