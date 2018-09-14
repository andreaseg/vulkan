#include <iostream>
#include "vulkan/instance.hpp"
#ifdef WINDOWS
#include <windows.h>
#endif

int main() {

    
    auto instance = Instance::Builder()
        .build()
        .unwrap();

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