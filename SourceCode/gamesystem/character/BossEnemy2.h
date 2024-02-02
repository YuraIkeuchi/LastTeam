#pragma once
#include"InterEnemy.h"
#include "EnemyBullet.h"
#include "EnemyThorn.h"
#include "ConfuEffect.h"
using namespace std;         //  名前空間指定
//普通の敵
class BossEnemy2 :public InterEnemy {
public:
	BossEnemy2();
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
	static void(BossEnemy2::* stateTable[])();
	static void(BossEnemy2::* attackTable[])();
private:
	void Inter();//待機
	void Attack();//攻撃
	void Teleport();//移動
	void SpinningAttack();
	void ShockWaveAttack();
	void Recovery();
	void Stun();

	void BirthArea(const int Width, const int Height, const string& name);//攻撃エリア
	void BirthPredict(const int Width, const int Height, const string& name);//予測エリア
	void BirthChantingHealParticle();

	void SpinningAttackBirthPredict(int AttackCount);
	void SpinningAttackBirthArea(int AttackCount);
	void ShockWaveAttackBirthPredict(int AttackCount);
	void ShockWaveAttackBirthArea(int AttackCount);

	//スキルのCSVを読み取る
	void LoadCsvSkill(std::string& FileName, const int id);
	//CSVを作成
	bool CreateSkill(int id);
	void PlayerCollide();
	void WarpEnemy();//敵のワープ処理
	void AttackMove();//攻撃時の動き
	//魔法陣
	void BirthMagic();
private:
	static const int BULLET_NUM = 5;
private:
	unique_ptr<ConfuEffect> confueffect;
	int m_AttackCount = {};
	int _charaState = STATE_INTER;

	enum BossType {
		Boss_SET,
		Boss_THROW,
		Boss_END,
	}_BossType = Boss_SET;

	enum AttackState {
		ATTACK_SPINNING,
		ATTACK_SHOCKWAVE,
		ATTACK_RECOVERY,
		ATTACK_STUN
	}_AttackState = ATTACK_SPINNING;
	std::vector<std::vector<int>> m_Area = {};
	// 攻撃エリア
	std::vector<unique_ptr<EnemyThorn>> enethorn;


	int m_RandWigth = {};
	int m_RandHeight = {};

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

	float m_RotFrame = {};
	int m_ShotDir = {};
	float m_AfterRotY = {};
	bool m_ChangeRot = {};
	bool m_RecoverySaveHP = {};
	float m_TmpHP = {};
	bool m_isStun = {};
	int m_ChantingCount = {};
};

