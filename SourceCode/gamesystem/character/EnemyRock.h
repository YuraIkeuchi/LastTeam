#pragma once
#include"InterEnemy.h"
#include "Shake.h"

using namespace std;         //  名前空間指定
//爆弾の敵
class EnemyRock :public InterEnemy {
public:
	EnemyRock();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ClearAction()override;
	void GameOverAction()override;//ゲームオーバー更新
	void ImGui_Origin()override;
	void Draw(DirectXCommon* dxCommon) override;//描画

		//ステータス初期化
	void InitState(const int width, const int height,const XMFLOAT3& pos);

private:
	//関数ポインタ
	static void(EnemyRock::* stateTable[])();

	bool RockCollide();

	void BirthParticle();

	void RockParticle();
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
	float m_BaseScale = 0.2f;
	float m_Frame = {};
	int m_Timer = {};
	int m_ParticleTimer = {};
	bool m_Hit = false;
	XMFLOAT3 m_ReturnPos = {};
	//シェイク用変数
	XMFLOAT3 m_ShakePos = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_AfterPos = {};
	unique_ptr<Shake> shake;

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

	enum RockState {
		ROCK_MAGIC,
		ROCK_FOLLOW,
		ROCK_BIRTH,
	}_RockState = ROCK_MAGIC;
};
