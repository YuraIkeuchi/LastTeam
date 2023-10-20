#pragma once
#include "CompositeNode.h"
namespace behaviorTree
{
    // 一連の流れを処理する
    class Sequencer :
        public CompositeNode
    {
    public:
        /// <summary>
        /// 子ノード実行終了時に呼び出し
        /// </summary>
        /// <param name="childStatus"></param>
        void OnChildExecuted(BehaviorStatus childStatus) override;

		bool CanExecute() override;
    };


}
