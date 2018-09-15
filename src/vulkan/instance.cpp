#include "instance.hpp"
#include "vulkan_functions.hpp"
#include <iostream>

const std::string Instance::Layer::RenderDocCapture = "VK_LAYER_RENDERDOC_Capture";
const std::string Instance::Layer::StandardValidation = "VK_LAYER_LUNARG_standard_validation";

const std::string Instance::Extension::DebugReport = "VK_EXT_debug_report";
const std::string Instance::Extension::DebugUtils = "VK_EXT_debug_utils";
const std::string Instance::Extension::Surface = "VK_KHR_surface";
const std::string Instance::Extension::Win32Surface = "VK_KHR_win32_surface";

#ifdef WINDOWS
#define LoadFunction GetProcAddress
#else
#define LoadFunction dlsym
#endif

#define VK_ENTRY_FUNCTION(name)\
name = (PFN_##name) LoadFunction(VULKAN_LIBRARY, #name);\
if (name == nullptr) {\
    std::cerr << "Unable to load entry function: " #name << std::endl;\
}

#define VK_GLOBAL_FUNCTION(name) \
name = (PFN_##name) vkGetInstanceProcAddr(nullptr, #name);\
if (name == nullptr) {\
    std::cerr << "Unable to load global function: " #name << std::endl;\
}

void load_global_functions() {
    if (VULKAN_LIBRARY != nullptr) return;

    #ifdef WINDOWS
    VULKAN_LIBRARY = LoadLibraryA("vulkan-1.dll");
    #else
    VULKAN_LIBRARY = dlopen("libvulkan.so.1", RTLD_NOW);
    #endif
    if (VULKAN_LIBRARY == nullptr) {
        std::cerr << "Unable to load vulkan library" << std::endl;
    }

    #include "vulkan_functions.inl"

    std::cout << "Loaded global vulkan functions" << std::endl;
    
}

#define VK_INSTANCE_FUNCTION(name) \
name = (PFN_##name) vkGetInstanceProcAddr(instance, #name);\
if (name == nullptr) {\
    std::cerr << "Unable to load instance level function: " #name << std::endl;\
}

void load_instance_level_functions(VkInstance instance) {
    #include "vulkan_functions.inl"
    std::cout << "Loaded instance level vulkan functions" << std::endl;
}


Result<std::vector<std::string>, VkResult> Instance::enumerate_layers() {
    load_global_functions();

    uint32_t count = 0;
    {
        VkResult result = vkEnumerateInstanceLayerProperties(&count, nullptr);
        if (VkResult_is_err(result)) {
            return Result<std::vector<std::string>, VkResult>(result);
        }
    }
    std::vector<VkLayerProperties> layers(count);
    {
        VkResult result = vkEnumerateInstanceLayerProperties(&count, &layers[0]);
        if (VkResult_is_err(result)) {
            return Result<std::vector<std::string>, VkResult>(result);
        }
    }
    std::vector<std::string> result(count);
    for (size_t i = 0; i < count; ++i) {
        result[i] = std::string(layers[i].layerName);
    }
    return Result<std::vector<std::string>, VkResult>(result);
}


Result<std::vector<std::string>, VkResult> Instance::enumerate_extensions() {
    load_global_functions();

    uint32_t count = 0;
    {
        VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
        if (VkResult_is_err(result)) {
            return Result<std::vector<std::string>, VkResult>(result);
        }
    }
    std::vector<VkExtensionProperties> layers(count);
    {
        VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &count, &layers[0]);
        if (VkResult_is_err(result)) {
            return Result<std::vector<std::string>, VkResult>(result);
        }
    }
    std::vector<std::string> result(count);
    for (size_t i = 0; i < count; ++i) {
        result[i] = std::string(layers[i].extensionName);
    }
    return Result<std::vector<std::string>, VkResult>(result);
}

Instance::Instance(VkInstance instance) : raw_instance(instance) {}

Instance::Builder::Builder() {

}

Instance::Builder& Instance::Builder::add_extension(const std::string& name) {
    properties.enabled_extensions.push_back(&name[0]);
    return *this;
}

Instance::Builder& Instance::Builder::add_layer(const std::string& name) {
    properties.enabled_layers.push_back(&name[0]);
    return *this;
}

Instance::Builder& Instance::Builder::app_name(std::string name) {
    properties.app_name = name;
    return *this;
}

Instance::Builder& Instance::Builder::app_version(uint32_t major, uint32_t minor, uint32_t patch) {
    properties.app_version = VK_MAKE_VERSION(major, minor, patch);
    return *this;
}

Instance::Builder& Instance::Builder::engine_name(std::string name) {
    properties.engine_name = name;
    return *this;
}

Instance::Builder& Instance::Builder::engine_version(uint32_t major, uint32_t minor, uint32_t patch) {
    properties.engine_version = VK_MAKE_VERSION(major, minor, patch);
    return *this;
}

Instance::Builder& Instance::Builder::api_version(uint32_t major, uint32_t minor, uint32_t patch) {
    properties.api_version = VK_MAKE_VERSION(major, minor, patch);
    return *this;
}

Result<Instance, VkResult> Instance::Builder::build() {
    load_global_functions();

    VkApplicationInfo app_info;
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = nullptr;
    app_info.apiVersion = properties.api_version.value_or(VK_MAKE_VERSION(1, 1, 0));
    app_info.applicationVersion = properties.app_version.value_or(VK_MAKE_VERSION(0, 1, 0));
    app_info.engineVersion = properties.engine_version.value_or(VK_MAKE_VERSION(0, 1, 0));
    app_info.pApplicationName = properties.app_name.value_or("Vulkan application").c_str();
    app_info.pEngineName = properties.app_name.value_or("Vulkan engine").c_str();

    VkInstanceCreateInfo create_info;
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pNext = nullptr;
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = properties.enabled_extensions.size();
    create_info.ppEnabledExtensionNames = (properties.enabled_extensions.size() > 0) ? &properties.enabled_extensions[0] : nullptr;
    create_info.enabledLayerCount = properties.enabled_layers.size();
    create_info.ppEnabledLayerNames = (properties.enabled_layers.size() > 0) ? &properties.enabled_layers[0] : nullptr;

    std::cout << "Creating instance with " << properties.enabled_extensions.size() << " extensions" << std::endl;
    for (auto &ext : properties.enabled_extensions) {
        std::cout << ext << std::endl;
    }
    std::cout << "Creating instance with " << properties.enabled_layers.size() << " layers" << std::endl;
    for (auto &ext : properties.enabled_layers) {
        std::cout << ext << std::endl;
    }

    VkInstance raw_instance;
    VkResult result = vkCreateInstance(&create_info, nullptr, &raw_instance);

    if (VkResult_is_err(result)) {
        return Result<Instance, VkResult>(result);
    }

    load_instance_level_functions(raw_instance);

    return Result<Instance, VkResult>(Instance(raw_instance));
}

void Instance::destroy() {
    vkDestroyInstance(raw_instance, nullptr);
    raw_instance = nullptr;

    #ifdef WINDOWS
    FreeLibrary(VULKAN_LIBRARY);
    #else
    dlclose(VULKAN_LIBRARY);
    #endif
    VULKAN_LIBRARY = nullptr;
}

Result<PhysicalDevice, VkResult> Instance::pick_physical_device() {
    uint32_t count = 0;
    {
        VkResult result = vkEnumeratePhysicalDevices(raw_instance, &count, nullptr);
        if (VkResult_is_err(result)) {
            return Result<PhysicalDevice, VkResult>(result);
        }
    }
    std::vector<VkPhysicalDevice> raw_physical_devices(count);
    {
        VkResult result = vkEnumeratePhysicalDevices(raw_instance, &count, &raw_physical_devices[0]);
        if (VkResult_is_err(result)) {
            return Result<PhysicalDevice, VkResult>(result);
        }
    }

    return Result<PhysicalDevice, VkResult>(PhysicalDevice(raw_physical_devices[0]));
}