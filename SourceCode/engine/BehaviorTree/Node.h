#pragma once
#include "Property.h"
#include "GameObject/GameObject.h"

namespace behaviorTree
{
	enum class BehaviorStatus
	{
		Inactive,		// 非アクティブ
		Sucess,		// 成功
		Failure,		// 失敗
		Runnning,		// 実行中
		Completed		// 完了
	};

	/// <summary>
	/// ノードの基底クラス
	/// </summary>
	class Node
	{
	public:
		Node();

		virtual void OnAwake() {};
		virtual void OnStart();
		virtual BehaviorStatus OnUpdate();
		virtual void OnEnd();
		virtual void OnAbort();

		/// <summary>
		/// 子ノードを追加
		/// </summary>
		/// <param name="chuld">追加する子ノード</param>
		virtual void AddNode(Node *child) {};
		/// <summary>
		/// 子ノードを複数追加
		/// </summary>
		/// <param name="chuld">追加する子ノード</param>
		/// 
		/// 

		// アクセッサ
		yEngine::Property<GameObject*> Owner
		{
			&owner_,
			yEngine::AccessorType::AllAccess
			
		};
		yEngine::Property<int> Index
		{
			&index_,
			yEngine::AccessorType::AllAccess
		};
		yEngine::Property<Node*> ParentNode
		{
			&parent_node_,
			yEngine::AccessorType::AllAccess
		};
		yEngine::Property<std::string> Name
		{
			&name_,
			yEngine::AccessorType::AllAccess
		};
		yEngine::Property<BehaviorStatus> Status
		{
			&status_,
			yEngine::AccessorType::ReadOnly
		};

	protected:
		GameObject* owner_;
		Node *parent_node_{ nullptr };
		std::string name_;
		int index_{ -1 };
		BehaviorStatus status_{BehaviorStatus::Inactive};
	};
}
