#pragma once
#include "ObjCommon.h"
#include "AttackArea.h"
#include "PredictArea.h"
#include "DrawNumber.h"
#include <memory>
#include <IKESprite.h>
#include <array>
#include "IKETexture.h"
#include "Player.h"
#include "DrawDamageNumber.h"
#include "DrawHealNumber.h"
#include "DrawPoisonNumber.h"

using namespace std;         //  名前空間指定
//キャラの状態
enum CharaState {
	STATE_INTER,
	STATE_ATTACK,
	STATE_SPECIAL,
};

//敵基底
class InterEnemy :
	public ObjCommon
{
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
	//三桁表示まで
	static const int NUMBER_MAX = 3;
	static const int DAMAGE_MAX = 2;
protected:
	array<unique_ptr<DrawNumber>, NUMBER_MAX> _drawnumber;
	std::vector<unique_ptr<DrawDamageNumber>> _damagenumber;
	std::vector<unique_ptr<DrawHealNumber>> _healnumber;
	array<unique_ptr<DrawPoisonNumber>, NUMBER_MAX> _drawPoisonnumber;
	unique_ptr<IKESprite> poisonState;

	unique_ptr<IKETexture> _charge;
	//unique_ptr<IKETexture> shadow_tex;
	static Player* player;
	//桁数
	enum DightType {
		FIRST_DIGHT,
		SECOND_DIGHT,
		THIRD_DIGHT
	};
	//HPの表示
	unique_ptr<IKESprite> hptex;
	
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
	float m_Radius = 0.5f;

	struct PanelProb {
		int AttackProb = 50;
		int SkillProb = 25;
		int GuardProb = 25;
	};

	//現在のマス番号
	int m_NowWidth = {};
	int m_NowHeight = {};

	//HPのUIに使う変数
	XMFLOAT2 m_HPPos = { 1000.0f,50.0f };
	XMFLOAT2 m_HPSize = {100.0f,15.0f};
	XMMATRIX m_MatView = {};
	XMMATRIX m_MatProjection = {};
	XMMATRIX m_MatPort = {};

	//数値化したHP表示のための変数
	array<int, NUMBER_MAX> m_DigitNumber;
	int m_InterHP = {};//整数にしたHP

	//数値化したPOISON表示のための変数
	array<int, NUMBER_MAX> m_PoisonTokenNum;

	bool m_Poison = false;
	bool m_PoisonLong = false;
	bool m_IsVenom = false;
	int m_PoisonToken = 0;
	bool m_IsDrainUp = false;
	int m_PoisonTimer = {};
	bool m_Alive = true;
	bool m_Death = false;

	bool m_CheckPanel = false;

	string m_EnemyTag = "Normal";
	bool m_LastEnemy = false;

	//影の変数
	XMFLOAT3 m_ShadowPos = {};
	XMFLOAT3 m_ShadowScale = {};
	float m_ChargeScale = {};
	bool m_Jump = false;
	//上昇度
	float m_AddPower = 0.0f;
	//重力加速度
	float m_Gravity = 0.02f;
	bool m_Rot = false;
	float m_AttackFrame = {};
	//予測エリア
	std::unique_ptr<PredictArea> predictarea;

	//ダメージ関係
	bool m_Damege = false;
	int m_DamageTimer = {};
	int m_FlashCount = {};

	float m_OverFrame = {};
public://getter setter
	void SetState(int state) { _charaState = state; }
	int GetState() { return _charaState; };
	
	void SetPoizonLong(bool isPoison) { m_PoisonLong = isPoison; }
	void SetPoizonVenom(bool isPoison) { m_IsVenom = isPoison; }
	void SetDrainUp(bool IsDrainUp) { m_IsDrainUp = IsDrainUp; }
	void SetLastEnemy(bool LastEnemy) { m_LastEnemy = LastEnemy; }
	static void SetPlayer(Player* player) { InterEnemy::player = player; }

	const float GetHP() { return m_HP; }
	const bool GetAlive() { return m_Alive; }
	const bool GetDeath() { return m_Death; }
	const bool GetLastEnemy() { return m_LastEnemy; }

	void SimpleDamege(float damage = 3.f);
	void SimpleHeal(float heal = 5.f);
public:
	//virtual ~InterEnemy() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual bool Initialize()override;
	void BaseInitialize(IKEModel* _model);
	void SkipInitialize();
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
	XMFLOAT3 SetPannelPos(int width, int height);

	void AwakeUpdate();

	void DeathUpdate();
private:
	void BirthParticle();
	//HPの割合を求める
	float HpPercent();
	//スプライトを敵座標に出す
	void WorldDivision();
	//UIのためのHPの管理
	void HPManage();
	void BirthPoisonParticle();
	void BirthHealParticle();
	void BirthDamage(const float Damage);//
	void BirthHeal(const float Heal);
	//ダメージの更新
	void DamageUpdate();
	void BirthHealNumber(const float heal);
protected:
	void Collide(vector<unique_ptr<AttackArea>>& area);
	//毒の状態
	void PoisonState();
};