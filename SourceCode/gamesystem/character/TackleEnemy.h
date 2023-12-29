#pragma once
#include"InterEnemy.h"
using namespace std;         //  名前空間指定
//普通の敵
class TackleEnemy :public InterEnemy {
public:
	TackleEnemy();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	//関数ポインタ
	static void(TackleEnemy::* stateTable[])();

private:
	void Inter();//待機
	void Attack();
	void Teleport();//移動
	void StandBy();//移動

	void WarpEnemy();//敵のワープ処理
	//魔法陣
	void BirthMagic();

	bool TackleCollide();

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

	struct PredictPanel {
		unique_ptr<IKETexture> tex;
		XMFLOAT3 pos = {};
		XMFLOAT3 afterPos = {};
		XMFLOAT3 beforePos = {};
		XMFLOAT3 scale = {};
		int width = -1;
		int height = -1;
		float usefulFrame = 0.f;
		bool isVisible = false;
		bool isVerse = false;
		bool isVanish = false;
	};

	static const int kPanelMax = 8;
	PredictPanel predictPanels[kPanelMax];
	float predictFrame = 0.f;
	int nextPredict = 0;
	int m_OldWidth = m_NowWidth;
	int m_OldHeight = m_NowHeight;
	//インターバルとか
	vector<int>m_Limit;
	float m_Speed = {};
	float m_Damage = {};
	bool m_Hit = false;
	float m_Frame = {};
};

