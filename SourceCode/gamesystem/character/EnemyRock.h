#pragma once
#pragma once
#include"InterEnemy.h"

using namespace std;         //  名前空間指定
//爆弾の敵
class EnemyRock :public InterEnemy {
public:
	EnemyRock();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//描画

		//ステータス初期化
	void InitState(const int width, const int height);
private:
	//関数ポインタ
	static void(EnemyRock::* stateTable[])();

private:
	void Inter();//生成
	void Attack();//攻撃
	void Close();//待機
private:
	int m_NowWidth = {};
	int m_NowHeight = {};
	float m_Damage = {};
	std::string StateName = "NONE";
	//上昇度
	float m_AddPower = 0.0f;
	//重力加速度
	float m_Gravity = 0.02f;
	float m_BaseScale = {};
	float m_Frame = {};
	int m_Timer = {};
};
