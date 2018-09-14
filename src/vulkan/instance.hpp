#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <string>
#include <vector>
#include <optional>
#include "include.hpp"

class Instance {
    public:
        struct Layer {
            const static std::string RenderDocCapture;
            const static std::string StandardValidation;

            ~Layer() = delete;
        };

        struct Extension {
            const static std::string DegbgReport;
            const static std::string DebugUtils;
            const static std::string Surface;
            const static std::string Win32Surface;

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

    class Builder {
        private:
        Properties properties;
        public:
        Builder();
        Builder& add_extension(std::string extension);
        Builder& add_layer(std::string layer);
        Builder& app_name(std::string name);
        Builder& app_version(uint32_t major, uint32_t minor, uint32_t patch);
        Builder& engine_name(std::string name);
        Builder& engine_version(uint32_t major, uint32_t minor, uint32_t patch);
        Builder& api_version(uint32_t major, uint32_t minor, uint32_t patch);
        Instance build();
    };

    static std::vector<std::string> enumerate_extensions();

    static std::vector<std::string> enumerate_layers();

    void destroy();

    
};

#endif // INSTANCE_HPP