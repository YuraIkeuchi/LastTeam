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
	void Particle();
	void Follow();//追従
public:

public://getter setter
public:
	//gettersetter
	const bool& GetAlive() { return m_Alive; }

	void SetAlive(const bool Alive) { m_Alive = Alive; }
	void SetPolterType(const int PolterType) { m_PolterType = PolterType; }

	void SetTargetTimer(const int TargetTimer) { m_TargetTimer = TargetTimer; }
	void SetCircleSpeed(const float CircleSpeed) { m_CircleSpeed = CircleSpeed; }

	void SetBasePos(const XMFLOAT3& BasePos) { m_BasePos = BasePos; }
private:
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

	double m_SpeedX = {};
	double m_SpeedZ = {};

	//円運動
	XMFLOAT3 m_BasePos = {};
	float m_CircleScale = 1.0f;
	float m_CircleSpeed = {};

	XMFLOAT2 m_Angle = {};

	float m_AddSpeed = {};//加速度

	int m_AliveTimer = {};

};
