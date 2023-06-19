#pragma once

#include "ExecuteContext.h"

class Executor
{
public:
	void OnStart();
	void OnUpdate(float dt);

	void SetCompiledPipeline(CompiledPipeline pipeline);
private:
	ExecuteContext m_Context;
	CompiledPipeline m_Pipeline;
};