# RAYEX
RAYEX is a work-in-progress, cross-platform, cross-vendor, Vulkan-based, real-time path tracer. 

It runs on both Linux (x64) and Windows (x64) using the Vulkan API. The purpose of this software is to provide a cross-platform, cross-vendor path tracer that is easy to integrate into existing projects. Vendor independence is achieved by using KHRONOS' new Vulkan ray tracing extension (first published on March 17, 2020). It uses [SDL2](https://www.libsdl.org/index.php) for windowing and [glm](https://glm.g-truc.net/0.9.9/index.html) as its math library. RAYEX features native [Dear ImGui](https://github.com/ocornut/imgui) support, enabling the user to create user interfaces without having to write any API-related code.

## Requirements
Unfortunately, KHRONOS' Vulkan ray tracing extension is not yet part of the NVIDIA main driver. Download the NVIDIA Vulkan beta drivers [here](https://developer.nvidia.com/vulkan-driver). 
AMD drivers can be downloaded [here](https://www.amd.com/en/support/kb/release-notes/rn-rad-win-20-11-2-vrt-beta) instead.

## Deploy
To integrate RAYEX in your existing C++ / CMake project, follow the steps below.

1. Download the latest [release](https://github.com/chillpert/rayex) or build the library yourself.
2. Place the **shared library objects** and the provided **include folder** somewhere inside your project.
3. Include the CMakeLists.txt inside the **include folder** to your CMakeLists.txt.
4. Link your target against the **shared library objects** and against SDL2.

Using CMake is not obligatory but recommended as RAYEX comes with its own CMakeLists.txt that will properly include all project headers. A release also comes with a fully functional example including a complete CMakeLists.txt that can be used for reference.

## Build
### Linux
To build the entire library yourself follow the steps below. 
If you want to build the provided example, make sure to enable the RX_BUILD_EXAMPLES build option in CMake.
```sh
$ git clone https://github.com/chillpert/rayex
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
Generate a Visual Studio 2019 solution using CMake and build it or simply open the project folder using Visual Studio 2019, confirm the prompt to generate the CMake cache and build it.

### Usage
The following cope snippet renders a cube. For a proper example take a look at the **documentation** or at **example/example.cpp**.
```
#include "Rayex.hpp"

int main( )
{
    // Initialize the renderer.
    rx::Rayex renderer;
    renderer.init( );
    
    // Load some geometry and submit it to RAYEX.
    auto cube   = rx::loadObj( "models/cube.obj" );
    renderer.scene( ).submitGeometry( cube );

    // Create an instance of the geometry from above and submit it for rendering.
    auto cubeInstance = rx::instance( cube );
    renderer.scene( ).submitGeometryInstance( cubeInstance );

    // Add a light source.
    renderer.scene( ).submitDirectionalLight( rx::directionalLightInstance( glm::vec3( -4.0F, 10.0F, 5.0F ) ) );

    // The main loop.
    while ( renderer.isRunning( ) )
    {
        renderer.run( );
    }
}
```

### Documentation
An extensive documentation of all classes and functions can be found [here](https://chillpert.github.io/rayex/html/index.html).
