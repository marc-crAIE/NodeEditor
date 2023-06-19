#pragma once

#include <string>
#include <vector>

#include "Core/UUID.h"
#include "NodeGraph/NodeGraph.h"

class EditorContextMenu
{
public:
	EditorContextMenu(const std::string& id)
		: m_ID(id)
	{}

	void Open() { m_Open = true; }
	void Draw();
protected:
	virtual void DrawContent() = 0;
private:
	bool m_Open = false;
	std::string m_ID;
};

class NewNodeContextMenu : public EditorContextMenu
{
public:
	NewNodeContextMenu(const std::string& id, NodeGraph& nodeGraph) 
		: EditorContextMenu(id), m_NodeGraph(nodeGraph)
	{}

	void Open() = delete;
	void Open(PinID draggedPin)
	{
		EditorContextMenu::Open();
		m_DraggedPin = draggedPin;
	}
protected:
	void DrawContent() override;
private:
	NodeGraph& m_NodeGraph;
	PinID m_DraggedPin = 0;
};