#pragma once
#include"InterEnemy.h"
using namespace std;         //  名前空間指定
//普通の敵
class NormalEnemy :public InterEnemy {
public:
	NormalEnemy();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	//キャラの状態
	enum CharaState
	{
		STATE_INTER,
		STATE_FOLLOW,
		STATE_CIRCLE,
		STATE_SIN,
	};

	//関数ポインタ
	static void(NormalEnemy::* stateTable[])();

	int _charaState = STATE_INTER;

private:
	void Inter();//待機
	void Circle();//円運動
	void Follow();//追従
	void MoveSin();//sin波
protected:

private:
	//円運動の変数
	float m_CircleAngle = 0.0f;
	float m_CircleRadius = 0.0f;
	float m_CircleSpeed = 0.0f;
	float m_CircleScale = 20.0f;
	//追従用変数
	XMFLOAT3 m_FollowVel{};
	//イージング後の位置
	XMFLOAT3 m_AfterPos = {};
	//初めの座標
	XMFLOAT3 m_StartPos = {};
};

