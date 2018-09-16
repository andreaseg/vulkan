#include "device.hpp"
#include "vulkan_functions.hpp"
#include "common.hpp"

const std::string Device::Extension::Swapchain = "VK_KHR_swapchain";
const std::string Device::Extension::Bit16Storage = "VK_KHR_16bit_storage";
const std::string Device::Extension::BindMemory2  = "VK_KHR_bind_memory2";
const std::string Device::Extension::DedicatedAllocation  = "VK_KHR_dedicated_allocation";
const std::string Device::Extension::DescriptorUpdateTemplate = "VK_KHR_descriptor_update_template";
const std::string Device::Extension::DeviceGroup  = "VK_KHR_device_group";
const std::string Device::Extension::GotMemoryReuirements2  = "VK_KHR_get_memory_requirements2";
const std::string Device::Extension::ImageFormatList = "VK_KHR_image_format_list";
const std::string Device::Extension::Maintenance1 = "VK_KHR_maintenance1";
const std::string Device::Extension::Maintenance2  = "VK_KHR_maintenance2";
const std::string Device::Extension::Maintenance3  = "VK_KHR_maintenance3";
const std::string Device::Extension::Multiview = "VK_KHR_multiview";
const std::string Device::Extension::PushDescriptor = "VK_KHR_push_descriptor";
const std::string Device::Extension::RelaxedBlockLayout = "VK_KHR_relaxed_block_layout";
const std::string Device::Extension::SamplerMirrorClampToEdge = "VK_KHR_sampler_mirror_clamp_to_edge";
const std::string Device::Extension::SamplerYCBCRConversion = "VK_KHR_sampler_ycbcr_conversion";
const std::string Device::Extension::ShaderDrawParameters = "VK_KHR_shader_draw_parameters";
const std::string Device::Extension::SorageBufferStorageClass = "VK_KHR_storage_buffer_storage_class";
const std::string Device::Extension::ExternalMemory = "VK_KHR_external_memory";
const std::string Device::Extension::ExternalMemoryWin32 = "VK_KHR_external_memory_win32";
const std::string Device::Extension::ExternalSemaphore = "VK_KHR_external_semaphore";
const std::string Device::Extension::ExternalSemaphoreWin32 = "VK_KHR_external_semaphore_win32";
const std::string Device::Extension::Win32KeyedMutex = "VK_KHR_win32_keyed_mutex";
const std::string Device::Extension::ExternalFence = "VK_KHR_external_fence";
const std::string Device::Extension::ExternalFenceWin32 = "VK_KHR_external_fence_win32";
const std::string Device::Extension::VariablePointers = "VK_KHR_variable_pointers";
const std::string Device::Extension::KHXDeviceGroup = "VK_KHX_device_group";
const std::string Device::Extension::KHXMultiview = "VK_KHX_multiview";
const std::string Device::Extension::BlendOperationAdvanced = "VK_EXT_blend_operation_advanced";
const std::string Device::Extension::DepthRangeUnrestricted = "VK_EXT_depth_range_unrestricted";
const std::string Device::Extension::DiscardRectangles = "VK_EXT_discard_rectangles";
const std::string Device::Extension::ShaderSubgroupBallot = "VK_EXT_shader_subgroup_ballot";
const std::string Device::Extension::ShaderSubgroupVote = "VK_EXT_shader_subgroup_vote";
const std::string Device::Extension::NvidiaDedicatedAllocation = "VK_NV_dedicated_allocation";
const std::string Device::Extension::NvidiaExternalMemory = "VK_NV_external_memory";
const std::string Device::Extension::NvidiaExternalMemoryWin32 = "VK_NV_external_memory_win32";
const std::string Device::Extension::NvidiaGLSLShader = "VK_NV_glsl_shader";
const std::string Device::Extension::NvidiaWin32KeyedMutex = "VK_NV_win32_keyed_mutex";
const std::string Device::Extension::NvidiaXDeviceGeneratedCommands = "VK_NVX_device_generated_commands";
const std::string Device::Extension::NvidiaXMultiviewPerViewAttribures = "VK_NVX_multiview_per_view_attributes";

