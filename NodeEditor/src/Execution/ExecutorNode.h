#pragma once

#include <iostream>

#include "Core/Base.h"
#include "ExecuteContext.h"
#include "ValueNode.h"

class ExecutorNode
{
public:
	virtual ~ExecutorNode() {}

	virtual void Execute(ExecuteContext& context) = 0;

	void SetNextNode(ExecutorNode* node)
	{
		m_NextNode = Scope<ExecutorNode>(node);
	}

	virtual ExecutorNode* GetNextNode() const
	{
		return m_NextNode.get();
	}
private:
	Scope<ExecutorNode> m_NextNode;
};

class EmptyExecutorNode : public ExecutorNode
{
public:
	void Execute(ExecuteContext& context) override {  }
};

class NodeGraphCompiler;

template<typename T>
class PrintExecutorNode : public ExecutorNode
{
public:
	PrintExecutorNode() = default;
	PrintExecutorNode(ValueNode<T> value)
		: m_Value(value)
	{}

	void Execute(ExecuteContext& context) override;
private:
	Scope<ValueNode<T>> m_Value;

	friend NodeGraphCompiler;
};

template<typename T>
inline void PrintExecutorNode<T>::Execute(ExecuteContext& context)
{
	if (m_Value) std::cout << m_Value->GetValue(context) << std::endl;
}

class IfExecutorNode : public ExecutorNode
{
public:
	IfExecutorNode(BoolValueNode* conditionNode, ExecutorNode* elseBranch) :
		m_Condition(conditionNode),
		m_Else(elseBranch) {}

	void Execute(ExecuteContext& context) override;

	ExecutorNode* GetNextNode() const override
	{
		if (m_PassedCondition)
			return ExecutorNode::GetNextNode();
		else
			return m_Else.get();
	}

private:
	bool m_PassedCondition = false;

	Scope<BoolValueNode> m_Condition;
	Scope<ExecutorNode> m_Else;
};
