#include <iostream>
#include "vulkan/instance.hpp"
#include "vulkan/physical_device.hpp"
#include "vulkan/device.hpp"
#include "vulkan/debug.hpp"
#ifdef WINDOWS
#include <windows.h>
#endif

int main() {

    
    auto instance = Instance::Builder()
        .add_layer(Instance::Layer::StandardValidation)
        //.add_layer(Instance::Layer::RenderDocCapture)
        .add_extension(Instance::Extension::DebugUtils)
        .add_extension(Instance::Extension::Surface)
        .add_extension(Instance::Extension::Win32Surface)
        .build()
        .expect("Unable to create instance");
    
    auto debugger = Debugger::Builder(instance)
        //.severity_error()
        //.severity_warning()
        .severity_all()
        .type_all()
        .write_to_file("vk_log.txt")
        .build()
        .expect("Unable to create debugger");

    auto physical_device = PhysicalDevice::Builder(instance).build().expect("Unable to pick physical device");

    auto [general_queue_family, transfer_queue_family] = QueueFamily::Builder(physical_device).build();

    auto device = Device::Builder(physical_device)
        .add_extension(Device::Extension::Swapchain)
        .add_instance_extension(Instance::Extension::DebugUtils)
        .add_instance_extension(Instance::Extension::Surface)
        .add_instance_extension(Instance::Extension::Win32Surface)
        .add_queue(general_queue_family, {1})
        .add_queue(transfer_queue_family, {1})
        .build()
        .expect("Unable to create device");

    auto transfer_queue = device.get_queue(transfer_queue_family, 0);
    auto general_queue = device.get_queue(general_queue_family, 0);

    device.destroy();
    instance.destroy(debugger);
    instance.destroy();

    
    std::cout << "ok" << std::endl;
    return 0;
}

#ifdef WINDOWS
// Windows entry-point
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;
    std::cout << "Using windows entry-point" << std::endl;
    return main();
}
#else
#endif