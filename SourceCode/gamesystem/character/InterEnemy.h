#pragma once
#include "ObjCommon.h"
#include <memory>
#include <IKESprite.h>

using namespace std;         //  名前空間指定
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
	unique_ptr<IKESprite> hptex;
	//キャラの状態
	enum CharaState {
		STATE_INTER,
		STATE_ATTACK,
	};
	int _charaState = STATE_INTER;
	//クールタイム
	int coolTimer = 0;
	//コマンド間隔
	int kIntervalMax = 120;
	//HP
	float m_HP = {};
	float m_MaxHP = {};
	//敵のダメージ判定のインターバル
	int m_DamegeTimer = {};
	//当たり判定の半径
	float m_Radius = 1.0f;

	//HPのUIに使う変数
	XMFLOAT2 m_HPPos = { 1000.0f,50.0f };
	XMFLOAT2 m_HPSize = {100.0f,30.0f};
	XMMATRIX m_MatView = {};
	XMMATRIX m_MatProjection = {};
	XMMATRIX m_MatPort = {};
public://getter setter
	void SetState(int state) {_charaState = state;}
	const float GetHP() { return m_HP; }
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

	void UIDraw();

	//カメラのデータをゲットする
	void GetData(const XMMATRIX& matView, const XMMATRIX& matProjection, const XMMATRIX& matPort);
private:
	void BirthParticle();
	//HPの割合を求める
	float HpPercent();
	//スプライトを敵座標に出す
	void WorldDivision();
protected:
	void Collide();
};