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

    ImNode::BeginNode(23);

    ImGui::Text("Test");

    ImNode::EndNode();

    ImNode::End();
    ImNode::SetCurrentEditor(nullptr);
}

void Editor::Load()
{
    ImNode::Config config;
    config.SettingsFile = "NodeEditor.json";
    m_EditorContext = ImNode::CreateEditor(&config);
}

void Editor::Unload()
{
    ImNode::DestroyEditor(m_EditorContext);
}
