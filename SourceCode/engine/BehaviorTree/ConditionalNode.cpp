#include "ConditionalNode.h"

#include"GameObject/GameObject.h"

behaviorTree::ConditionalNode::ConditionalNode(std::function<BehaviorStatus(GameObject *)> condition)
{
	condition_ = condition;
}

behaviorTree::BehaviorStatus behaviorTree::ConditionalNode::OnUpdate()
{
	Node::OnUpdate();
	status_ = condition_(owner_);
	return status_;

}
