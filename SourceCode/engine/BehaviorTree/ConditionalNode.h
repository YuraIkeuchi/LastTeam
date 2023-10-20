#pragma once
#include "Node.h"

namespace behaviorTree
{

    class ConditionalNode :
        public Node
    {
    public:
        ConditionalNode(std::function<BehaviorStatus(GameObject *)> condition);
        
        BehaviorStatus OnUpdate() override;
    private:
        std::function<BehaviorStatus(GameObject *)> condition_;

    };
}

