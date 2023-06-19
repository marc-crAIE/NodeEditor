#include "nepch.h"
#include "EditorContextMenu.h"

#include "Editor/Node/EvaluationEditorNode.h"
#include "Core/ImGuiCommon.h"

void EditorContextMenu::Draw()
{
	ImNode::Suspend();

	if (m_Open)
	{
		ImGui::OpenPopup(m_ID.c_str());
		m_Open = false;
	}

	if (ImGui::BeginPopup(m_ID.c_str()))
	{
		DrawContent();
		ImGui::EndPopup();
	}

	ImNode::Resume();
}

template<typename T>
static bool IsCompatible(const EditorNodePin& nodePin)
{
	if (nodePin.Type == PinType::Invalid)
		return true;

	EditorNode* node = EditorNode::GetClassRepresent<T>();
	EditorNodePin targetPin;
	for (const auto& pin : node->GetPins())
	{
		if (pin.Type == nodePin.Type && pin.IsInput != nodePin.IsInput)
		{
			targetPin = pin;
			break;
		}
	}
	return targetPin.Type != PinType::Invalid;
}

#define ADD_NODE(text, type) if (IsCompatible<type>(nodePin) && ImGui::MenuItem(text)) newNode = new type()

void NewNodeContextMenu::DrawContent()
{
	EditorNode* newNode = nullptr;

	EditorNodePin nodePin;
	if (m_DraggedPin) nodePin = m_NodeGraph.GetPinByID(m_DraggedPin);

	if (ImGui::BeginMenu("Constants"))
	{
		ADD_NODE("Bool", BoolEditorNode);
		ADD_NODE("Float", FloatEditorNode);
		ADD_NODE("Float2", Float2EditorNode);
		ADD_NODE("Float3", Float3EditorNode);
		ADD_NODE("Float4", Float4EditorNode);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Split"))
	{
		ADD_NODE("Float2", SplitFloat2EditorNode);
		ADD_NODE("Float3", SplitFloat3EditorNode);
		ADD_NODE("Float4", SplitFloat4EditorNode);
		ImGui::EndMenu();
	}

	ADD_NODE("Print", PrintEditorNode);
	ADD_NODE("If", IfEditorNode);

	if (newNode)
	{
		m_NodeGraph.AddNode(newNode);
		ImNode::SetNodePosition((uintptr_t)newNode->GetID(), ImNode::ScreenToCanvas(ImGui::GetMousePos()));

		if (nodePin.Type != PinType::Invalid)
		{
			EditorNodePin targetPin;
			for (const auto& pin : newNode->GetPins())
			{
				if (pin.Type == nodePin.Type && pin.IsInput != nodePin.IsInput)
				{
					targetPin = pin;
					break;
				}
			}

			if (targetPin.Type != PinType::Invalid)
			{
				const PinID startPin = nodePin.IsInput ? targetPin.ID : nodePin.ID;
				const PinID endPin = nodePin.IsInput ? nodePin.ID : targetPin.ID;
				m_NodeGraph.AddLink({ UUID(), startPin, endPin});
			}
		}
	}
}
