#pragma once
#include "ObjCommon.h"

enum PolterType {
	TYPE_FOLLOW,
	TYPE_BOUND,
};
//ボスが生む攻撃音符クラス
class EnemyBullet :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	EnemyBullet();

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

private://ステート
	static void (EnemyBullet::* stateTable[])();
private:
	void Throw();//投げる

public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }

	void SetAlive(const bool Alive) { m_Alive = Alive; }
	void SetPolterType(const int PolterType) { m_PolterType = PolterType; }

	void SetTargetTimer(const int TargetTimer) { m_TargetTimer = TargetTimer; }

private:
	unique_ptr<IKEObject3d> m_Pannel = nullptr;
	//現在のマス番号
	int m_NowWidth = {};
	int m_NowHeight = {};
	bool m_Alive = true;//生存フラグ
	int m_PolterType = TYPE_FOLLOW;

	int m_TargetTimer = {};//出現時間の目標

	int m_ThrowTimer = {};

	enum ThrowType {
		THROW_SET,
		THROW_INTER,
		THROW_PLAY,
	};

	int m_ThrowType = THROW_SET;
	XMFLOAT3 m_AfterPos = {};
	float m_Frame = {};

	float m_BaseScale = {};
	XMFLOAT2 m_Angle = {};

	float m_AddSpeed = {};//加速度

	int m_AliveTimer = {};

	//投げる方向
	enum ThrowDir {
		DIR_STRAIGHT,//まっすぐ
		DIR_SLASHUP,//斜め上
		DIR_SLASHDOWN,//斜めした
	};
	int m_ThrowDir = {};

	XMFLOAT3 m_PanelPos = {};
};
