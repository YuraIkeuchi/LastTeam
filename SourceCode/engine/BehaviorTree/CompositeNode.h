#pragma once
#include "Node.h"

namespace behaviorTree
{
	/// <summary>
	/// 条件分岐用ノード
	/// </summary>
	class CompositeNode :
		public Node
	{
	public:
		CompositeNode() {};

		virtual bool CanExecute();
		/// <summary>
		/// 中断時呼び出し
		/// </summary>
		/// <param name="childNodeIndex">中断を呼び出した子ノードの全体でのインデックス</param>
		virtual void OnConditionalAbort(int childNodeIndex);
		/// <summary>
		/// 子ノードの実行が終わった際に呼び出す
		/// すべてのノードが実行完了かつ成功の場合はコンプリートにする
		/// </summary>
		/// <param name="childStatus"></param>
		virtual void OnChildExecuted(BehaviorStatus childStatus);

		/// <summary>
		/// 全ての子ノードのOnAwake呼び出し
		/// </summary>
		void OnAwake() override;
		/// <summary>
		/// スタート呼び出し
		/// </summary>
		void OnStart() override;
		void OnAbort() override;
		BehaviorStatus OnUpdate() override;

		void AddNode(Node *child) override;

		// アクセッサ
		yEngine::Property<bool> NeedsConditionalAbort
		{
			&node_conditional_abort_,
			yEngine::AccessorType::AllAccess
		};
		yEngine::Property<int> CurrentChildIndex
		{
			&current_child_index_,
			yEngine::AccessorType::ReadOnly
		};
		yEngine::Property<std::vector<std::unique_ptr<Node>>> Children
		{
			&children_,
			yEngine::AccessorType::ReadOnly

		};


	protected:
		// 現在のインデックス
		int current_child_index_{ 0 };
		// 分岐先の子ノード
		std::vector<std::unique_ptr<Node>> children_;

	private:
		bool node_conditional_abort_{ false };
		bool has_conditional_node_{ false };

	};

}
