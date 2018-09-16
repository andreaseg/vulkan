#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "physical_device.hpp"
#include <vector>
#include <optional>
#include <tuple>

class Queue {
    private:
    VkQueue raw_queue;
    public:
    Queue(VkQueue raw_queue);
};

class TransferQueue: public Queue {
    using Queue::Queue;
};

class GeneralQueue: public Queue {
    using Queue::Queue;
};

class Device {
    public:
    struct Extension {
        const static std::string Swapchain;
        const static std::string Bit16Storage;
        const static std::string BindMemory2;
        const static std::string DedicatedAllocation;
        const static std::string DescriptorUpdateTemplate;
        const static std::string DeviceGroup;
        const static std::string GotMemoryReuirements2;
        const static std::string ImageFormatList;
        const static std::string Maintenance1;
        const static std::string Maintenance2;
        const static std::string Maintenance3;
        const static std::string Multiview;
        const static std::string PushDescriptor;
        const static std::string RelaxedBlockLayout;
        const static std::string SamplerMirrorClampToEdge;
        const static std::string SamplerYCBCRConversion;
        const static std::string ShaderDrawParameters;
        const static std::string SorageBufferStorageClass;
        const static std::string ExternalMemory;
        const static std::string ExternalMemoryWin32;
        const static std::string ExternalSemaphore;
        const static std::string ExternalSemaphoreWin32;
        const static std::string Win32KeyedMutex;
        const static std::string ExternalFence;
        const static std::string ExternalFenceWin32;
        const static std::string VariablePointers;
        const static std::string KHXDeviceGroup;
        const static std::string KHXMultiview;
        const static std::string BlendOperationAdvanced;
        const static std::string DepthRangeUnrestricted;
        const static std::string DiscardRectangles;
        const static std::string ShaderSubgroupBallot;
        const static std::string ShaderSubgroupVote;
        const static std::string NvidiaDedicatedAllocation;
        const static std::string NvidiaExternalMemory;
        const static std::string NvidiaExternalMemoryWin32;
        const static std::string NvidiaGLSLShader;
        const static std::string NvidiaWin32KeyedMutex;
        const static std::string NvidiaXDeviceGeneratedCommands;
        const static std::string NvidiaXMultiviewPerViewAttribures;

        static Result<std::vector<std::string>, VkResult> enumerate(VkPhysicalDevice physical_device);
    };
    private:

    VkDevice raw_device;

    struct BuilderProperties {
        VkPhysicalDevice physical_device;
        std::optional<VkPhysicalDeviceFeatures> features;
        std::vector<const char*> enabled_extensions;
        std::vector<std::string> load_extensions;
        std::vector<std::tuple<QueueFamily, std::vector<float>>> queues;
    };
    public:
    class Builder {
        BuilderProperties builder_properties;
        public:
        Builder(VkPhysicalDevice physical_device);
        Builder& add_extension(const std::string& extension);
        Builder& add_instance_extension(const std::string& extension);
        Builder& add_queue(QueueFamily queue_family, std::vector<float> priorities);
        Builder& enable_features(VkPhysicalDeviceFeatures features);
        Result<Device, VkResult> build();
    };

    Device(VkDevice raw_device);

    Queue get_queue(QueueFamily queue_family, uint32_t index);
    TransferQueue get_queue(TransferQueueFamily queue_family, uint32_t index);
    GeneralQueue get_queue(GeneralQueueFamily queue_family, uint32_t index);

    void destroy();
};



#endif // DEVICE_HPP