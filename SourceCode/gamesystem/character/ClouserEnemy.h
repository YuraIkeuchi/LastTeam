#pragma once
#include"InterEnemy.h"
#include "EnemyRock.h"

using namespace std;         //  名前空間指定
//普通の敵
class ClouserEnemy :public InterEnemy {
public:
	ClouserEnemy();
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
	static void(ClouserEnemy::* stateTable[])();
	static void(ClouserEnemy::* attackTable[])();
private:
	void Inter();//待機
	void Attack();//攻撃
	void Teleport();//移動
	void BirthArea(const int Width, const int Height);//攻撃エリア
	void BirthPredict(const int Width, const int Height);//予測エリア
	void WarpEnemy();//敵のワープ処理
	void AttackMove();//攻撃時の動き
//魔法陣
	void BirthMagic();

private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
	
	std::vector<std::vector<int>> m_Area = {};
	// 岩エリア
	std::vector<unique_ptr<EnemyRock>> enerock;


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

	int m_ShotDir = {};
	float m_AfterRotY = {};
	bool m_ChangeRot = {};
};

