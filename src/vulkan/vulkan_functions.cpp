#include "vulkan_functions.hpp"

#define VK_ENTRY_FUNCTION(name)\
PFN_##name name;

#define VK_GLOBAL_FUNCTION(name)\
PFN_##name name;

#define VK_INSTANCE_FUNCTION(name)\
PFN_##name name;

#define VK_DEVICE_FUNCTION(name)\
PFN_##name name;

#define VK_INSTANCE_EXTENSION_FUNCTION(name, extension)\
PFN_##name name;

#define VK_EXTENSION_FUNCTION(name, extension)\
PFN_##name name;

#include "vulkan_functions.inl"