#include "nepch.h"
#include "PinEvaluator.h"

static EditorNodePin GetOutputPinIfInput(const NodeGraph& nodeGraph, EditorNodePin pin)
{
	if (pin.IsInput)
	{
		const PinID outPinID = nodeGraph.GetOutputPinForInput(pin.ID);
		if (!outPinID) return EditorNodePin{};
		pin = nodeGraph.GetPinByID(outPinID);
	}
	return pin;
}

BoolValueNode* PinEvaluator::EvaluateBool(EditorNodePin pin)
{
	pin = GetOutputPinIfInput(m_NodeGraph, pin);
	NE_ASSERT(pin.Type == PinType::Bool);

	if (pin.Type == PinType::Invalid)
	{
		m_ErrorMessages.push_back("Bool pin input missing link!");
		return new ConstantValueNode<bool>(false);
	}

	EditorNode* node = m_NodeGraph.GetPinOwner(pin.ID);
	switch (node->GetType())
	{
	case EditorNodeType::Bool: return EvaluateBool(static_cast<BoolEditorNode*>(node));
	case EditorNodeType::BoolBinaryOperator: return EvaluateBoolBinaryOperator(static_cast<BoolBinaryOperatorEditorNode*>(node));
	case EditorNodeType::FloatComparisonOperator: return EvaluateFloatComparisonOperator(static_cast<FloatComparisonOperatorEditorNode*>(node));
	default:
		NOT_IMPLEMENTED;
		m_ErrorMessages.push_back("[NodeGraphCompiler::EvaluateBoolPin] internal error!");
	}
	return new ConstantValueNode<bool>(false);
}

StringValueNode* PinEvaluator::EvaluateString(EditorNodePin pin)
{
	pin = GetOutputPinIfInput(m_NodeGraph, pin);
	NE_ASSERT(pin.Type == PinType::String);

	if (pin.Type == PinType::Invalid)
	{
		m_ErrorMessages.push_back("String pin input missing link!");
		return new ConstantValueNode<std::string>("");
	}

	EditorNode* node = m_NodeGraph.GetPinOwner(pin.ID);
	switch (node->GetType())
	{
	case EditorNodeType::String: return EvaluateString(static_cast<StringEditorNode*>(node));
	//case EditorNodeType::StringBinaryOperator: return EvaluateStringBinaryOperator(static_cast<StringBinaryOperatorEditorNode*>(node));
	default:
		NOT_IMPLEMENTED;
		m_ErrorMessages.push_back("[NodeGraphCompiler::EvaluateStringPin] internal error!");
	}
	return new ConstantValueNode<std::string>("");
}

FloatValueNode* PinEvaluator::EvaluateFloat(EditorNodePin pin)
{
	pin = GetOutputPinIfInput(m_NodeGraph, pin);
	NE_ASSERT(pin.Type == PinType::Float);

	if (pin.Type == PinType::Invalid)
	{
		m_ErrorMessages.push_back("Float pin input missing link!");
		return new ConstantValueNode<float>(0.0f);
	}

	EditorNode* node = m_NodeGraph.GetPinOwner(pin.ID);
	switch (node->GetType())
	{
	case EditorNodeType::Float: return EvaluateFloat(static_cast<FloatEditorNode*>(node));
	case EditorNodeType::FloatBinaryOperator: return EvaluateFloatBinaryOperator(static_cast<FloatBinaryOperatorEditorNode*>(node));
	case EditorNodeType::SplitFloat2: return EvaluateSplitFloat2(static_cast<SplitFloat2EditorNode*>(node), pin);
	case EditorNodeType::SplitFloat3: return EvaluateSplitFloat3(static_cast<SplitFloat3EditorNode*>(node), pin);
	case EditorNodeType::SplitFloat4: return EvaluateSplitFloat4(static_cast<SplitFloat4EditorNode*>(node), pin);
	case EditorNodeType::OnUpdate: return new VariableValueNode<float>("dt");
	default:
		NOT_IMPLEMENTED;
		m_ErrorMessages.push_back("[NodeGraphCompiler::EvaluateFloatPin] internal error!");
	}
	return new ConstantValueNode<float>(0.0f);
}

