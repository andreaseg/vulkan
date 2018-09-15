#ifndef VK_INCLUDE_HPP
#define VK_INCLUDE_HPP

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#ifdef WINDOWS
#include <windows.h>
static HMODULE VULKAN_LIBRARY = nullptr;
#else
static void* VULKAN_LIBRARY = nullptr;
#endif

#endif // VK_INCLUDE_HPP