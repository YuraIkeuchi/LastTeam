#include "BehaviorTree.h"
#include "CompositeNode.h"


behaviorTree::SimpleBehaviorTree::SimpleBehaviorTree(GameObject* owner):
	owner_(owner)
{
}

void behaviorTree::SimpleBehaviorTree::SetRootNode(Node *node)
{
	root_node_ = std::move(node);
}

void behaviorTree::SimpleBehaviorTree::Start()
{
	CallOnAwake(root_node_);
}

void behaviorTree::SimpleBehaviorTree::Update()
{
	if (is_completed_)
	{
		return;
	}

	int abort_index = ReevaluateConditionalTasks();
	if (abort_index != -1)
	{
		// 中断したノードと現在実行中のノードの共通の祖先を探索
		int ca_index = CommonAncestorNode(abort_index, active_node_index_);
		
		Node *active_node = node_container_[active_node_index_].get();
		active_node->OnAbort();

		while (active_stack_.size() != 0)
		{
			PopNode();
			active_node = node_container_[active_node_index_].get();
			active_node->OnAbort();

			if (active_node_index_ == ca_index)
			{
				break;
			}

			ConditionalReevaluate *cr{nullptr};
			int i = 0;
			for (auto &r : reevaluate_container_)
			{
				if (r->Index == active_node_index_)
				{
					cr = r;
					break;
				}
				++i;
			}
			if (cr != nullptr)
			{
				reevaluate_container_.erase(reevaluate_container_.begin() + i);
			}
		}
	}

	BehaviorStatus status = BehaviorStatus::Inactive;
	if (active_node_index_ == -1)
	{
		status = Execute(root_node_);
	}
	else
	{
		Node *node = node_container_[active_node_index_].get();
		status = Execute(node);
	}

	if (status == BehaviorStatus::Completed)
	{
		is_completed_ = true;
	}
}

void behaviorTree::SimpleBehaviorTree::PushNode(Node *node)
{
	if (active_stack_.size() == 0 ||
		active_stack_.top() != node->Index)
	{
		active_stack_.push(node->Index);
		active_node_index_ = active_stack_.top();
	}
}

void behaviorTree::SimpleBehaviorTree::PopNode()
{
	active_stack_.pop();
	active_node_index_ = active_stack_.top();
}

int behaviorTree::SimpleBehaviorTree::CommonAncestorNode(int node1, int node2)
{
	std::vector<int> parent_nodes;

	// 祖先のインデックスをリスト化
	Node *parent1 = node_container_[node1]->ParentNode;
	while (parent1 != nullptr)
	{
		parent_nodes.emplace_back(parent1->Index);
		parent1 = parent1->ParentNode;
	}

	Node *parent2 = node_container_[node2]->ParentNode;
	int num = parent2->Index;
	while (
		std::find_if(
			parent_nodes.begin(),
			parent_nodes.end(),
			[&](int &parent_index)
			{
				return parent_index == num;
			})
		== parent_nodes.end()
		)
	{
		parent2 = parent2->ParentNode;
		num = parent2->Index;
	}

	return num;
}

void behaviorTree::SimpleBehaviorTree::CallOnAwake(Node *node)
{
	node->Index = static_cast<int>(node_container_.size());
	node_container_.emplace_back(std::move(node));

	node->Owner = owner_;

	// ノード起動
	node->OnAwake();

	// CompositeNodeなら再帰起動
	CompositeNode *cnode = dynamic_cast<CompositeNode*>(node);
	if (cnode != nullptr)
	{
		for (auto &child : cnode->Children.r_)
		{
			CallOnAwake(child.get());
		}
	}
}

// ノードの再評価
int behaviorTree::SimpleBehaviorTree::ReevaluateConditionalTasks()
{
	for (int i = 0; i < reevaluate_container_.size(); ++i)
	{
		const ConditionalReevaluate cr = *reevaluate_container_[i];
		const BehaviorStatus &status = node_container_[cr.Index]->OnUpdate();
		
		// 前回と変化があれば先祖までさかのぼり処理停止
		if (cr.Status != status)
		{
			CompositeNode *cnode = dynamic_cast<CompositeNode *>(node_container_[cr.CompositeIndex].get());
			if (cnode != nullptr)
			{
				cnode->OnConditionalAbort(cr.Index);
			}
			reevaluate_container_.erase(reevaluate_container_.begin() + i);
			return cr.Index;
		}
	}

	return -1;
}

behaviorTree::BehaviorStatus behaviorTree::SimpleBehaviorTree::Execute(Node *node)
{
	// 実行中ノードをActive Stackに追加
	PushNode(node);

	CompositeNode *cnode = dynamic_cast<CompositeNode *>(node);
	if (cnode != nullptr)
	{

		while (cnode->CanExecute())
		{
			Node *child = cnode->Children.r_[cnode->CurrentChildIndex].get();
			BehaviorStatus child_status = Execute(child);

			// 再評価が必要ならリストに追加
			if (cnode->NeedsConditionalAbort)
			{
				if (dynamic_cast<ConditionalNode *>(child) != nullptr)
				{
					ConditionalReevaluate *add_date = reevaluate_container_.emplace_back(std::move(new ConditionalReevaluate()));
					add_date->Index = child->Index.r_;
					add_date->CompositeIndex = cnode->Index.r_;
					add_date->Status = child_status;
				}
			}

			if (child_status == BehaviorStatus::Runnning)
			{
				return BehaviorStatus::Runnning;
			}

			cnode->OnChildExecuted(child_status);
		

		}

		if (cnode->Status == BehaviorStatus::Runnning)
		{
			node->OnEnd();
			PopNode();
		}
		
		return cnode->Status;
	}
	else
	{
		BehaviorStatus status = node->OnUpdate();

		if (status != BehaviorStatus::Runnning)
		{
			node->OnEnd();
			PopNode();
		}

		return status;
	}
}

void behaviorTree::SimpleBehaviorTree::ConditionalReevaluate::Initialize(int i, BehaviorStatus status, int stack, int composite)
{
	index_ = i;
	status_ = status;
	stack_index_ = stack;
	composite_index_ = composite;
}
