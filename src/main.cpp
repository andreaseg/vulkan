#include <iostream>
#include "vulkan/instance.hpp"
#ifdef WINDOWS
#include <windows.h>
#endif

int main() {
    auto layers = Instance::enumerate_layers();
    for (auto &layer: layers) {
        std::cout << layer << std::endl;
    }
    auto extensions = Instance::enumerate_extensions();
    for (auto &extension: extensions) {
        std::cout << extension << std::endl;
    }

    
    Instance instance = Instance::Builder()
        .add_extension(Instance::Extension::DegbgReport)
        .add_layer(Instance::Layer::StandardValidation)
        .add_layer(Instance::Layer::RenderDocCapture)
        .build();

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