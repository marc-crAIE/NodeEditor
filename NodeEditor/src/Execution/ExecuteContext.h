#pragma once

struct ExecuteContext
{
	bool Failure = false;
};

class ExecutorNode;

struct CompiledPipeline
{
	ExecutorNode* OnStartNode = nullptr;
	ExecutorNode* OnUpdateNode = nullptr;
};