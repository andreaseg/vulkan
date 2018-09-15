#include "physical_device.hpp"
#include "vulkan_functions.hpp"
#include <vector>
#include <algorithm>

PhysicalDevice::PhysicalDevice(VkPhysicalDevice raw_physical_device) : raw_physical_device(raw_physical_device) {}

std::tuple<GeneralQueueFamily, TransferQueueFamily> PhysicalDevice::pick_queue_families() {
    if (vkGetPhysicalDeviceQueueFamilyProperties == nullptr) {
        std::cerr << "missing" << std::endl;
    }
    if (vkGetInstanceProcAddr == nullptr) {
        std::cerr << "missing2" << std::endl;
    }
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(raw_physical_device, &count, nullptr);
    std::vector<VkQueueFamilyProperties> properties(count);
    vkGetPhysicalDeviceQueueFamilyProperties(raw_physical_device, &count, &properties[0]);

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

QueueFamily::QueueFamily(uint32_t index, uint32_t count): index(index), count(count) {}

uint32_t QueueFamily::get_index() {
    return index;
}

uint32_t QueueFamily::get_count() {
    return count;
}