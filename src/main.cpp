#include <iostream>
#include "vulkan/instance.hpp"
#ifdef WINDOWS
#include <windows.h>
#endif

int main() {

    
    auto instance = Instance::Builder()
        .add_layer(Instance::Layer::StandardValidation)
        .add_layer(Instance::Layer::RenderDocCapture)
        .add_extension(Instance::Extension::Surface)
        .add_extension(Instance::Extension::Win32Surface)
        .build()
        .expect("Unable to create instance");

    auto physical_device = instance.pick_physical_device().expect("Unable to pick physical device");
    auto [general_queue_family, transfer_queue_family] = physical_device.pick_queue_families();

    std::cout << general_queue_family.get_index() << general_queue_family.get_count() << std::endl;
    std::cout << transfer_queue_family.get_index() << transfer_queue_family.get_count() << std::endl;

    instance.destroy();
    

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