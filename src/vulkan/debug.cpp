#include "debug.hpp"
#include "common.hpp"
#include "vulkan_functions.hpp"
#include <iostream>
#include <fstream>

VkBool32 print_debug(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    (void)pUserData;

    std::string severity = "";
    if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) != 0) {
        severity = "Verbose";
    } else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) != 0) {
        severity = "Info";
    } else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) != 0) {
        severity = "Warning";
    } else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) != 0) {
        severity = "Error";
    }

    std::string type = "";
    if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) != 0) {
        type = "General";
    } else if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) != 0) {
        type = "Validation";
    } else if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) != 0) {
        type = "Performance";
    }

    std::cout << type << "/" << severity << ": ";
    
    if (pCallbackData->cmdBufLabelCount > 0) {
        for (size_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
            std::cout << "[" << pCallbackData->pCmdBufLabels[i].pLabelName << "] ";
        }
    }

    if (pCallbackData->queueLabelCount > 0) {
        for (size_t i = 0; i < pCallbackData->queueLabelCount; i++) {
            std::cout << "[" << pCallbackData->pQueueLabels[i].pLabelName << "] ";
        }
    }
    
    std::cout << pCallbackData->pMessageIdName << ", " << pCallbackData->pMessage << std::endl;

    return 0;
}

std::string print_file;

VkBool32 print_debug_to_file(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    (void)pUserData;

    std::ofstream ofile;
    ofile.open(print_file, std::ios::app);

    std::string severity = "";
    if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) != 0) {
        severity = "Verbose";
    } else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) != 0) {
        severity = "Info";
    } else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) != 0) {
        severity = "Warning";
    } else if ((messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) != 0) {
        severity = "Error";
    }

    std::string type = "";
    if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) != 0) {
        type = "General";
    } else if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) != 0) {
        type = "Validation";
    } else if ((messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) != 0) {
        type = "Performance";
    }

    ofile << type << "/" << severity << ": ";
    
    if (pCallbackData->cmdBufLabelCount > 0) {
        for (size_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
            ofile << "[" << pCallbackData->pCmdBufLabels[i].pLabelName << "] ";
        }
    }

    if (pCallbackData->queueLabelCount > 0) {
        for (size_t i = 0; i < pCallbackData->queueLabelCount; i++) {
            ofile << "[" << pCallbackData->pQueueLabels[i].pLabelName << "] ";
        }
    }
    
    ofile << pCallbackData->pMessageIdName << ", " << pCallbackData->pMessage << std::endl;

    ofile.close();

    return 0;
}

Debugger::Builder::Builder(VkInstance instance) {
    builder_properties.raw_instance = instance;
}

Result<Debugger, VkResult> Debugger::Builder::build() {

    VkDebugUtilsMessengerCreateInfoEXT create_info;
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.pNext = nullptr;
    create_info.flags = 0;
    create_info.pUserData = 0;
    create_info.messageSeverity = builder_properties.severity;
    create_info.messageType = builder_properties.message_type;
    if (builder_properties.print_file_path.length() == 0) {
        create_info.pfnUserCallback = print_debug;
    } else {
        print_file = builder_properties.print_file_path;
        std::ofstream ofile;
        ofile.open(print_file, std::ofstream::out | std::ofstream::trunc);
        if (ofile.is_open()) {
            ofile.close();
            create_info.pfnUserCallback = print_debug_to_file;
        } else {
            create_info.pfnUserCallback = print_debug;
        }
        
    }
    

    VkDebugUtilsMessengerEXT messenger;
    VkResult result = vkCreateDebugUtilsMessengerEXT(builder_properties.raw_instance, &create_info, nullptr, &messenger);
    if (VkResult_is_err(result)) {
        return Result<Debugger, VkResult>(result);
    }

    return Result<Debugger, VkResult>(Debugger(messenger));
}

Debugger::Debugger(VkDebugUtilsMessengerEXT messenger) : messenger(messenger) {}

Debugger::Builder& Debugger::Builder::severity_verbose() {
    builder_properties.severity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    return *this;
}

Debugger::Builder& Debugger::Builder::severity_info() {
    builder_properties.severity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
    return *this;
}

Debugger::Builder& Debugger::Builder::severity_warning() {
    builder_properties.severity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    return *this;
}

Debugger::Builder& Debugger::Builder::severity_error() {
    builder_properties.severity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    return *this;
}

Debugger::Builder& Debugger::Builder::type_general() {
    builder_properties.message_type |= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
    return *this;
}

Debugger::Builder& Debugger::Builder::type_validation() {
    builder_properties.message_type |= VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    return *this;
}

Debugger::Builder& Debugger::Builder::type_performance() {
    builder_properties.message_type |= VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    return *this;
}

Debugger::Builder& Debugger::Builder::severity_all() {
    builder_properties.severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    return *this;
}

Debugger::Builder& Debugger::Builder::type_all() {
    builder_properties.message_type = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
        | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    return *this;
}

Debugger::Builder& Debugger::Builder::write_to_file(std::string path) {
    builder_properties.print_file_path = path;
    return *this;
}