#pragma once
#include"InterEnemy.h"
using namespace std;         //  名前空間指定
//普通の敵
class TutorialEnemy :public InterEnemy {
public:
	TutorialEnemy();
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
	static void(TutorialEnemy::* stateTable[])();

private:
	void Inter();//待機
	void StandBy();
	void Attack();
	void Teleport();//移動

	void WarpEnemy();//敵のワープ処理
	//魔法陣
	void BirthMagic();

	bool TackleCollide();

public:

private:

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
	float m_Speed = {};
	float m_Damage = {};
	bool m_Hit = false;
	float m_Frame = {};

	std::unique_ptr<PredictArea> predictArea;
	float predictFrame = 0.f;
	int nextPredict = 0;
	int m_OldWidth = m_NowWidth;
	int m_OldHeight = m_NowHeight;

};

