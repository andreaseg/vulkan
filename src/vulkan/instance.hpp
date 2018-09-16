#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <string>
#include <vector>
#include <optional>
#include "include.hpp"
#include "result.hpp"


class Instance {
    public:
    struct Layer {
        const static std::string RenderDocCapture;
        const static std::string StandardValidation;

        static Result<std::vector<std::string>, VkResult> enumerate();

        ~Layer() = delete;
    };

    struct Extension {
        const static std::string DebugReport;
        const static std::string DebugUtils;
        const static std::string Surface;
        const static std::string Win32Surface;

        static Result<std::vector<std::string>, VkResult> enumerate();

        ~Extension() = delete;
    };

    private:

    struct Properties {
        std::optional<std::string> app_name;
        std::optional<uint32_t> app_version;
        std::optional<std::string> engine_name;
        std::optional<uint32_t> engine_version;
        std::optional<uint32_t> api_version;
        std::vector<const char*> enabled_layers;
        std::vector<const char*> enabled_extensions;
    };

    VkInstance raw_instance;

    Instance(VkInstance instance);

    public:

    operator VkInstance() {return raw_instance;}

    class Builder {
        private:
        Properties properties;
        public:
        Builder();
        Builder& add_extension(const std::string& extension);
        Builder& add_layer(const std::string& layer);
        Builder& app_name(std::string name);
        Builder& app_version(uint32_t major, uint32_t minor, uint32_t patch);
        Builder& engine_name(std::string name);
        Builder& engine_version(uint32_t major, uint32_t minor, uint32_t patch);
        Builder& api_version(uint32_t major, uint32_t minor, uint32_t patch);
        Result<Instance, VkResult> build();
    };

    void destroy();

    void destroy(VkDebugUtilsMessengerEXT messenger);
    
};

#endif // INSTANCE_HPP