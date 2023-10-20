#include "Selector.h"

void behaviorTree::Selector::OnChildExecuted(BehaviorStatus childStatus)
{
	current_child_index_++;
	status_ = childStatus;
}

bool behaviorTree::Selector::CanExecute()
{
	bool execute
	{
		current_child_index_ < children_.size() &&
		status_ != BehaviorStatus::Sucess
	};

	return execute;
}
