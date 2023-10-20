#pragma once
#include "CompositeNode.h"

namespace behaviorTree
{
    // 一つだけ実行する
    class Selector :
        public CompositeNode
    {
    public:
        /// <summary>
        /// 子ノードの実行後に呼び出し
        /// </summary>
        /// <param name="childStatus"></param>
        void OnChildExecuted(BehaviorStatus childStatus) override;
        
        bool CanExecute() override;
    };

}

