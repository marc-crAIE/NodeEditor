#pragma once
#include "Core/Base.h"
#include "Core/Log.h"
#include <filesystem>

#ifdef NE_ENABLE_ASSERTS
// These assert macros are not supposed to be used and are only helpers for NE_ASSERT and NE_CORE_ASSERT

#define NE_INTERNAL_ASSERT_IMPL(check, msg, ...) { if(!(check)) { NE_ERROR(msg, __VA_ARGS__); NE_DEBUGBREAK(); } }
#define NE_INTERNAL_ASSERT_WITH_MSG(check, ...) NE_INTERNAL_ASSERT_IMPL(check, "Assertion failed: {0}", __VA_ARGS__)
#define NE_INTERNAL_ASSERT_NO_MSG(check) NE_INTERNAL_ASSERT_IMPL(check, "Assertion '{0}' failed at {1}:{2}", NE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define NE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define NE_INTERNAL_ASSERT_GET_MACRO(...) NE_EXPAND_MACRO( NE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, NE_INTERNAL_ASSERT_WITH_MSG, NE_INTERNAL_ASSERT_NO_MSG) )

// These assert macros are supposed to be used, CORE is intended for internal engine stuff

#define NE_ASSERT(...) NE_EXPAND_MACRO( NE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(__VA_ARGS__) )

#define NOT_IMPLEMENTED NE_ASSERT(0, "Not implemented!")
#else
// Defines them but gives them no use

#define NE_ASSERT(...)
#define NE_CORE_ASSERT(...)
#define NOT_IMPLEMENTED
#endif