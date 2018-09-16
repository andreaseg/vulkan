#ifndef VK_DEBUG_HPP
#define VK_DEBUG_HPP

#include "include.hpp"
#include "result.hpp"

class Debugger {
    private:
    VkDebugUtilsMessengerEXT messenger;
    struct BuilderProperties {
        VkInstance raw_instance;
        uint32_t severity;
        uint32_t message_type;
    };
    public:
    class Builder {
        BuilderProperties builder_properties;
        public:
        Builder(VkInstance instance);
        Builder& severity_verbose();
        Builder& severity_info();
        Builder& severity_warning();
        Builder& severity_error();
        Builder& type_general();
        Builder& type_validation();
        Builder& type_performance();
        Builder& severity_all();
        Builder& type_all();
        Result<Debugger, VkResult> build();
    };
    operator VkDebugUtilsMessengerEXT() {return messenger;}
    Debugger(VkDebugUtilsMessengerEXT messenger);
};

#endif // VK_DEBUG_HPP