#ifndef VK_ENTRY_FUNCTION
#define VK_ENTRY_FUNCTION(name)
#endif

VK_ENTRY_FUNCTION(vkGetInstanceProcAddr)

#undef VK_ENTRY_FUNCTION

#ifndef VK_GLOBAL_FUNCTION
#define VK_GLOBAL_FUNCTION(name)
#endif

VK_GLOBAL_FUNCTION(vkEnumerateInstanceExtensionProperties)
VK_GLOBAL_FUNCTION(vkEnumerateInstanceLayerProperties)
VK_GLOBAL_FUNCTION(vkCreateInstance)
VK_GLOBAL_FUNCTION(vkDestroyInstance)

#undef VK_GLOBAL_FUNCTION

#ifndef VK_INSTANCE_FUNCTION
#define VK_INSTANCE_FUNCTION(name)
#endif

VK_INSTANCE_FUNCTION(vkEnumeratePhysicalDevices)
VK_INSTANCE_FUNCTION(vkGetPhysicalDeviceProperties)
VK_INSTANCE_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties)
VK_INSTANCE_FUNCTION(vkCreateDevice)

#undef VK_INSTANCE_FUNCTION

#ifndef VK_DEVICE_FUNCTION
#define VK_DEVICE_FUNCTION(name)
#endif

VK_DEVICE_FUNCTION(vkDestroyDevice)
VK_DEVICE_FUNCTION(vkGetDeviceQueue)

#undef VK_DEVICE_FUNCTION