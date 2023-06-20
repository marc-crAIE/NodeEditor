#include "nepch.h"
#include "NodeGraphCompiler.h"

CompiledPipeline NodeGraphCompiler::Compile(const NodeGraph& graph)
{
	m_ErrorMessages.clear();
	m_CurrentGraph = &graph;
	m_PinEvaluator = CreateScope<PinEvaluator>(graph, m_ErrorMessages);

	CompiledPipeline pipeline{};
	pipeline.OnStartNode = Compile(m_CurrentGraph->GetOnStartNode());
	pipeline.OnUpdateNode = Compile(m_CurrentGraph->GetOnUpdateNode());
	return pipeline;
}

ExecutionEditorNode* NodeGraphCompiler::GetNextExecutorNode(ExecutionEditorNode* executorNode)
{
	// TODO: Do not use dynamic cast, but check node by type if it is execution

	const auto nextNodePin = m_CurrentGraph->GetInputPinFromOutput(executorNode->GetOutput().ID);
	if (!nextNodePin) 
		return (ExecutionEditorNode*) nullptr;
	EditorNode* node = m_CurrentGraph->GetPinOwner(nextNodePin);
	ExecutionEditorNode* exNode = dynamic_cast<ExecutionEditorNode*>(node);
	NE_ASSERT(exNode);
	return exNode;
}

ExecutorNode* NodeGraphCompiler::Compile(ExecutionEditorNode* executorNode)
{
	ExecutorNode* firstNode = new EmptyExecutorNode{};
	ExecutorNode* currentNode = firstNode;
	ExecutionEditorNode* currentEditorNode = executorNode;

	while (currentEditorNode)
	{
		ExecutorNode* nextNode = CompileExecutorNode(currentEditorNode);
		currentNode->SetNextNode(nextNode);
		currentNode = nextNode;
		currentEditorNode = GetNextExecutorNode(currentEditorNode);
	}

	return firstNode;
}

#define COMPILE_NODE(NodeType, Compiler, SpecificType) case EditorNodeType::NodeType: return Compiler(static_cast<SpecificType*>(executorNode))

ExecutorNode* NodeGraphCompiler::CompileExecutorNode(ExecutionEditorNode* executorNode)
{
	switch (executorNode->GetType())
	{
		COMPILE_NODE(Print, CompilePrintNode, PrintEditorNode);
		COMPILE_NODE(If, CompileIfNode, IfEditorNode);
		COMPILE_NODE(OnStart, CompileEmptyNode, ExecutionEditorNode);
		COMPILE_NODE(OnUpdate, CompileEmptyNode, ExecutionEditorNode);
	default:
		m_ErrorMessages.push_back("[NodeGraphCompiler::CompileExecutorNode] internal error!");
		NE_ASSERT(0, "Not implemented!");
	}
	return new EmptyExecutorNode{};
}

ExecutorNode* NodeGraphCompiler::CompileEmptyNode(ExecutionEditorNode* node)
{
	return new EmptyExecutorNode{};
}

ExecutorNode* NodeGraphCompiler::CompilePrintNode(PrintEditorNode* printNode)
{
	PinID inputPinID = m_CurrentGraph->GetOutputPinForInput(printNode->GetInput().ID);
	if (!inputPinID)
	{
		m_ErrorMessages.push_back("Print node missing outputs!");
		return new EmptyExecutorNode{};
	}

	EditorNodePin inputPin = m_CurrentGraph->GetPinByID(inputPinID);

	switch (inputPin.Type)
	{
	case PinType::Bool:
	{
		PrintExecutorNode<bool>* printNode = new PrintExecutorNode<bool>();
		printNode->m_Value = Scope<BoolValueNode>(m_PinEvaluator->EvaluateBool(inputPin));
		return printNode;
	}
	case PinType::String:
	{
		PrintExecutorNode<std::string>* printNode = new PrintExecutorNode<std::string>();
		printNode->m_Value = Scope<StringValueNode>(m_PinEvaluator->EvaluateString(inputPin));
		return printNode;
	}
	case PinType::Float:
	{
		PrintExecutorNode<float>* printNode = new PrintExecutorNode<float>();
		printNode->m_Value = Scope<FloatValueNode>(m_PinEvaluator->EvaluateFloat(inputPin));
		return printNode;
	}
	case PinType::Float2:
	{
		PrintExecutorNode<Float2>* printNode = new PrintExecutorNode<Float2>();
		printNode->m_Value = Scope<Float2ValueNode>(m_PinEvaluator->EvaluateFloat2(inputPin));
		return printNode;
	}
	case PinType::Float3:
	{
		PrintExecutorNode<Float3>* printNode = new PrintExecutorNode<Float3>();
		printNode->m_Value = Scope<Float3ValueNode>(m_PinEvaluator->EvaluateFloat3(inputPin));
		return printNode;
	}
	case PinType::Float4:
	{
		PrintExecutorNode<Float4>* printNode = new PrintExecutorNode<Float4>();
		printNode->m_Value = Scope<Float4ValueNode>(m_PinEvaluator->EvaluateFloat4(inputPin));
		return printNode;
	}
	}

	return new EmptyExecutorNode{};
}

ExecutorNode* NodeGraphCompiler::CompileIfNode(IfEditorNode* ifNode)
{
	BoolValueNode* conditionNode = m_PinEvaluator->EvaluateBool(ifNode->GetConditionPin());

	ExecutorNode* elseBranch = nullptr;
	if (const NodeID elsePinInput = m_CurrentGraph->GetInputPinFromOutput(ifNode->GetExecutionElse().ID))
	{
		EditorNode* elseEditorNode = m_CurrentGraph->GetPinOwner(elsePinInput);
		ExecutionEditorNode* exElseEditorNode = dynamic_cast<ExecutionEditorNode*>(elseEditorNode);
		NE_ASSERT(exElseEditorNode);

		if (exElseEditorNode) elseBranch = Compile(exElseEditorNode);
	}
	return new IfExecutorNode{ conditionNode, elseBranch };
}
