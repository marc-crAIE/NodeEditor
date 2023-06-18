#include "nepch.h"
#include "NodeGraph.h"

void NodeGraph::AddNode(EditorNode* node)
{
	m_Nodes[node->GetID()] = Scope<EditorNode>(node);
}

void NodeGraph::AddLink(const EditorNodeLink& link)
{
	for (const auto& it : m_Links)
	{
		const auto& l = it.second;

		const auto& endPin = GetPinByID(link.End);
		if (endPin.Type != PinType::Execution && l.End == link.End)
		{
			m_Links.erase(it.first);
			break;
		}

		const auto& startPin = GetPinByID(link.Start);
		if (startPin.Type == PinType::Execution && l.Start == link.Start)
		{
			m_Links.erase(it.first);
			break;
		}
	}
	m_Links[link.ID] = link;
}

void NodeGraph::RemoveNode(NodeID nodeID)
{
	EditorNode* node = m_Nodes[nodeID].get();

	std::vector<LinkID> linksToRemove;
	for (const auto& l : m_Links)
	{
		const auto& link = l.second;
		if (GetPinOwner(link.Start) == node || GetPinOwner(link.End) == node)
			linksToRemove.push_back(link.ID);
	}

	for (const LinkID& linkID : linksToRemove)
		RemoveLink(linkID);

	m_Nodes.erase(nodeID);
}

void NodeGraph::RemoveLink(LinkID linkID)
{
	m_Links.erase(linkID);
}

void NodeGraph::RemovePin(PinID pinID)
{
	std::vector<LinkID> linksToRemove;
	for (const auto& l : m_Links)
	{
		const auto& link = l.second;
		if (link.Start == pinID || link.End == pinID)
			linksToRemove.push_back(link.ID);
	}

	for (const LinkID& linkID : linksToRemove)
		RemoveLink(linkID);

	EditorNode* node = GetPinOwner(pinID);
	node->RemovePin(pinID);
}

EditorNodePin NodeGraph::GetPinByID(PinID pinID) const
{
	for (const auto& n : m_Nodes)
	{
		const auto& node = n.second;
		for (const auto& pin : node->GetPins())
		{
			if (pin.ID == pinID)
				return pin;
		}
	}
	NE_ASSERT(0, "Pin not found!");
	return EditorNodePin{};
}

EditorNode* NodeGraph::GetPinOwner(PinID pinID) const
{
	for (const auto& n : m_Nodes)
	{
		const auto& node = n.second;
		for (const auto& pin : node->GetPins())
		{
			if (pin.ID == pinID)
				return node.get();
		}
	}
	return nullptr;
}

NodeGraph* NodeGraph::CreateDefault()
{
	NodeGraph* nodeGraph = new NodeGraph();
	nodeGraph->AddNode(new OnStartEditorNode());
	nodeGraph->AddNode(new OnUpdateEditorNode());
	return nodeGraph;
}
