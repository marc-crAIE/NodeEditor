#pragma once

#include <unordered_map>

#include "Core/Base.h"

struct ExecutorVariables
{
	std::unordered_map<uint32_t, bool> Bools;
	std::unordered_map<uint32_t, float> Floats;
	std::unordered_map<uint32_t, Float2> Float2s;
	std::unordered_map<uint32_t, Float3> Float3s;
	std::unordered_map<uint32_t, Float4> Float4s;

	template<typename T> std::unordered_map<uint32_t, T>& GetMapFromType();
	template<> std::unordered_map<uint32_t, bool>& GetMapFromType<bool>() { return Bools; }
	template<> std::unordered_map<uint32_t, float>& GetMapFromType<float>() { return Floats; }
	template<> std::unordered_map<uint32_t, Float2>& GetMapFromType<Float2>() { return Float2s; }
	template<> std::unordered_map<uint32_t, Float3>& GetMapFromType<Float3>() { return Float3s; }
	template<> std::unordered_map<uint32_t, Float4>& GetMapFromType<Float4>() { return Float4s; }
};

struct ExecuteContext
{
	ExecutorVariables Variables;

	bool Failure = false;
};

class ExecutorNode;

struct CompiledPipeline
{
	ExecutorNode* OnStartNode = nullptr;
	ExecutorNode* OnUpdateNode = nullptr;
};