#include <iostream>
#include "vulkan/instance.hpp"
#include "vulkan/physical_device.hpp"
#ifdef WINDOWS
#include <windows.h>
#endif

int main() {

    
    auto instance = Instance::Builder()
        .add_layer(Instance::Layer::StandardValidation)
        //.add_layer(Instance::Layer::RenderDocCapture)
        .add_extension(Instance::Extension::Surface)
        .add_extension(Instance::Extension::Win32Surface)
        .build()
        .expect("Unable to create instance");

    auto physical_device = PhysicalDevice::Builder(instance).build().expect("Unable to pick physical device");

    auto [general_queue_family, transfer_queue_family] = QueueFamily::Builder(physical_device).build();

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