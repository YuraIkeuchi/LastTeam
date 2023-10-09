#pragma once
#include "ObjCommon.h"


//キャラの状態
enum CharaState {
	STATE_INTER,
	STATE_ATTACK,
	STATE_STANDBY,
};
//敵基底
class InterEnemy :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
protected:
	XMFLOAT3 randPanelPos();

protected:

	int _charaState = STATE_INTER;
	//クールタイム
	int coolTimer = 0;
	//コマンド間隔
	int kIntervalMax = 120;
	//HP
	float m_HP = {};
	//敵のダメージ判定のインターバル
	int m_DamegeTimer = {};
	//当たり判定の半径
	float m_Radius = 1.0f;

	struct PanelProb {
		int AttackProb = 50;
		int SkillProb = 25;
		int GuardProb = 25;

	};

public://getter setter
	void SetState(int state) { _charaState = state; }
	int GetState() { return _charaState; };
public:
	//virtual ~InterEnemy() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual bool Initialize()override;
	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize() = 0;
	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update();

	virtual void Action() = 0;//敵の行動

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(DirectXCommon* dxCommon)override;

	void ImGuiDraw();

	virtual void ImGui_Origin() = 0;

private:
	void BirthParticle();
protected:
	void Collide();
};