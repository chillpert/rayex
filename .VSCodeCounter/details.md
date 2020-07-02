# Details

Date : 2020-06-30 11:50:20

Directory c:\Repos\renderer\src

Total : 92 files,  5661 codes, 277 comments, 1449 blanks, all 7387 lines

[summary](results.md)

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [src/Renderer.cpp](/src/Renderer.cpp) | C++ | 98 | 0 | 17 | 115 |
| [src/Renderer.hpp](/src/Renderer.hpp) | C++ | 33 | 4 | 11 | 48 |
| [src/api/Api.cpp](/src/api/Api.cpp) | C++ | 660 | 54 | 161 | 875 |
| [src/api/Api.hpp](/src/api/Api.hpp) | C++ | 97 | 5 | 17 | 119 |
| [src/api/Components.cpp](/src/api/Components.cpp) | C++ | 13 | 0 | 2 | 15 |
| [src/api/Components.hpp](/src/api/Components.hpp) | C++ | 16 | 2 | 4 | 22 |
| [src/api/Instance.cpp](/src/api/Instance.cpp) | C++ | 90 | 3 | 21 | 114 |
| [src/api/Instance.hpp](/src/api/Instance.hpp) | C++ | 19 | 8 | 7 | 34 |
| [src/api/Model.cpp](/src/api/Model.cpp) | C++ | 58 | 0 | 14 | 72 |
| [src/api/Model.hpp](/src/api/Model.hpp) | C++ | 77 | 2 | 23 | 102 |
| [src/api/Pipeline.cpp](/src/api/Pipeline.cpp) | C++ | 188 | 2 | 36 | 226 |
| [src/api/Pipeline.hpp](/src/api/Pipeline.hpp) | C++ | 65 | 0 | 9 | 74 |
| [src/api/RenderPass.cpp](/src/api/RenderPass.cpp) | C++ | 34 | 0 | 6 | 40 |
| [src/api/RenderPass.hpp](/src/api/RenderPass.hpp) | C++ | 19 | 0 | 8 | 27 |
| [src/api/Shader.cpp](/src/api/Shader.cpp) | C++ | 67 | 5 | 19 | 91 |
| [src/api/Shader.hpp](/src/api/Shader.hpp) | C++ | 31 | 0 | 9 | 40 |
| [src/api/Surface.cpp](/src/api/Surface.cpp) | C++ | 61 | 5 | 13 | 79 |
| [src/api/Surface.hpp](/src/api/Surface.hpp) | C++ | 27 | 6 | 9 | 42 |
| [src/api/Swapchain.cpp](/src/api/Swapchain.cpp) | C++ | 92 | 7 | 25 | 124 |
| [src/api/Swapchain.hpp](/src/api/Swapchain.hpp) | C++ | 31 | 5 | 10 | 46 |
| [src/api/buffers/Buffer.cpp](/src/api/buffers/Buffer.cpp) | C++ | 101 | 10 | 22 | 133 |
| [src/api/buffers/Buffer.hpp](/src/api/buffers/Buffer.hpp) | C++ | 55 | 2 | 16 | 73 |
| [src/api/buffers/CommandBuffer.cpp](/src/api/buffers/CommandBuffer.cpp) | C++ | 62 | 2 | 15 | 79 |
| [src/api/buffers/CommandBuffer.hpp](/src/api/buffers/CommandBuffer.hpp) | C++ | 41 | 2 | 14 | 57 |
| [src/api/buffers/Framebuffer.cpp](/src/api/buffers/Framebuffer.cpp) | C++ | 57 | 0 | 14 | 71 |
| [src/api/buffers/Framebuffer.hpp](/src/api/buffers/Framebuffer.hpp) | C++ | 32 | 0 | 10 | 42 |
| [src/api/buffers/IndexBuffer.hpp](/src/api/buffers/IndexBuffer.hpp) | C++ | 83 | 3 | 22 | 108 |
| [src/api/buffers/QueryPool.cpp](/src/api/buffers/QueryPool.cpp) | C++ | 38 | 0 | 8 | 46 |
| [src/api/buffers/QueryPool.hpp](/src/api/buffers/QueryPool.hpp) | C++ | 29 | 0 | 8 | 37 |
| [src/api/buffers/UniformBuffer.cpp](/src/api/buffers/UniformBuffer.cpp) | C++ | 48 | 1 | 13 | 62 |
| [src/api/buffers/UniformBuffer.hpp](/src/api/buffers/UniformBuffer.hpp) | C++ | 36 | 0 | 11 | 47 |
| [src/api/buffers/VertexBuffer.cpp](/src/api/buffers/VertexBuffer.cpp) | C++ | 40 | 3 | 10 | 53 |
| [src/api/buffers/VertexBuffer.hpp](/src/api/buffers/VertexBuffer.hpp) | C++ | 33 | 0 | 8 | 41 |
| [src/api/descriptors/DescriptorSet.cpp](/src/api/descriptors/DescriptorSet.cpp) | C++ | 95 | 3 | 21 | 119 |
| [src/api/descriptors/DescriptorSet.hpp](/src/api/descriptors/DescriptorSet.hpp) | C++ | 51 | 0 | 14 | 65 |
| [src/api/descriptors/DescriptorSetLayout.cpp](/src/api/descriptors/DescriptorSetLayout.cpp) | C++ | 47 | 1 | 12 | 60 |
| [src/api/descriptors/DescriptorSetLayout.hpp](/src/api/descriptors/DescriptorSetLayout.hpp) | C++ | 29 | 1 | 11 | 41 |
| [src/api/devices/Device.cpp](/src/api/devices/Device.cpp) | C++ | 80 | 4 | 23 | 107 |
| [src/api/devices/Device.hpp](/src/api/devices/Device.hpp) | C++ | 18 | 5 | 6 | 29 |
| [src/api/devices/Memory.cpp](/src/api/devices/Memory.cpp) | C++ | 31 | 0 | 8 | 39 |
| [src/api/devices/Memory.hpp](/src/api/devices/Memory.hpp) | C++ | 17 | 0 | 7 | 24 |
| [src/api/devices/PhysicalDevice.cpp](/src/api/devices/PhysicalDevice.cpp) | C++ | 101 | 4 | 34 | 139 |
| [src/api/devices/PhysicalDevice.hpp](/src/api/devices/PhysicalDevice.hpp) | C++ | 25 | 31 | 12 | 68 |
| [src/api/gui/GuiBase.cpp](/src/api/gui/GuiBase.cpp) | C++ | 196 | 4 | 41 | 241 |
| [src/api/gui/GuiBase.hpp](/src/api/gui/GuiBase.hpp) | C++ | 70 | 0 | 15 | 85 |
| [src/api/image/Image.cpp](/src/api/image/Image.cpp) | C++ | 86 | 1 | 17 | 104 |
| [src/api/image/Image.hpp](/src/api/image/Image.hpp) | C++ | 25 | 0 | 9 | 34 |
| [src/api/image/ImageView.cpp](/src/api/image/ImageView.cpp) | C++ | 43 | 0 | 10 | 53 |
| [src/api/image/ImageView.hpp](/src/api/image/ImageView.hpp) | C++ | 32 | 0 | 9 | 41 |
| [src/api/image/Sampler.cpp](/src/api/image/Sampler.cpp) | C++ | 24 | 0 | 2 | 26 |
| [src/api/image/Sampler.hpp](/src/api/image/Sampler.hpp) | C++ | 15 | 0 | 4 | 19 |
| [src/api/image/Texture.cpp](/src/api/image/Texture.cpp) | C++ | 55 | 1 | 16 | 72 |
| [src/api/image/Texture.hpp](/src/api/image/Texture.hpp) | C++ | 45 | 0 | 9 | 54 |
| [src/api/misc/DebugMessenger.cpp](/src/api/misc/DebugMessenger.cpp) | C++ | 99 | 2 | 13 | 114 |
| [src/api/misc/DebugMessenger.hpp](/src/api/misc/DebugMessenger.hpp) | C++ | 27 | 7 | 9 | 43 |
| [src/api/misc/DebugUtility.hpp](/src/api/misc/DebugUtility.hpp) | C++ | 39 | 2 | 10 | 51 |
| [src/api/misc/Dummy.hpp](/src/api/misc/Dummy.hpp) | C++ | 42 | 3 | 14 | 59 |
| [src/api/misc/Initializers.cpp](/src/api/misc/Initializers.cpp) | C++ | 45 | 0 | 8 | 53 |
| [src/api/misc/Initializers.hpp](/src/api/misc/Initializers.hpp) | C++ | 14 | 0 | 6 | 20 |
| [src/api/misc/Vertex.hpp](/src/api/misc/Vertex.hpp) | C++ | 63 | 0 | 16 | 79 |
| [src/api/pools/CommandPool.cpp](/src/api/pools/CommandPool.cpp) | C++ | 27 | 0 | 7 | 34 |
| [src/api/pools/CommandPool.hpp](/src/api/pools/CommandPool.hpp) | C++ | 20 | 0 | 8 | 28 |
| [src/api/pools/DescriptorPool.cpp](/src/api/pools/DescriptorPool.cpp) | C++ | 40 | 0 | 9 | 49 |
| [src/api/pools/DescriptorPool.hpp](/src/api/pools/DescriptorPool.hpp) | C++ | 29 | 0 | 8 | 37 |
| [src/api/queue/Queue.hpp](/src/api/queue/Queue.hpp) | C++ | 33 | 0 | 8 | 41 |
| [src/api/queue/QueueManager.cpp](/src/api/queue/QueueManager.cpp) | C++ | 282 | 3 | 79 | 364 |
| [src/api/queue/QueueManager.hpp](/src/api/queue/QueueManager.hpp) | C++ | 57 | 4 | 23 | 84 |
| [src/api/raytracing/BottomLevelAS.cpp](/src/api/raytracing/BottomLevelAS.cpp) | C++ | 125 | 14 | 28 | 167 |
| [src/api/raytracing/BottomLevelAS.hpp](/src/api/raytracing/BottomLevelAS.hpp) | C++ | 28 | 3 | 9 | 40 |
| [src/api/raytracing/RaytraceBuilder.cpp](/src/api/raytracing/RaytraceBuilder.cpp) | C++ | 169 | 12 | 43 | 224 |
| [src/api/raytracing/RaytraceBuilder.hpp](/src/api/raytracing/RaytraceBuilder.hpp) | C++ | 45 | 0 | 13 | 58 |
| [src/api/raytracing/TopLevelAS.cpp](/src/api/raytracing/TopLevelAS.cpp) | C++ | 70 | 4 | 17 | 91 |
| [src/api/raytracing/TopLevelAS.hpp](/src/api/raytracing/TopLevelAS.hpp) | C++ | 26 | 1 | 8 | 35 |
| [src/api/sync/Fence.cpp](/src/api/sync/Fence.cpp) | C++ | 35 | 0 | 9 | 44 |
| [src/api/sync/Fence.hpp](/src/api/sync/Fence.hpp) | C++ | 26 | 0 | 9 | 35 |
| [src/api/sync/Semaphore.cpp](/src/api/sync/Semaphore.cpp) | C++ | 34 | 0 | 8 | 42 |
| [src/api/sync/Semaphore.hpp](/src/api/sync/Semaphore.hpp) | C++ | 26 | 0 | 8 | 34 |
| [src/base/Base.hpp](/src/base/Base.hpp) | C++ | 7 | 14 | 3 | 24 |
| [src/base/CameraBase.hpp](/src/base/CameraBase.hpp) | C++ | 34 | 0 | 10 | 44 |
| [src/base/NodeBase.hpp](/src/base/NodeBase.hpp) | C++ | 41 | 1 | 12 | 54 |
| [src/example/main.cpp](/src/example/main.cpp) | C++ | 276 | 15 | 69 | 360 |
| [src/pch/stdafx.cpp](/src/pch/stdafx.cpp) | C++ | 1 | 0 | 1 | 2 |
| [src/pch/stdafx.hpp](/src/pch/stdafx.hpp) | C++ | 29 | 0 | 5 | 34 |
| [src/utils/Core.hpp](/src/utils/Core.hpp) | C++ | 16 | 0 | 2 | 18 |
| [src/utils/Utility.cpp](/src/utils/Utility.cpp) | C++ | 8 | 0 | 1 | 9 |
| [src/utils/Utility.hpp](/src/utils/Utility.hpp) | C++ | 47 | 1 | 16 | 64 |
| [src/window/Time.cpp](/src/window/Time.cpp) | C++ | 46 | 2 | 15 | 63 |
| [src/window/Time.hpp](/src/window/Time.hpp) | C++ | 23 | 0 | 8 | 31 |
| [src/window/WindowBase.cpp](/src/window/WindowBase.cpp) | C++ | 113 | 2 | 35 | 150 |
| [src/window/WindowBase.hpp](/src/window/WindowBase.hpp) | C++ | 35 | 1 | 15 | 51 |
| [src/window/WindowProperties.cpp](/src/window/WindowProperties.cpp) | C++ | 26 | 0 | 4 | 30 |
| [src/window/WindowProperties.hpp](/src/window/WindowProperties.hpp) | C++ | 42 | 0 | 11 | 53 |

[summary](results.md)