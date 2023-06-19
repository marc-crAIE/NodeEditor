#include "nepch.h"
#include "Executor.h"

#include "Core/ImGuiCommon.h"
#include "ExecutorNode.h"

void Executor::OnStart()
{
	m_Context = ExecuteContext{};

	ExecutorNode* node = m_Pipeline.OnStartNode;
	while (node)
	{
		node->Execute(m_Context);
		node = node->GetNextNode();
	}
}

void Executor::OnUpdate(float dt)
{
	if (m_Context.Failure) return;

	ExecutorNode* node = m_Pipeline.OnUpdateNode;
	const std::string dtVarname = "dt";
	// TODO: Add DT var to a variables 'block'

	while (node)
	{
		node->Execute(m_Context);
		node = node->GetNextNode();
	}
}

void Executor::SetCompiledPipeline(CompiledPipeline pipeline)
{
	delete m_Pipeline.OnStartNode;
	delete m_Pipeline.OnUpdateNode;
	m_Pipeline = pipeline;
}
