#pragma once

#include <memory>

#include "GameObject/GameObject.h"
#include "ObjCommon.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DrawNumber.h"

using namespace behaviorTree;

enum class Debuff
{
	kNone = 0,
	kPoison = 1 << 1,
};

class BaseEnemy:
	public ObjCommon,
	public GameObject
{
public:
	BaseEnemy();
	//初期化
	bool Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw(DirectXCommon* dxCommon) override;

	//UI用
	void UIDraw() override;
	//ImGui
	void ImGuiDraw() override;

	XMFLOAT3 SetPannelPos(int width, int height);

	const float GetHP() { return hitpoint_; }

	bool GetAlive() { return is_alive_; }


	void SetPoizonLong(bool isPoison) { m_PoisonLong = isPoison; }
	void SetPoizonVenom(bool isPoison) { m_IsVenom = isPoison; }
protected:

	// ランダムなパネルを取得
	XMFLOAT3 RandPanelPos();
	
	//HPの表示
	unique_ptr<IKESprite> hptex_;
	static const int kNumberMax{ 3 };	// 桁数
	array<unique_ptr<DrawNumber>, kNumberMax> draw_number_;
	static const int kHpDrawNumMax{ 3 };
	array<int, kHpDrawNumMax> digit_number_;
	

	//桁数
	enum DightType {
		kFirstDight,
		kSecondDight,
		kThirdDight
	};
	virtual void CreateTree() {};
	struct Position
	{
		int width = -1;
		int height = -1;
	}panel_position_;

	float hitpoint_{};		// 体力
	float max_hitpoint_{};	// 最大体力
	int inter_hitpoint_{};	// HP（整数）
	bool is_alive_{ false };	// 生存確認

	// ビヘイビアツリー
	std::unique_ptr<SimpleBehaviorTree> behavior_tree_;

	bool m_PoisonLong = false;
	bool m_IsVenom = false;
	int m_PoisonTimer = {};

	// デバフ
	int debuff_ { static_cast<int>(Debuff::kNone)};


	// ポイズンパーティクル
	void BirthPoisonParticle();

private:

	//HPのUIに使う変数
	XMFLOAT2 m_HPPos = { 1000.0f,50.0f };
	XMFLOAT2 m_HPSize = { 100.0f,15.0f };
	XMMATRIX m_MatView = {};
	XMMATRIX m_MatProjection = {};
	XMMATRIX m_MatPort = {};
	void WorldDivision();
	float HpPercent();
};

// デバック用エネミークラスです
class TestEnemy :
	public BaseEnemy
{
public:
	// コンストラクタ
	TestEnemy();

	//初期化
	bool Initialize();
	//UI用
	void UIDraw() override;
	//ImGui
	void ImGuiDraw() override;

protected:
	void CreateTree()override;
};