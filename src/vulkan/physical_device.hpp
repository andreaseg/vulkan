#ifndef PHYSICAL_DEVICE_HPP
#define PHYSICAL_DEVICE_HPP

#include "include.hpp"
#include "result.hpp"
#include <tuple>

class QueueFamily {
    private:
    uint32_t index;
    uint32_t count;
    public:
    QueueFamily(uint32_t index, uint32_t count);
    uint32_t get_index();
    uint32_t get_count();
};

// Supports transfer
class TransferQueueFamily: public QueueFamily {
    using QueueFamily::QueueFamily;
};

// Supports compute + graphics
class GeneralQueueFamily: public QueueFamily {
    using QueueFamily::QueueFamily;
};

class PhysicalDevice {
    private:
    VkPhysicalDevice raw_physical_device;
    public:
    PhysicalDevice(VkPhysicalDevice raw_physical_device);
    std::tuple<GeneralQueueFamily, TransferQueueFamily> pick_queue_families();
};

#endif // PHYSICAL_DEVICE_HPP