Float2ValueNode* PinEvaluator::EvaluateFloat2(EditorNodePin pin)
{
	pin = GetOutputPinIfInput(m_NodeGraph, pin);
	NE_ASSERT(pin.Type == PinType::Float2);

	if (pin.Type == PinType::Invalid)
	{
		m_ErrorMessages.push_back("Float2 pin input missing link!");
		return new ConstantValueNode<Float2>({});
	}

	EditorNode* node = m_NodeGraph.GetPinOwner(pin.ID);
	switch (node->GetType())
	{
	case EditorNodeType::Float2: return EvaluateFloat2(static_cast<Float2EditorNode*>(node));
	case EditorNodeType::Float2BinaryOperator: return EvaluateFloat2BinaryOperator(static_cast<Float2BinaryOperatorEditorNode*>(node));
	default:
		NOT_IMPLEMENTED;
		m_ErrorMessages.push_back("[NodeGraphCompiler::EvaluateFloatPin] internal error!");
	}
	return new ConstantValueNode<Float2>({});
}

Float3ValueNode* PinEvaluator::EvaluateFloat3(EditorNodePin pin)
{
	pin = GetOutputPinIfInput(m_NodeGraph, pin);
	NE_ASSERT(pin.Type == PinType::Float3);

	if (pin.Type == PinType::Invalid)
	{
		m_ErrorMessages.push_back("Float3 pin input missing link!");
		return new ConstantValueNode<Float3>({});
	}

	EditorNode* node = m_NodeGraph.GetPinOwner(pin.ID);
	switch (node->GetType())
	{
	case EditorNodeType::Float3: return EvaluateFloat3(static_cast<Float3EditorNode*>(node));
	case EditorNodeType::Float3BinaryOperator: return EvaluateFloat3BinaryOperator(static_cast<Float3BinaryOperatorEditorNode*>(node));
	default:
		NOT_IMPLEMENTED;
		m_ErrorMessages.push_back("[NodeGraphCompiler::EvaluateFloatPin] internal error!");
	}
	return new ConstantValueNode<Float3>({});
}

Float4ValueNode* PinEvaluator::EvaluateFloat4(EditorNodePin pin)
{
	pin = GetOutputPinIfInput(m_NodeGraph, pin);
	NE_ASSERT(pin.Type == PinType::Float4);

	if (pin.Type == PinType::Invalid)
	{
		m_ErrorMessages.push_back("Float4 pin input missing link!");
		return new ConstantValueNode<Float4>({});
	}

	EditorNode* node = m_NodeGraph.GetPinOwner(pin.ID);
	switch (node->GetType())
	{
	case EditorNodeType::Float4: return EvaluateFloat4(static_cast<Float4EditorNode*>(node));
	case EditorNodeType::Float4BinaryOperator: return EvaluateFloat4BinaryOperator(static_cast<Float4BinaryOperatorEditorNode*>(node));
	default:
		NOT_IMPLEMENTED;
		m_ErrorMessages.push_back("[NodeGraphCompiler::EvaluateFloatPin] internal error!");
	}
	return new ConstantValueNode<Float4>({});
}

BoolValueNode* PinEvaluator::EvaluateBool(BoolEditorNode* node)
{
	return new ConstantValueNode<bool>(node->GetValue());
}

BoolValueNode* PinEvaluator::EvaluateBoolBinaryOperator(BoolBinaryOperatorEditorNode* node)
{
	BoolValueNode* a = EvaluateBool(node->GetAPin());
	BoolValueNode* b = EvaluateBool(node->GetBPin());
	return new BoolBinaryOperatorValueNode(a, b, node->GetOp());
}

BoolValueNode* PinEvaluator::EvaluateFloatComparisonOperator(FloatComparisonOperatorEditorNode* node)
{
	FloatValueNode* a = EvaluateFloat(node->GetAPin());
	FloatValueNode* b = EvaluateFloat(node->GetBPin());
	return new ComparisonValueNode<float>{ a, b, node->GetOp() };
}

StringValueNode* PinEvaluator::EvaluateString(StringEditorNode* node)
{
	return new ConstantValueNode<std::string>(node->GetValue());
}

