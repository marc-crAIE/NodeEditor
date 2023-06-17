#pragma once

#include "Core/ImGuiCommon.h"

class Editor
{
public:
    Editor() = default;
    ~Editor();

    void Render();

    void Load();
    void Unload();
private:
    ImNode::EditorContext* m_EditorContext = nullptr;
};