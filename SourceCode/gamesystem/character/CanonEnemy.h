#pragma once
#include"InterEnemy.h"
#include "EnemyBullet.h"

using namespace std;         //  名前空間指定
//普通の敵
class CanonEnemy :public InterEnemy {
public:
	CanonEnemy();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	//関数ポインタ
	static void(CanonEnemy::* stateTable[])();

private:
	void Inter();//待機
	void Attack();//攻撃
	void Teleport();//移動
	void BirthBullet();//
	void WarpEnemy();//敵のワープ処理
	//魔法陣
	void BirthMagic();
private:
static const int BULLET_NUM = 3;
private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
	vector<unique_ptr<EnemyBullet>> bullets;//ポルターガイスト

	enum CanonType {
		CANON_SET,
		CANON_THROW,
		CANON_END,
	}_CanonType = CANON_SET;

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
	int m_BulletNum = {};
	float m_RotFrame = {};
	int m_ShotDir = {};
	float m_AfterRotY = {};
	bool m_ChangeRot = {};
	float m_BaseScale = {};
	float m_ScaleFrame = {};
};

