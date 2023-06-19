#pragma once

#include "Core/Base.h"
#include "ExecuteContext.h"

template<typename T>
class ValueNode
{
public:
	ValueNode() = default;

	virtual T GetValue(ExecuteContext& context) const = 0;
};

using BoolValueNode = ValueNode<bool>;
using FloatValueNode = ValueNode<float>;
using Float2ValueNode = ValueNode<Float2>;
using Float3ValueNode = ValueNode<Float3>;
using Float4ValueNode = ValueNode<Float4>;

template<typename T>
class ConstantValueNode : public ValueNode<T>
{
public:
	ConstantValueNode(const T& value) :
		m_Value(value) {}

	virtual T GetValue(ExecuteContext& context) const override { return m_Value; }
private:
	T m_Value;
};

template<typename U, typename T>
class SplitVectorValueNode : public ValueNode<T>
{
public:
	SplitVectorValueNode(ValueNode<U>* value, unsigned vecIndex) 
		: m_Value(value), m_VecIndex(vecIndex)
	{
		NE_ASSERT(vecIndex < 4 && vecIndex >= 0);
	}

	virtual T GetValue(ExecuteContext& context) const override
	{
		if (!m_Value)
		{
			NE_ASSERT(0, "[FAILURE] [SplitVectorValueNode] Missing input!");
			context.Failure = true;
			return T{};
		}
		return m_Value->GetValue(context)[m_VecIndex];
	}

private:
	unsigned m_VecIndex;
	Scope<ValueNode<U>> m_Value;
};