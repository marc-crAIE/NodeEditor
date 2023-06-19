#include "nepch.h"
#include "EvaluationEditorNode.h"

#include "Core/ImGuiCommon.h"

void BoolEditorNode::RenderContent()
{
    ImGui::Checkbox("", &m_Value);
}

void FloatNEditorNode::RenderContent()
{
    const std::string valueNames[] = { "X", "Y", "Z", "W" };

    for (unsigned i = 0; i < m_NumValues; i++)
    {
        ImGui::SetNextItemWidth(50.0f);
        ImGui::DragFloat(valueNames[i].c_str(), &m_Values[i]);
    }
}

void BinaryOperatorEditorNode::RenderContent()
{
    ImGui::Text(m_Op.c_str());
}
