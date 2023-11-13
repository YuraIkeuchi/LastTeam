#pragma once

#include <memory>

#include "GameObject/GameObject.h"
#include "ObjCommon.h"
#include "BehaviorTree/BehaviorTree.h"


using namespace behaviorTree;



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

protected:

	XMFLOAT3 RandPanelPos();
	
	virtual void CreateTree() {};
	struct Position
	{
		int width = -1;
		int height = -1;
	}panel_position_;

	float hitpoint_{};		// 体力
	float max_hitpoint_{};	// 最大体力

	// ビヘイビアツリー
	std::unique_ptr<SimpleBehaviorTree> behavior_tree_;


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