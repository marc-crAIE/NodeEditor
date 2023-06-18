#pragma once

#include "Core/ImGuiCommon.h"
#include "NodeGraph/NodeGraph.h"

class Editor
{
public:
    Editor() = default;
    ~Editor();

    void Render();

    void Load(NodeGraph* nodeGraph);
    void Unload();
private:
    ImNode::EditorContext* m_EditorContext = nullptr;

    Scope<NodeGraph> m_NodeGraph;
};