#pragma once

#include <string>

#include "Core/UUID.h"
#include "EditorNodePin.h"

enum class EditorNodeType
{
	Invalid = -1,

	OnUpdate,
	OnStart,
	Bool,
	Float,
	Float2,
	Float3,
	Float4
};

struct EditorNodeLink
{
	LinkID ID;
	PinID Start;
	PinID End;
};

class EditorNode
{
public:
	EditorNode(const std::string& label, EditorNodeType type);

	void Render();

	size_t AddPin(const EditorNodePin& pin);
	void RemovePin(PinID pinID);

	NodeID GetID() const { return m_ID; }
	EditorNodeType GetType() const { return m_Type; }
	const std::vector<EditorNodePin>& GetPins() const { return m_Pins; }
private:
	std::string m_Label;
	EditorNodeType m_Type;
	NodeID m_ID = 0;

	std::vector<EditorNodePin> m_Pins;
};