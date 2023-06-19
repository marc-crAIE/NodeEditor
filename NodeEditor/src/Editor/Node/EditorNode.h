#pragma once

#include <string>
#include <typeindex>

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
	Float4,
	SplitFloat2,
	SplitFloat3,
	SplitFloat4,
	Print,
	If
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
public:
	template<typename T>
	static EditorNode* GetClassRepresent()
	{
		if (s_ClassRepresents.find(typeid(T)) == s_ClassRepresents.end())
			s_ClassRepresents[typeid(T)] = new T{};
		return s_ClassRepresents[typeid(T)];
	}
protected:
	virtual void RenderContent() {}
public:
	static std::unordered_map<std::type_index, EditorNode*> s_ClassRepresents;
private:
	std::string m_Label;
	EditorNodeType m_Type;
	NodeID m_ID = 0;

	std::vector<EditorNodePin> m_Pins;
};