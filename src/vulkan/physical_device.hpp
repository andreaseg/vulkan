#ifndef PHYSICAL_DEVICE_HPP
#define PHYSICAL_DEVICE_HPP

#include "include.hpp"
#include "result.hpp"
#include <tuple>

class PhysicalDevice {
    private:
    VkPhysicalDevice raw_physical_device;
    struct BuilderProperties {
        VkInstance raw_instance;
    };
    public:
    class Builder {
        private:
        BuilderProperties builder_properties;
        public:
        Builder(VkInstance instance);

        Result<PhysicalDevice, VkResult> build();
    };

    operator VkPhysicalDevice() {return raw_physical_device;}

    PhysicalDevice(VkPhysicalDevice raw_physical_device);
};

struct TransferQueueFamily;
struct GeneralQueueFamily;

class QueueFamily {
    private:
    struct BuilderProperties {
        VkPhysicalDevice raw_physical_device;
    };
    protected:
    uint32_t index;
    uint32_t count;
    uint32_t flags;
    public:
    QueueFamily(uint32_t index, uint32_t count);
    uint32_t get_index();
    uint32_t get_count();
    uint32_t get_flags();

    class Builder {
        private:
        BuilderProperties builder_properties;
        public:
        Builder(PhysicalDevice physical_device);

        std::tuple<GeneralQueueFamily, TransferQueueFamily> build();
    };
};

// Supports transfer
struct TransferQueueFamily: public QueueFamily {
    TransferQueueFamily(uint32_t index, uint32_t count);
};

// Supports compute + graphics
struct GeneralQueueFamily: public QueueFamily {
    GeneralQueueFamily(uint32_t index, uint32_t count);
};


#endif // PHYSICAL_DEVICE_HPP