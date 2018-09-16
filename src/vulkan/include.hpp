#ifndef VK_INCLUDE_HPP
#define VK_INCLUDE_HPP

#ifdef WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#endif // VK_INCLUDE_HPP