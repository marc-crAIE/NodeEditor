#include "nepch.h"
#include "EditorNodePin.h"

EditorNodePin EditorNodePin::CreateInputPin(const std::string& label, PinType type)
{
	EditorNodePin pin;
	pin.IsInput = true;
	pin.Type = type;
	pin.ID = UUID();
	pin.Label = label;
	return pin;
}

EditorNodePin EditorNodePin::CreateOutputPin(const std::string& label, PinType type)
{
	EditorNodePin pin;
	pin.IsInput = false;
	pin.Type = type;
	pin.ID = UUID();
	pin.Label = label;
	return pin;
}
