#pragma once

#include "Editor/Node/EditorNode.h"
#include "Editor/Node/ExecutionEditorNode.h"
#include "Editor/Node/EvaluationEditorNode.h"
#include "NodeGraph.h"
#include "PinEvaluator.h"
#include "Execution/ExecuteContext.h"
#include "Execution/ExecutorNode.h"

class NodeGraphCompiler
{
public:
	CompiledPipeline Compile(const NodeGraph& graph);

	const std::vector<std::string> GetErrorMessages() const { return m_ErrorMessages; }
private:
	ExecutionEditorNode* GetNextExecutorNode(ExecutionEditorNode* executorNode);

	ExecutorNode* Compile(ExecutionEditorNode* executorNode);
	ExecutorNode* CompileExecutorNode(ExecutionEditorNode* executorNode);

	ExecutorNode* CompileEmptyNode(ExecutionEditorNode* node);
	ExecutorNode* CompilePrintNode(PrintEditorNode* printNode);
	ExecutorNode* CompileIfNode(IfEditorNode* ifNode);
private:
	Scope<PinEvaluator> m_PinEvaluator = nullptr;
	const NodeGraph* m_CurrentGraph = nullptr;

	std::vector<std::string> m_ErrorMessages;
};