# RAYEXEC
RAYEXEC is a cross-platform Vulkan-based path tracing rendering library. 

It runs on both Linux (x64) and Windows (x64) using the Vulkan API. The purpose of this software is to provide a cross-platform renderer with path tracing capabilities that is easy to integrate into existing projects. It uses SDL2 for windowing and GLM as its maths library. RAYEXEC features native [Dear ImGui](https://github.com/ocornut/imgui) support.

## Requirements
NVIDIA Vulkan Beta Drivers [[Download]](https://developer.nvidia.com/vulkan-driver) 

## Deploy
To integrate RAYEXEC to your existing C++ / CMake project follow the steps below.

1. Download the latest [release]().
2. Place the **shared library objects** and the provided **include folder** somewhere inside your project.
3. Include the CMakeLists.txt inside the **include folder** to your CMakeLists.txt
4. Link your target against the **shared library objects** and against SDL2 

Using CMake is not obligatory but it assures platform independence.

## Build
### Linux
To build the entire library yourself follow the steps below. 
If you want to build the provided example, make sure to enable the RX_BUILD_EXAMPLES build option in CMake.
```sh
$ git clone https://github.com/chillpert/rayexec
$ cd renderer 
$ mkdir build && cd build
$ cmake ..
$ make all
```
Execute the example. Depending on the build type you want to choose, replace [buildtype].
```sh
$ ./bin/[buildtype]/example
```

### Windows
Generate a Visual Studio solution using CMake. 

### Usage
The following shows the most minimal example possible. Nothing will be rendered. For a proper example take a look at the **documentation** or at **example/example.cpp**.
```
#include "RayExec.hpp"

int main( )
{
    RayExec renderer;
    renderer.init( );
    
    while ( renderer.isRunning( ) )
    {
        renderer.run( );
    }
}
```

### Documentation
An extensive documentation of all classes and functions can be found [here](https://christianhilpert.com/rayexec).
