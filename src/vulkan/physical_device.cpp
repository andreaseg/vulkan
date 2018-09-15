#include "physical_device.hpp"
#include "vulkan_functions.hpp"
#include "common.hpp"
#include <vector>
#include <algorithm>

PhysicalDevice::PhysicalDevice(VkPhysicalDevice raw_physical_device) : raw_physical_device(raw_physical_device) {}

PhysicalDevice::Builder::Builder(Instance instance) {
    builder_properties.raw_instance = instance;
}

Result<PhysicalDevice, VkResult> PhysicalDevice::Builder::build() {
    uint32_t count = 0;
    {
        VkResult result = vkEnumeratePhysicalDevices(builder_properties.raw_instance, &count, nullptr);
        if (VkResult_is_err(result)) {
            return Result<PhysicalDevice, VkResult>(result);
        }
    }
    std::vector<VkPhysicalDevice> raw_physical_devices(count);
    {
        VkResult result = vkEnumeratePhysicalDevices(builder_properties.raw_instance, &count, &raw_physical_devices[0]);
        if (VkResult_is_err(result)) {
            return Result<PhysicalDevice, VkResult>(result);
        }
    }

    return Result<PhysicalDevice, VkResult>(PhysicalDevice(raw_physical_devices[0]));
}

QueueFamily::QueueFamily(uint32_t index, uint32_t count): index(index), count(count) {}

uint32_t QueueFamily::get_index() {
    return index;
}

uint32_t QueueFamily::get_count() {
    return count;
}

QueueFamily::Builder::Builder(PhysicalDevice physical_device) {
    builder_properties.raw_physical_device = physical_device;
}

std::tuple<GeneralQueueFamily, TransferQueueFamily> QueueFamily::Builder::build() {
    if (vkGetPhysicalDeviceQueueFamilyProperties == nullptr) {
    std::cerr << "missing" << std::endl;
    }
    if (vkGetInstanceProcAddr == nullptr) {
        std::cerr << "missing2" << std::endl;
    }
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(builder_properties.raw_physical_device, &count, nullptr);
    std::vector<VkQueueFamilyProperties> properties(count);
    vkGetPhysicalDeviceQueueFamilyProperties(builder_properties.raw_physical_device, &count, &properties[0]);

    const uint32_t uint32_max = 4294967295;

    uint32_t g_index = uint32_max;
    uint32_t t_index = uint32_max;

    for (uint32_t i = 0; i < count; i++) {
        if ((properties[i].queueFlags & (VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT | VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)) != 0) {
            g_index = i;
        }
        if ((properties[i].queueFlags & (VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT)) != 0) {
            t_index = i;
        }
    }

    if (g_index == uint32_max || t_index == uint32_max) {
        std::cerr << "Missing queues" << std::endl;
    }

    return {GeneralQueueFamily(g_index, properties[g_index].queueCount), TransferQueueFamily(t_index, properties[t_index].queueCount)};
}