#pragma once

#include "Core/ImGuiCommon.h"
#include "NodeGraph/NodeGraph.h"
#include "Drawing/EditorContextMenu.h"

class Editor
{
public:
    Editor() = default;
    ~Editor();

    void Render();

    void Load(NodeGraph* nodeGraph);
    void Unload();

    const NodeGraph& GetNodeGraph() const { return *m_NodeGraph; }
private:
    void UpdateEditor();
    void RenderEditor();

    void RenderContextMenus();
private:
    ImNode::EditorContext* m_EditorContext = nullptr;

    Scope<NodeGraph> m_NodeGraph;

    Scope<NewNodeContextMenu> m_NewNodeMenu;
};