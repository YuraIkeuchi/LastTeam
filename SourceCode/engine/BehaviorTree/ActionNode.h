#pragma once
#include <functional>

#include "Node.h"

namespace behaviorTree
{
    /// <summary>
    /// アクションを行うノード
    /// </summary>
    class ActionNode :
        public Node
    {
    public:
        ActionNode(std::function<BehaviorStatus(GameObject*)> action);
        BehaviorStatus OnUpdate() override;

    private:
        // 実行内容
        std::function<BehaviorStatus(GameObject *)> action_;
    };

}