#define VK_DEVICE_FUNCTION(name) \
name = (PFN_##name) vkGetDeviceProcAddr(device, #name);\
if (name == nullptr) {\
    std::cerr << "Unable to load device level function: " #name << std::endl;\
}

#define VK_EXTENSION_FUNCTION(name, extension) \
for (auto& n: extension_names) {\
    if (std::string(extension) == n) {\
        name = (PFN_##name) vkGetDeviceProcAddr(device, #name);\
        if (name == nullptr) {\
            std::cerr << "Unable to load device level extension function: " #name " from " << std::string(extension) << std::endl;\
        }\
    }\
}

void load_device_level_functions(VkDevice device, std::vector<std::string> extension_names) {
    #include "vulkan_functions.inl"
}

Result<std::vector<std::string>, VkResult> Device::Extension::enumerate(VkPhysicalDevice physical_device) {
    uint32_t count = 0;
    {
        VkResult result = vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &count, nullptr);
        if (VkResult_is_err(result)) {
            return Result<std::vector<std::string>, VkResult>(result);
        }
    }
    std::vector<VkExtensionProperties> extensions(count);
    {
        VkResult result = vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &count, &extensions[0]);
        if (VkResult_is_err(result)) {
            return Result<std::vector<std::string>, VkResult>(result);
        }
    }
    std::vector<std::string> result(count);
    for (size_t i = 0; i < count; ++i) {
        result[i] = std::string(extensions[i].extensionName);
    }
    return Result<std::vector<std::string>, VkResult>(result);
}

Device::Builder::Builder(VkPhysicalDevice physical_device) {
    builder_properties.physical_device = physical_device;
}

Device::Builder& Device::Builder::add_extension(const std::string& extension) {
    builder_properties.enabled_extensions.push_back(&extension[0]);
    builder_properties.load_extensions.push_back(extension);
    return *this;
}

Device::Builder& Device::Builder::add_instance_extension(const std::string& extension) {
    builder_properties.load_extensions.push_back(extension);
    return *this;
}

Device::Builder& Device::Builder::add_queue(QueueFamily queue_family, std::vector<float> priorities) {
    builder_properties.queues.push_back({queue_family, priorities});
    return *this;
}

Device::Builder& Device::Builder::enable_features(VkPhysicalDeviceFeatures features) {
    builder_properties.features = features;
    return *this;
}

Result<Device, VkResult> Device::Builder::build() {
    VkDeviceCreateInfo create_info;
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pNext = nullptr;
    create_info.flags = 0;
    create_info.enabledLayerCount = 0;
    create_info.ppEnabledLayerNames = nullptr;

    create_info.enabledExtensionCount = builder_properties.enabled_extensions.size();
    create_info.ppEnabledExtensionNames 
        = (builder_properties.enabled_extensions.size() > 0) ? &builder_properties.enabled_extensions[0] : nullptr;

    if (builder_properties.features.has_value()) {
        create_info.pEnabledFeatures = &builder_properties.features.value();
    } else {
        create_info.pEnabledFeatures = nullptr;
    }

    create_info.queueCreateInfoCount = builder_properties.queues.size();
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos(builder_properties.queues.size());
    
    for (size_t i = 0; i < builder_properties.queues.size(); i++) {
        queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[i].pNext = nullptr;
        queue_create_infos[i].queueFamilyIndex = std::get<0>(builder_properties.queues[i]).get_index();
        queue_create_infos[i].flags = 0;
        queue_create_infos[i].queueCount = min(std::get<1>(builder_properties.queues[i]).size(), builder_properties.queues.size());
        queue_create_infos[i].pQueuePriorities = &std::get<1>(builder_properties.queues[i])[0];
    }

    create_info.pQueueCreateInfos = &queue_create_infos[0];

    VkDevice raw_device;
    VkResult result = vkCreateDevice(builder_properties.physical_device, &create_info, nullptr, &raw_device);
    if (VkResult_is_err(result)) {
        return Result<Device, VkResult>(result);
    }

    load_device_level_functions(raw_device, builder_properties.load_extensions);

    return Result<Device, VkResult>(Device(raw_device));
}

Device::Device(VkDevice raw_device): raw_device(raw_device) {}

void Device::destroy() {
    vkDestroyDevice(raw_device, nullptr);
    raw_device = nullptr;
}

Queue::Queue(VkQueue raw_queue) : raw_queue(raw_queue) {}

inline VkQueue get_raw_queue(VkDevice device, QueueFamily queue_family, uint32_t index) {
    VkQueue raw_queue;
    vkGetDeviceQueue(device, queue_family.get_index(), index, &raw_queue);
    return raw_queue;
}

Queue Device::get_queue(QueueFamily queue_family, uint32_t index) {
    return Queue(get_raw_queue(raw_device, queue_family, index));
}

TransferQueue Device::get_queue(TransferQueueFamily queue_family, uint32_t index) {
    return TransferQueue(get_raw_queue(raw_device, queue_family, index));
}

GeneralQueue Device::get_queue(GeneralQueueFamily queue_family, uint32_t index) {
    return GeneralQueue(get_raw_queue(raw_device, queue_family, index));
}