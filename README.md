# Rayex

Rayex is a framework for cross-platform, cross-vendor, real-time path tracing in Vulkan. It runs on both Linux and Windows and is implemented using the new Khronos Vulkan ray tracing extension. Additionally, rayex uses [SDL2](https://www.libsdl.org/index.php) for windowing and [glm](https://glm.g-truc.net/0.9.9/index.html) as its math library. The framework also features native [Dear ImGui](https://github.com/ocornut/imgui) support enabling the user to create graphical user interfaces without writing any API-related code.

Rayex is still in development, so documentation and examples might not always be up to date or complete.

## Table of Contents
1. [Requirements](#requirements)
2. [Deploy](#deploy)
3. [Build](#build)
4. [Examples](#examples)
5. [Documentation](#documentation)

## Requirements
**General:**
- Download compatible drivers for [NVIDIA](https://www.nvidia.com/Download/index.aspx?lang=en-us) or [AMD](https://www.amd.com/en/support/kb/release-notes/rn-rad-win-20-11-2-vrt-beta) if not already installed
- `C++20` has to be enabled for the target (see CMakeLists.txt)
- `VulkanSDK` (1.2.162.1 or higher)

**Linux:**
- Install `SDL2` with package manager

**Windows:**
- [Microsoft Visual C++ Redistributable](https://visualstudio.microsoft.com/downloads/?q=Microsoft+Visual+C%2B%2B+Redistributable+for+Visual+Studio+2019) for Visual Studio 2019

## Deploy
To integrate rayex in an existing C++ / CMake project, please follow the steps below. Alternatively, [build](#build) rayex yourself.

1. [Download](https://github.com/chillpert/rayex) pre-built binaries for your system and place them inside your project (assuming `myProject/external/rayex`).
2. Inside your project's CMakeLists.txt:

   1. Include **"include/CMakeLists.txt"** to get all necessary rayex headers,  e.g.
   ```
   include(${PROJECT_SOURCE_DIR}/external/rayex/include/CMakeLists.txt)
   ```
   2. Inform CMake about the location of all binaries required, e.g.
   ```
   target_link_directories(example PRIVATE ${PROJECT_SOURCE_DIR}/external/rayex/libs)
   ``` 
   3. Link your target against the rayex and SDL, e.g.
   ```
   target_link_libraries(example librayex.so SDL2main SDL2)
   ```

## Build
```sh
$ git clone https://github.com/chillpert/rayex --recursive
$ cd rayex 
$ mkdir build && cd build
$ cmake ..
$ make
```

**Note:** On Windows open the Visual Studio solution file (.sln) in the build directory and compile instead of `$ make`.

## Examples
The following cope snippet shows how to render a single mesh. For a proper example, take a look at the **documentation** and at the official [examples](https://github.com/chillpert/rayex-examples) for rayex.

```
#include "Rayex.hpp"

int main( )
{
    // Initialize the renderer.
    rx::Rayex renderer;
    renderer.init( );
    
    // Load some geometry and submit it to rayex.
    auto cube   = rx::loadObj( "cube.obj" );
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

## Documentation
A documentation of all classes and functions can be found [here](https://chillpert.github.io/rayex/html/index.html).

## Current State
### Issues
- [ ] Fix hardware evaluation (in VkCore)
- [ ] Crashes again when GUI disabled
- [ ] Do a complete C++ guidelines check
- [ ] Test Windows build
- [ ] Remove unncessary includes (especially in PCH)
- [ ] Provide new binaries for all platforms
- [ ] Enable all warnings for rayex and disable all warnings for third party
- [ ] Test rayex deploy with only headers and libs again
- [ ] Remove any paths set at compile time
- [ ] Remove big examples and put it in separate repo (combined with )
- [ ] Consider decreasing project size
- [ ] Fix camera reset when moving mouse after loading a scene

### Planned Features
- [ ] Use rasterization for primary rays
- [ ] Improve documentation (especially requirements for model loading)
- [ ] Improved acceleration structure handling with optional performance tweaks
- [ ] Implement stochastic level of detail
- [ ] Multiple importance sampling
- [ ] HDR skymaps and skyboxes as light sources
- [ ] Denoising
- [ ] Add area light sampling for next event estimation

