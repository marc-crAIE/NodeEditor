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
			NE_ASSERT(0, "[FAILURE] [VariableValueNode] Variable is not declared");
			context.Failure = true;
			return T{};
		}
		return variableMap[m_VarKey];
	}

private:
	uint32_t m_VarKey;
};

template<typename T>
class BinaryArithmeticOperatorValueNode : public ValueNode<T>
{
public:
	BinaryArithmeticOperatorValueNode(ValueNode<T>* a, ValueNode<T>* b, char op) :
		m_A(Scope<ValueNode<T>>(a)),
		m_B(Scope<ValueNode<T>>(b)),
		m_Op(op) {}

	virtual T GetValue(ExecuteContext& context) const override
	{
		if (!m_A || !m_B)
		{
			NE_ASSERT(0, "[Failure] [BinaryArithmeticOperatorValueNode] Binary operator has missing values!");
			context.Failure = true;
			return T{};
		}

		const T a = m_A->GetValue(context);
		const T b = m_B->GetValue(context);

		switch (m_Op)
		{
		case '+':
			return a + b;
		case '-':
			return a - b;
		case '/':
			return a / b;
		case '*':
			return a * b;
		}
		NOT_IMPLEMENTED;
		return a;
	}

private:
	Scope<ValueNode<T>> m_A;
	Scope<ValueNode<T>> m_B;
	char m_Op;
};

template<typename T>
class ComparisonValueNode : public ValueNode<bool>
{
public:
	ComparisonValueNode(ValueNode<T>* a, ValueNode<T>* b, const std::string& op) :
		m_A(a),
		m_B(b),
		m_Op(op) {}

	virtual bool GetValue(ExecuteContext& context) const
	{
		if (!m_A || !m_B)
		{
			NE_ASSERT(0, "[Failure] [ComparisonValueNode] Comparison operator has missing values!");
			context.Failure = true;
			return false;
		}

		const T a = m_A->GetValue(context);
		const T b = m_B->GetValue(context);

		if (m_Op == "==") return a == b;
		if (m_Op == "!=") return a != b;
		if (m_Op == ">") return a > b;
		if (m_Op == "<") return a < b;
		if (m_Op == ">=") return a >= b;
		if (m_Op == "<=") return a <= b;

		NOT_IMPLEMENTED;
		return false;
	}

private:
	Scope<ValueNode<T>> m_A;
	Scope<ValueNode<T>> m_B;
	std::string m_Op;
};

class BoolBinaryOperatorValueNode : public BoolValueNode
{
public:
	BoolBinaryOperatorValueNode(BoolValueNode* a, BoolValueNode* b, const std::string& op) :
		m_A(a),
		m_B(b),
		m_Op(op) {}

	virtual bool GetValue(ExecuteContext& context) const
	{
		if (!m_A || !m_B)
		{
			NE_ASSERT(0, "[Failure] [BoolBinaryOperatorValueNode] Bool operator has missing values!");
			context.Failure = true;
			return false;
		}

		const bool a = m_A->GetValue(context);
		const bool b = m_B->GetValue(context);

		if (m_Op == "AND") return a && b;
		if (m_Op == "OR") return a || b;
		if (m_Op == "XOR") return (a && !b) || (!a && b);

		NOT_IMPLEMENTED;
		return false;
	}

private:
	Scope<BoolValueNode> m_A;
	Scope<BoolValueNode> m_B;
	std::string m_Op;
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