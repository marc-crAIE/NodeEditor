#include "nepch.h"
#include "ExecutorNode.h"

void IfExecutorNode::Execute(ExecuteContext& context)
{
	m_PassedCondition = m_Condition->GetValue(context);
}
