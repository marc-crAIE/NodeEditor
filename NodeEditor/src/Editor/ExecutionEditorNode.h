#pragma once

#include "EditorNode.h"

class ExecutionEditorNode : public EditorNode
{
public:
	ExecutionEditorNode(const std::string& label, EditorNodeType type, bool input = true)
		: EditorNode(label, type)
	{
		if (input)
			m_PinInput = AddPin(EditorNodePin::CreateInputPin("", PinType::Execution));
		m_PinOutput = AddPin(EditorNodePin::CreateOutputPin("", PinType::Execution));
	}

	const EditorNodePin& GetInput() const { return GetPins()[m_PinInput]; }
	const EditorNodePin& GetOutput() const { return GetPins()[m_PinOutput]; }
private:
	size_t m_PinInput;
	size_t m_PinOutput;
};

class OnStartEditorNode : public ExecutionEditorNode
{
public:
	OnStartEditorNode() : ExecutionEditorNode("On Start", EditorNodeType::OnStart, true) {}
};

class OnUpdateEditorNode : public ExecutionEditorNode
{
public:
	OnUpdateEditorNode() 
		: ExecutionEditorNode("On Update", EditorNodeType::OnStart, true) 
	{
		AddPin(EditorNodePin::CreateOutputPin("DT", PinType::Float));
	}
};