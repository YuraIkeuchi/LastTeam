#pragma once
#include <vector>
#include <string>
#include <stack>
#include "GameObject/GameObject.h"

#include "Property.h"

#include "Node.h"
#include "ActionNode.h"
#include "ConditionalNode.h"
#include "CompositeNode.h"
#include "Repeater.h"
#include "Selector.h"
#include "Sequencer.h"

namespace behaviorTree
{
	class SimpleBehaviorTree
	{
	public:
		SimpleBehaviorTree(GameObject* owner);

		/// <summary>
		/// ルートノード設定
		/// </summary>
		/// <param name="node"></param>
		void SetRootNode(Node *node);

		void Start();
		void Update();


		yEngine::Property<std::vector<std::unique_ptr<Node>>> AllNode
		{
			&node_container_,
			yEngine::AccessorType::ReadOnly
		};
		yEngine::Property<std::stack<int>> ActiveStack
		{
			&active_stack_,
			yEngine::AccessorType::ReadOnly
		};

		/// <summary>
		///	再評価用の情報
		/// </summary>
		class ConditionalReevaluate
		{
		public:
			ConditionalReevaluate() {};
			void Initialize(int i, BehaviorStatus status, int stack, int composite);
			yEngine::Property<int> Index
			{
				&index_,
				yEngine::AccessorType::AllAccess
			};
			yEngine::Property<BehaviorStatus> Status
			{
				&status_,
				yEngine::AccessorType::AllAccess
			};
			yEngine::Property<int> CompositeIndex
			{
				&composite_index_,
				yEngine::AccessorType::AllAccess
			};
			yEngine::Property<int> StackIndex
			{
				&stack_index_,
				yEngine::AccessorType::AllAccess
			};
		private:
			int index_{ -1 };
			BehaviorStatus status_;
			int composite_index_;
			int stack_index_;
		};

	private:
		/// <summary>
		/// アクティブなノードとしてスタックに追加
		/// </summary>
		/// <param name="node"></param>
		void PushNode(Node *node);
		/// <summary>
		/// ノードをスタックから取り除く
		/// </summary>
		void PopNode();
		/// <summary>
		/// 二つのノードの共通の祖先を見つける
		/// </summary>
		/// <param name="node1">1つ目のノードID</param>
		/// <param name="node2">2つ目のノードID</param>
		/// <returns></returns>
		int CommonAncestorNode(int node1, int node2);
		/// <summary>
		/// 対象ノードの軌道
		/// </summary>
		/// <param name="node"></param>
		void CallOnAwake(Node *node);

		// ノードの再評価
		int ReevaluateConditionalTasks();

		BehaviorStatus Execute(Node *node);

		GameObject* owner_;					// ツリーを保持しているオブジェクト
		Node *root_node_;	// 一番上にあるノード

		std::vector<std::unique_ptr<Node>> node_container_;	//全ノード
		std::vector<ConditionalReevaluate*> reevaluate_container_;	// 再評価用コンテナ

		std::stack<int> active_stack_;		// 実行中ノードを保持するスタック
		
		bool is_completed_{ false };
		
		int active_node_index_{ -1 };


	};
};