//StringValueNode* PinEvaluator::EvaluateStringBinaryOperator(StringBinaryOperatorEditorNode* node)
//{
//	StringValueNode* a = EvaluateString(node->GetAPin());
//	StringValueNode* b = EvaluateString(node->GetBPin());
//	return new BinaryArithmeticOperatorValueNode<std::string>{ a, b, node->GetOp()[0] };
//}

FloatValueNode* PinEvaluator::EvaluateFloat(FloatEditorNode* node)
{
	return new ConstantValueNode<float>(node->GetFloat());
}

FloatValueNode* PinEvaluator::EvaluateFloatBinaryOperator(FloatBinaryOperatorEditorNode* node)
{
	FloatValueNode* a = EvaluateFloat(node->GetAPin());
	FloatValueNode* b = EvaluateFloat(node->GetBPin());
	return new BinaryArithmeticOperatorValueNode<float>{ a, b, node->GetOp()[0] };
}

Float2ValueNode* PinEvaluator::EvaluateFloat2(Float2EditorNode* node)
{
	return new ConstantValueNode<Float2>(node->GetFloat2());
}

Float2ValueNode* PinEvaluator::EvaluateFloat2BinaryOperator(Float2BinaryOperatorEditorNode* node)
{
	Float2ValueNode* a = EvaluateFloat2(node->GetAPin());
	Float2ValueNode* b = EvaluateFloat2(node->GetBPin());
	return new BinaryArithmeticOperatorValueNode<Float2>{ a, b, node->GetOp()[0] };
}

Float3ValueNode* PinEvaluator::EvaluateFloat3(Float3EditorNode* node)
{
	return new ConstantValueNode<Float3>(node->GetFloat3());
}

Float3ValueNode* PinEvaluator::EvaluateFloat3BinaryOperator(Float3BinaryOperatorEditorNode* node)
{
	Float3ValueNode* a = EvaluateFloat3(node->GetAPin());
	Float3ValueNode* b = EvaluateFloat3(node->GetBPin());
	return new BinaryArithmeticOperatorValueNode<Float3>{ a, b, node->GetOp()[0] };
}

Float4ValueNode* PinEvaluator::EvaluateFloat4(Float4EditorNode* node)
{
	return new ConstantValueNode<Float4>(node->GetFloat4());
}

Float4ValueNode* PinEvaluator::EvaluateFloat4BinaryOperator(Float4BinaryOperatorEditorNode* node)
{
	Float4ValueNode* a = EvaluateFloat4(node->GetAPin());
	Float4ValueNode* b = EvaluateFloat4(node->GetBPin());
	return new BinaryArithmeticOperatorValueNode<Float4>{ a, b, node->GetOp()[0] };
}

FloatValueNode* PinEvaluator::EvaluateSplitFloat2(SplitFloat2EditorNode* node, const EditorNodePin& pin)
{
	NE_ASSERT(pin.Type == PinType::Float);
	NE_ASSERT(m_NodeGraph.GetPinOwner(pin.ID) == node);

	ValueNode<Float2>* vec = EvaluateFloat2(node->GetInputVectorPin());
	const unsigned vecIndex = node->GetOutputPinIndex(pin.ID);
	return new SplitVectorValueNode<Float2, float>(vec, vecIndex);
}

FloatValueNode* PinEvaluator::EvaluateSplitFloat3(SplitFloat3EditorNode* node, const EditorNodePin& pin)
{
	NE_ASSERT(pin.Type == PinType::Float);
	NE_ASSERT(m_NodeGraph.GetPinOwner(pin.ID) == node);

	ValueNode<Float3>* vec = EvaluateFloat3(node->GetInputVectorPin());
	const unsigned vecIndex = node->GetOutputPinIndex(pin.ID);
	return new SplitVectorValueNode<Float3, float>(vec, vecIndex);
}

FloatValueNode* PinEvaluator::EvaluateSplitFloat4(SplitFloat4EditorNode* node, const EditorNodePin& pin)
{
	NE_ASSERT(pin.Type == PinType::Float);
	NE_ASSERT(m_NodeGraph.GetPinOwner(pin.ID) == node);

	ValueNode<Float4>* vec = EvaluateFloat4(node->GetInputVectorPin());
	const unsigned vecIndex = node->GetOutputPinIndex(pin.ID);
	return new SplitVectorValueNode<Float4, float>(vec, vecIndex);
}
