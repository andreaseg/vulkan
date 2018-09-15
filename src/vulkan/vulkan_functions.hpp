#ifndef VK_FUNCTIONS_HPP
#define VK_FUNCTIONS_HPP

#include "include.hpp"

#ifdef WINDOWS
#include <windows.h>
static HMODULE VULKAN_LIBRARY = nullptr;
#else
static void* VULKAN_LIBRARY = nullptr;
#endif

#define VK_ENTRY_FUNCTION(name)\
extern PFN_##name name;

#define VK_GLOBAL_FUNCTION(name)\
extern PFN_##name name;

#define VK_INSTANCE_FUNCTION(name)\
extern PFN_##name name;

#define VK_DEVICE_FUNCTION(name)\
extern PFN_##name name;

#include "vulkan_functions.inl"

#endif // VK_FUNCTIONS_HPP