# RAYEX
RAYEX is a work-in-progress, cross-platform, cross-vendor, Vulkan-based, real-time path tracer. 

It runs on both Linux (x64) and Windows (x64) using the Vulkan API. This software aims to provide a cross-platform, cross-vendor path tracer that is easy to integrate into existing projects. Vendor independence is achieved by using the new KHRONOS Vulkan ray tracing extension (first published on March 17, 2020, as an experimental version). It uses [SDL2](https://www.libsdl.org/index.php) for windowing and [glm](https://glm.g-truc.net/0.9.9/index.html) as its math library. RAYEX features native [Dear ImGui](https://github.com/ocornut/imgui) support, enabling the user to create user interfaces without writing any API-related code.

## Requirements
Install the latest NVIDIA driver. If you run into issues try the NVIDIA Vulkan beta drivers [here](https://developer.nvidia.com/vulkan-driver), or check your GPU compatibility.
AMD drivers can be downloaded [here](https://www.amd.com/en/support/kb/release-notes/rn-rad-win-20-11-2-vrt-beta) instead.

## Deploy [OUT OF DATE]
To integrate RAYEX in an existing C++ / CMake project, please follow the steps below.

1. Download the latest [release](https://github.com/chillpert/rayex) or build the library yourself.
2. Place the **Rayex library** and the provided **include folder** somewhere inside your project.
3. Link your target against the **Rayex library**.

Using CMake is not obligatory but recommended as RAYEX comes with its own CMakeLists.txt that will properly include all project headers. A release also comes with a fully functional example, including a complete CMakeLists.txt that can be used for reference.

## Build [OUT OF DATE]
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
Generate a Visual Studio 2019 solution using CMake and build it or open the project folder using Visual Studio 2019, confirm the prompt to generate the CMake cache, and build it.

### Usage [OUT OF DATE]
The following cope snippet renders a cube. For a proper example, take a look at the **documentation** or **example/example.cpp**.
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

    // The main loop.
    while ( renderer.isRunning( ) )
    {
        renderer.run( );
    }
}
```

### TODO

- [ ] Fix hardware evaluation (in VkCore)
- [ ] Crashes again when GUI disabled
- [ ] Do a complete C++ guidelines check
- [ ] Test Windows build
- [ ] Remove unncessary includes (especially in PCH)
- [ ] Provide new binaries for all platforms
- [ ] Enable all warnings for Rayex and disable all warnings for third party
- [ ] Test Rayex deploy with only headers and libs again
- [ ] Remove any paths set at compile time
- [ ] Remove big examples and put it in separate repo (combined with )
- [ ] Try to decrease project size (either compile SDL and KTX as well or only provide necessary headers since precompiled binaries have to be used under Windows)

### Planned Features

- [ ] Use rasterization for primary rays
- [ ] Improve documentation (especially requirements for model loading)
- [ ] Improved acceleration structure handling with optional performance tweaks
- [ ] Implement stochastic level of detail
- [ ] Multiple importance sampling
- [ ] HDR skymaps and skyboxes as light sources
- [ ] Denoising
- [ ] Add area light sampling for next event estimation

### Documentation
A documentation of all classes and functions can be found [here](https://chillpert.github.io/rayex/html/index.html).
