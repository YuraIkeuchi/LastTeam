#include "ActionNode.h"
#include "GameObject/GameObject.h"

behaviorTree::ActionNode::ActionNode(std::function<BehaviorStatus(GameObject *)> action)
{
	action_ = action;
}

behaviorTree::BehaviorStatus behaviorTree::ActionNode::OnUpdate()
{
	Node::OnUpdate();
	status_ = action_(owner_);
	return status_;

}
