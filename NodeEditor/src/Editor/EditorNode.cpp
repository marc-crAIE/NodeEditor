#include "nepch.h"
#include "EditorNode.h"

#include "Core/ImGuiCommon.h"

EditorNode::EditorNode(const std::string& label, EditorNodeType type)
	: m_Label(label), m_ID(UUID()), m_Type(type)
{ }

void EditorNode::Render()
{
	ImGui::PushID(m_ID);

	ImNode::BeginNode((uintptr_t)m_ID);

	ImGui::Text(m_Label.c_str());

	ImNode::EndNode();

	ImGui::PopID();
}
