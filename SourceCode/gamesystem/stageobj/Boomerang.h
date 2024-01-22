#pragma once
#include "ObjCommon.h"
#include "IKETexture.h"
#include "Player.h"
#include "PredictArea.h"
//ブーメランの攻撃(敵)
class Boomerang :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	Boomerang();

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

	void InitState(const XMFLOAT3& pos);
private://ステート
	static void (Boomerang::* stateTable[])();
private:
	void Throw();//投げる

public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }

	void SetAlive(const bool Alive) { m_Alive = Alive; }

	void SetTargetTimer(const int TargetTimer) { m_TargetTimer = TargetTimer; }
	void SetPlayer(Player* player) { this->player = player; }

private:
	unique_ptr<PredictArea> predictArea;
	unique_ptr<IKETexture> dir_tex;
	Player* player;
	//パネル
	struct Panel {
		unique_ptr<IKETexture> tex = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		bool predict = false;
	};
	Panel panels;
	//現在のマス番号
	int m_NowWidth = {};
	int m_NowHeight = {};
	bool m_Alive = false;//生存フラグ

	int m_TargetTimer = {};//出現時間の目標

	int m_ThrowTimer = {};

	enum ThrowType {
		THROW_SET,
		THROW_PLAY,
	};

	int m_ThrowType = THROW_SET;
	XMFLOAT3 m_AfterPos = {};
	float m_Frame = {};

	float m_BaseScale = {};
	XMFLOAT2 m_Angle = {};

	float m_AddSpeed = {};//加速度

	int m_AliveTimer = {};

	float m_Damage = {};
	
	XMFLOAT3 m_TargetPos = {};

	enum MoveDir {
		MOVE_STRAIGHT,
		MOVE_UP,
		MOVE_DOWN,
		MOVE_RETURN,
	}_MoveDir = MOVE_UP;

	float m_Length = {};
	float m_Speed = {};

	XMFLOAT3 m_DirRot = { 90.0f,270.0f,0.0f };
	float m_RotFrame = {};
	bool m_Hit = false;
	int m_HitTimer = {};
	float predictFrame = 0.f;
	int nextPredict = 0;
	int m_OldWidth = m_NowWidth;
	int m_OldHeight = m_NowHeight;
};
