#pragma once

#include <string>

#include "Core/UUID.h"

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

class EditorNode
{
public:
	EditorNode(const std::string& label, EditorNodeType type);

	void Render();

	NodeID GetID() const { return m_ID; }
	EditorNodeType GetType() const { return m_Type; }
private:
	std::string m_Label;
	EditorNodeType m_Type;
	NodeID m_ID = 0;
};