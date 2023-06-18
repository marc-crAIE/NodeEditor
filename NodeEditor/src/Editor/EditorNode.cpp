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

size_t EditorNode::AddPin(const EditorNodePin& pin)
{
	m_Pins.push_back(pin);
	return m_Pins.size() - 1;
}

void EditorNode::RemovePin(PinID pinID)
{
	for (size_t i = 0; i < m_Pins.size(); i++)
	{
		const auto& pin = m_Pins[i];
		if (pin.ID == pinID)
		{
			m_Pins.erase(m_Pins.begin() + 1);
			return;
		}
	}
}
