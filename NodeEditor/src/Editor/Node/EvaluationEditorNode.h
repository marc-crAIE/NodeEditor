#pragma once

#include "EditorNode.h"
#include "Editor/Drawing/EditorContextMenu.h"

class EvaluationEditorNode : public EditorNode
{
public:
	using EditorNode::EditorNode;
};

class BoolEditorNode : public EvaluationEditorNode
{
public:
	BoolEditorNode() 
		: EvaluationEditorNode("Bool", EditorNodeType::Bool)
	{
		AddPin(EditorNodePin::CreateOutputPin("", PinType::Bool));
	}

	bool GetValue() const { return m_Value; }
protected:
	virtual void RenderContent() override;
private:
	bool m_Value = false;
};

class FloatNEditorNode : public EvaluationEditorNode
{
public:
	FloatNEditorNode(uint32_t numFloats, EditorNodeType nodeType, PinType pinType)
		: EvaluationEditorNode("Float" + (numFloats == 1 ? "" : std::to_string(numFloats)), nodeType), m_NumValues(numFloats)
	{
		AddPin(EditorNodePin::CreateOutputPin("", pinType));

		for (uint32_t i = 0; i < 4; i++)
			m_Values[i] = 0.0f;
	}

	float GetFloat() const { NE_ASSERT(m_NumValues == 1); return m_Values[0]; }
	Float2 GetFloat2() const { NE_ASSERT(m_NumValues == 2); return Float2(m_Values[0], m_Values[1]); }
	Float3 GetFloat3() const { NE_ASSERT(m_NumValues == 3); return Float3{ m_Values[0], m_Values[1], m_Values[2] }; }
	Float4 GetFloat4() const { NE_ASSERT(m_NumValues == 4); return Float4{ m_Values[0], m_Values[1], m_Values[2], m_Values[3] }; }
protected:
	virtual void RenderContent() override;
private:
	uint32_t m_NumValues;
	float m_Values[4];
};

template<EditorNodeType nodeType, PinType pinType, uint32_t numFloats>
class FloatNEditorNodeT : public FloatNEditorNode
{
public:
	FloatNEditorNodeT()
		: FloatNEditorNode(numFloats, nodeType, pinType) 
	{}

	static_assert(numFloats <= 4 && numFloats > 0);
};


class SplitVectorEditorNode : public EvaluationEditorNode
{
public:
	SplitVectorEditorNode(EditorNodeType nodeType, PinType inputPinType, PinType outputPinType, unsigned numOutputs) :
		EvaluationEditorNode("Split " + EditorNodePin::ToString(inputPinType), nodeType),
		m_NumOutputs(numOutputs)
	{
		static std::vector<std::string> outputLabels = { "x", "y", "z", "w" };
		m_InputVectorPin = AddPin(EditorNodePin::CreateInputPin("", inputPinType));
		for (unsigned i = 0; i < numOutputs; i++)
			m_OutputPins[i] = AddPin(EditorNodePin::CreateOutputPin(outputLabels[i], outputPinType));
	}

	unsigned GetOutputPinIndex(PinID pinID)
	{
		for (unsigned i = 0; i < m_NumOutputs; i++)
		{
			if (pinID == GetPins()[m_OutputPins[i]].ID)
				return i;
		}
		NE_ASSERT(0);
		return 0;
	}

	const EditorNodePin& GetInputVectorPin() const { return GetPins()[m_InputVectorPin]; }
private:
	unsigned m_NumOutputs;
	unsigned m_InputVectorPin;
	unsigned m_OutputPins[4];
};

template<EditorNodeType nodeType, PinType inputPinType, PinType outputPinType, unsigned numOutputs>
class SplitVectorEditorNodeT : public SplitVectorEditorNode
{
public:
	SplitVectorEditorNodeT() :
		SplitVectorEditorNode(nodeType, inputPinType, outputPinType, numOutputs)
	{ }

	static_assert(numOutputs <= 4 && numOutputs > 0);
};

using FloatEditorNode = FloatNEditorNodeT<EditorNodeType::Float, PinType::Float, 1>;

using Float2EditorNode = FloatNEditorNodeT<EditorNodeType::Float2, PinType::Float2, 2>;
using SplitFloat2EditorNode = SplitVectorEditorNodeT<EditorNodeType::SplitFloat2, PinType::Float2, PinType::Float, 2>;

using Float3EditorNode = FloatNEditorNodeT<EditorNodeType::Float3, PinType::Float3, 3>;
using SplitFloat3EditorNode = SplitVectorEditorNodeT<EditorNodeType::SplitFloat3, PinType::Float3, PinType::Float, 3>;

using Float4EditorNode = FloatNEditorNodeT<EditorNodeType::Float4, PinType::Float4, 4>;
using SplitFloat4EditorNode = SplitVectorEditorNodeT<EditorNodeType::SplitFloat4, PinType::Float4, PinType::Float, 4>;