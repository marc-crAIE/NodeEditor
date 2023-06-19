#pragma once

#include <vector>
#include <string>

#include "NodeGraph.h"
#include "Execution/ValueNode.h"
#include "Editor/Node/EvaluationEditorNode.h"

class PinEvaluator
{
public:
	PinEvaluator(const NodeGraph& nodeGraph, std::vector<std::string>& errorMessages) 
		: m_NodeGraph(nodeGraph), m_ErrorMessages(errorMessages) 
	{}

	BoolValueNode* EvaluateBool(EditorNodePin pin);
	FloatValueNode* EvaluateFloat(EditorNodePin pin);
	Float2ValueNode* EvaluateFloat2(EditorNodePin pin);
	Float3ValueNode* EvaluateFloat3(EditorNodePin pin);
	Float4ValueNode* EvaluateFloat4(EditorNodePin pin);
private:
	BoolValueNode* EvaluateBool(BoolEditorNode* node);
	FloatValueNode* EvaluateFloat(FloatEditorNode* node);
	Float2ValueNode* EvaluateFloat2(Float2EditorNode* node);
	Float3ValueNode* EvaluateFloat3(Float3EditorNode* node);
	Float4ValueNode* EvaluateFloat4(Float4EditorNode* node);
	FloatValueNode* EvaluateSplitFloat2(SplitFloat2EditorNode* node, const EditorNodePin& pin);
	FloatValueNode* EvaluateSplitFloat3(SplitFloat3EditorNode* node, const EditorNodePin& pin);
	FloatValueNode* EvaluateSplitFloat4(SplitFloat4EditorNode* node, const EditorNodePin& pin);
private:
	const NodeGraph& m_NodeGraph;
	std::vector<std::string>& m_ErrorMessages;
};