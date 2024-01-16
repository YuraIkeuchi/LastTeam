#pragma once
#include"InterEnemy.h"
#include "EnemyRock.h"
#include "EnemyTornade.h"
using namespace std;         //  名前空間指定
//普通の敵
class BossEnemy3 :public InterEnemy {
public:
	BossEnemy3();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ClearAction()override;//クリア更新
	void GameOverAction()override;//ゲームオーバー更新
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	//関数ポインタ
	static void(BossEnemy3::* stateTable[])();
	static void(BossEnemy3::* attackTable[])();
private:
	void Inter();//待機
	void Attack();//攻撃
	void Teleport();//移動
	void RockAttack();//岩の攻撃
	void RandomAttack();//ランダム攻撃
	void AroundAttack();//周りの攻撃
	void BirthRock();//
	void BirthArea(const int Width, const int Height, const string& name);//攻撃エリア
	void BirthPredict(const int Width, const int Height, const string& name);//予測エリア

	//スキルのCSVを読み取る
	void LoadCsvSkill(std::string& FileName, const int id);
	//CSVを作成
	bool CreateSkill(int id);
	void PlayerCollide();
	void WarpEnemy();//敵のワープ処理
	void AttackMove();//攻撃時の動き
	void SelectSafeArea();
//魔法陣
	void BirthMagic();
private:
	static const int SUPPORT_NUM = 2;
	static const int BULLET_NUM = 5;
private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
		// 岩エリア
	std::vector<unique_ptr<EnemyRock>> enerock;
	
	enum AttackState {
		ATTACK_ROCK,
		ATTACK_RANDOM,
		ATTACK_AROUND,
	}_AttackState = ATTACK_RANDOM;
	std::vector<std::vector<int>> m_Area = {};
	// 攻撃エリア
	std::vector<unique_ptr<EnemyTornade>> enetornade;
	
	int m_RandWidth = {};
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
	int m_BulletNum = {};


	int m_ShotDir = {};
	float m_AfterRotY = {};
	bool m_ChangeRot = {};

	bool m_SafeArea[4][4];

	int m_RockCount = {};
};

