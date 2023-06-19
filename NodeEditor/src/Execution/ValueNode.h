#pragma once

#include "Core/Base.h"
#include "Util/Hash.h"
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

template<typename T>
class VariableValueNode : public ValueNode<T>
{
public:
	VariableValueNode(const std::string& varName) 
		: m_VarKey(Hash::Crc32(varName)) 
	{}

	virtual T GetValue(ExecuteContext& context) const override
	{
		auto& variableMap = context.Variables.GetMapFromType<T>();
		if (variableMap.find(m_VarKey) == variableMap.end())
		{
			NE_ASSERT(0, "[FAILURE] [VariableValueNode] Variable not declared");
			context.Failure = true;
			return T{};
		}
		return variableMap[m_VarKey];
	}

private:
	uint32_t m_VarKey;
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