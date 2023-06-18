#pragma once

#include <string>

#include "Core/UUID.h"

enum class PinType
{
	Invalid = -1,

	Execution,
	Bool,
	Float,
	Float2,
	Float3,
	Float4
};

struct EditorNodePin
{
	bool IsInput = false;
	PinType Type = PinType::Invalid;
	PinID ID = 0;
	std::string Label = "";

	static EditorNodePin CreateInputPin(const std::string& label, PinType type);
	static EditorNodePin CreateOutputPin(const std::string& label, PinType type);
};