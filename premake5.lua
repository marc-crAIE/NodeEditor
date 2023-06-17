include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "NodeEditor"
	architecture "x86_64"
	startproject "NodeEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "NodeEditor/vendor/GLFW"
	include "NodeEditor/vendor/Glad"
	include "NodeEditor/vendor/imgui"
group ""

group "Core"
	include "NodeEditor"
group ""