#pragma once
#include"InterEnemy.h"
#include "CounterBomb.h"
#include "LastBomb.h"
#include "AbsorptionEffect.h"
using namespace std;         //  名前空間指定
//ラスボスのお供の敵
class SupportEnemy :public InterEnemy {
public:
	SupportEnemy();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ClearAction()override;//クリア更新
	void GameOverAction()override;//ゲームオーバー更新
	void ImGui_Origin()override;
	void DeathSpecial()override;
	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	//関数ポインタ
	static void(SupportEnemy::* stateTable[])();
	static void(SupportEnemy::* attackTable[])();
private:
	void Inter();//待機
	void Attack();//攻撃
	void Teleport();//移動
	void BirthArea(const int Width, const int Height, const string& name);//攻撃エリア
	void BirthPredict(const int Width, const int Height, const string& name);//予測エリア

	void WarpEnemy();//敵のワープ処理
	void AttackMove();//攻撃時の動き
	void CounterAttack();
	void BombAttack();
	void BirthCounter();//カウンターのボム作成

	//魔法陣
	void BirthMagic();
	void BirthCharge();
private:
	static const int BULLET_NUM = 5;
private:
	std::vector<unique_ptr<CounterBomb>> counterbomb;
	std::vector<unique_ptr<AbsorptionEffect>> abseffect;
	unique_ptr<LastBomb> lastbomb;
	int m_AttackCount = {};
	int _charaState = STATE_INTER;

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
	//インターバルとか
	vector<int>m_Limit;
	//攻撃のインターバルとか
	vector<int>m_AttackLimit;
	int m_BulletNum = {};


	int m_ShotDir = {};
	float m_AfterRotY = {};
	bool m_ChangeRot = {};

	int m_SupportType = {};

	enum BombAttackType {
		SET_BOMB,
		JUMP_MOVE,
		FALL_MOVE,
	}_BombAttackType = SET_BOMB;

	enum AttackState {
		ATTACK_COUNTER,
		ATTACK_BOMB
	}_AttackState = ATTACK_COUNTER;
};

