#include "CompositeNode.h"

bool behaviorTree::CompositeNode::CanExecute()
{
    return true;
}

void behaviorTree::CompositeNode::OnConditionalAbort(int childNodeIndex)
{
    OnEnd();
    current_child_index_ = 0;
}

void behaviorTree::CompositeNode::OnChildExecuted(BehaviorStatus childStatus)
{
    // 全ての子ノードが実行されていなければreturn
    if (current_child_index_ < children_.size())
    {
        return;

    }
    if (NeedsConditionalAbort && has_conditional_node_)
    {
        return;

    }
    if (childStatus == BehaviorStatus::Sucess ||
        childStatus == BehaviorStatus::Completed)
    {
        status_ = BehaviorStatus::Completed;
    }
}

void behaviorTree::CompositeNode::OnAwake()
{
    current_child_index_ = 0;

    for (auto &child : children_)
    {
        if (dynamic_cast<CompositeNode*>(child.get()) != nullptr)
        {
            has_conditional_node_ = true;
            return;
        }
    }
}

void behaviorTree::CompositeNode::OnStart()
{
}

void behaviorTree::CompositeNode::OnAbort()
{
    Node::OnAbort();
    current_child_index_ = 0;
}

behaviorTree::BehaviorStatus behaviorTree::CompositeNode::OnUpdate()
{
    Node::OnUpdate();
    return status_;
}

void behaviorTree::CompositeNode::AddNode(Node *child)
{
    if (
        std::find_if(
            children_.begin(),
            children_.end(),
            [&](std::unique_ptr<Node> &node)
            {
                return node.get() == child;
            }
        ) == children_.end())
    {
        child->ParentNode = this;
        children_.emplace_back(std::move(child));
    }
}