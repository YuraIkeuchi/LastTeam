#pragma once
#include "ObjCommon.h"
#include "Player.h"
#include "Explosion.h"
//爆弾の攻撃(敵)
class LastBomb :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	LastBomb();

	bool Initialize() override;//初期化
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

	bool Collide();	//当たり判定

	void InitState(const XMFLOAT3& pos, const int width, const int height);

	void BirthExplosion();
	void DeleteBomb();

private://ステート
	static void (LastBomb::* stateTable[])();
private:
	void Throw();//投げる

public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }

	void SetAlive(const bool Alive) { m_Alive = Alive; }

	void SetTargetTimer(const int TargetTimer) { m_TargetTimer = TargetTimer; }
	void SetBirthCount(const int BirthCount) { m_BirthCount = BirthCount; }
	void SetPlayer(Player* player) { this->player = player; }

private:
	Player* player;
	//現在のマス番号
	int m_NowWidth = {};
	int m_NowHeight = {};
	bool m_Alive = true;//生存フラグ

	int m_TargetTimer = {};//出現時間の目標

	int m_ThrowTimer = {};

	enum BombState {
		BOMB_SET,
		BOMB_THROW,
	}_BombState = BOMB_SET;

	XMFLOAT3 m_AfterPos = {};
	float m_Frame = {};

	float m_BaseScale = {};
	
	float m_Damage = {};
	bool m_Hit = false;

	float m_AddPower = {};
	float m_Gravity = 0.02f;
	bool m_Jump = false;

	int m_BirthCount = {};

	bool m_Attack[4][4] = {false};

	// 攻撃エリア
	std::vector<unique_ptr<Explosion>> explosion;
};
