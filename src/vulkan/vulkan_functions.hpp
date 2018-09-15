#ifndef VK_FUNCTIONS_HPP
#define VK_FUNCTIONS_HPP

#include "include.hpp"

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