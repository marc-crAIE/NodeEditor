#include "nepch.h"
#include "Editor.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Editor::~Editor()
{
}

void Editor::Render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImNode::SetCurrentEditor(m_EditorContext);
    ImNode::Begin("Node Editor", ImVec2(0.0, 0.0f));

    UpdateEditor();
    RenderEditor();

    RenderContextMenus();

    ImNode::End();
    ImNode::SetCurrentEditor(nullptr);
}

void Editor::Load(NodeGraph* nodeGraph)
{
    ImNode::Config config;
    config.SettingsFile = "NodeEditor.json";
    m_EditorContext = ImNode::CreateEditor(&config);

    m_NodeGraph = Scope<NodeGraph>(nodeGraph);

    m_NewNodeMenu = CreateScope<NewNodeContextMenu>("New Node Menu", *nodeGraph);
}

void Editor::Unload()
{
    ImNode::DestroyEditor(m_EditorContext);
}

void Editor::UpdateEditor()
{
    ImNode::PinId startPinID, endPinID, pin;
    ImNode::LinkId link;
    ImNode::NodeId node;

    if (ImNode::BeginCreate(ImColor(255, 255, 255), 2.0f))
    {
        if (ImNode::QueryNewLink(&startPinID, &endPinID))
        {
            if (startPinID && endPinID)
            {
                auto startPin = m_NodeGraph->GetPinByID(startPinID.Get());
                auto endPin = m_NodeGraph->GetPinByID(endPinID.Get());

                // Make sure start is always output pin
                if (startPin.IsInput)
                {
                    const auto tmpPin = startPin;
                    startPin = endPin;
                    endPin = tmpPin;
                }

                bool validLink = startPin.ID != endPin.ID;
                validLink = validLink && startPin.IsInput != endPin.IsInput;
                validLink = validLink && (startPin.Type == endPin.Type || IsGenericPinType(startPin.Type, endPin.Type));

                if (!validLink)
                {
                    ImNode::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                }
                else if (ImNode::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
                {
                    m_NodeGraph->AddLink(EditorNodeLink{ UUID(), startPin.ID, endPin.ID});
                }
            }
        }

        if (ImNode::QueryNewNode(&pin))
        {
            if (ImNode::AcceptNewItem())
            {
                m_NewNodeMenu->Open(pin.Get());
            }
        }
    }
    ImNode::EndCreate();

    // Open context menus
    if (ImNode::ShowBackgroundContextMenu()) m_NewNodeMenu->Open(0);
}

void Editor::RenderEditor()
{
    m_NodeGraph->ForEachNode([](EditorNode* node) { node->Render(); });

    m_NodeGraph->ForEachLink([this](const EditorNodeLink& link) {
        const auto& outputPin = m_NodeGraph->GetPinByID(link.Start);
        const bool isExecution = outputPin.Type == PinType::Execution;
        ImNode::Link((uintptr_t)link.ID, (uintptr_t)link.Start, (uintptr_t)link.End, GetPinColor(outputPin.Type), isExecution ? 3.0f : 1.0f);
    });
}

void Editor::RenderContextMenus()
{
    m_NewNodeMenu->Draw();
}
