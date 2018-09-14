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

#define VK_FUNCTION(name) \
static PFN_##name name;

// Global
VK_FUNCTION(vkGetInstanceProcAddr)
VK_FUNCTION(vkEnumerateInstanceExtensionProperties)
VK_FUNCTION(vkEnumerateInstanceLayerProperties)
VK_FUNCTION(vkCreateInstance)
VK_FUNCTION(vkDestroyInstance)

// Instance level

#endif // VK_INCLUDE_HPP