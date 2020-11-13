# group `API` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`RAYEX_NAMESPACE::Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api) | Initializes and owns all Vulkan components and displays a picture on the screen.
`class `[`RAYEX_NAMESPACE::Bindings`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings) | A utility class for managing descriptor related resources.
`class `[`RAYEX_NAMESPACE::Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) | A wrapper class for a Vulkan buffer.
`class `[`RAYEX_NAMESPACE::CommandBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer) | A wrapper class for Vulkan command buffers.
`class `[`RAYEX_NAMESPACE::IndexBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_index_buffer) | A specialised buffer for index data.
`class `[`RAYEX_NAMESPACE::StorageBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_storage_buffer) | A shader storage buffer wrapper class.
`class `[`RAYEX_NAMESPACE::UniformBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer) | A specialised buffer for uniforms.
`class `[`RAYEX_NAMESPACE::VertexBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer) | A specialised buffer for vertex data.
`class `[`RAYEX_NAMESPACE::Image`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image) | A wrapper class for a Vulkan image.
`class `[`RAYEX_NAMESPACE::Texture`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_texture) | A helper class for creating ready-to-use textures.
`class `[`RAYEX_NAMESPACE::Pipeline`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_pipeline) | A wrapper class for a Vulkan graphics pipeline.
`class `[`RAYEX_NAMESPACE::RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder) | Manages the building process of the acceleration structures.
`class `[`RAYEX_NAMESPACE::RenderPass`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_render_pass) | A wrapper class for a Vulkan render pass.
`class `[`RAYEX_NAMESPACE::Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface) | A wrapper class for a Vulkan surface.
`class `[`RAYEX_NAMESPACE::Swapchain`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain) | A wrapper class for a Vulkan swapchain.
`class `[`RAYEX_NAMESPACE::DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger) | A wrapper class for a Vulkan debug utility messenger.
`class `[`RAYEX_NAMESPACE::Gui`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui) | A class to create an ImGui-based GUI.
`struct `[`RAYEX_NAMESPACE::Descriptors`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_descriptors) | Encapsulates descriptor-related resources.
`struct `[`RAYEX_NAMESPACE::DirectionalLightSSBO`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light_s_s_b_o) | A wrapper for [RAYEX_NAMESPACE::DirectionalLight](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light) matching the buffer alignment requirements.
`struct `[`RAYEX_NAMESPACE::PointLightSSBO`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light_s_s_b_o) | A wrapper for [RAYEX_NAMESPACE::PointLight](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light) matching the buffer alignment requirements.
`struct `[`RAYEX_NAMESPACE::MeshSSBO`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o) | A wrapper for [RAYEX_NAMESPACE::MeshSSBO](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o) matching the buffer alignment requirements.
`struct `[`RAYEX_NAMESPACE::GeometryInstanceSSBO`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o) | A wrapper for [RAYEX_NAMESPACE::GeometryInstanceSSBO](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o) matching the buffer alignment requirements.
`struct `[`RAYEX_NAMESPACE::CameraUbo`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_ubo) | A uniform buffer object for camera data.
`struct `[`RAYEX_NAMESPACE::AccelerationStructure`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) | A wrapper for a Vulkan acceleration Structure.
`struct `[`RAYEX_NAMESPACE::Tlas`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_tlas) | A wrapper for a top level acceleration structure.
`struct `[`RAYEX_NAMESPACE::Blas`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas) | A wrapper for a bottom level acceleration structure.
`struct `[`RAYEX_NAMESPACE::BlasInstance`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance) | An instance of a bottom level acceleration structure.
`struct `[`RAYEX_NAMESPACE::Vertex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex) | Describes a vertex and its data.

# class `RAYEX_NAMESPACE::Api` 

Initializes and owns all Vulkan components and displays a picture on the screen.

All API components and resources are freed using scope-bound destruction. 

Because of scope-bound destruction it is required to pay close attention to the order of resource allocations done by the user.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public friend `[`Rayex`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a654c4ab4f46b43e1dac112eafb763aac) | 
`public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a6d337866b5a344ec9f2a1c3e968f1fef)`() = default` | 
`public  `[`~Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a2466f3fcfd89ab8d1d80eed1254950c0)`()` | 
`public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a2876a9e34331f4d262d31ba3862eb171)`(std::shared_ptr< `[`Window`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` > window,std::shared_ptr< `[`Camera`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` > camera)` | #### Parameters
`public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a75fc41dab952a0b3c08d95a30ff451a8)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &) = delete` | 
`public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a12e91dc6ced5d4022accb12716616d11)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1afe3d5724cdbbf6e5756ec4b7c8cba6f0)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a48e629366ef5f94b8573a288c872955c)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &&) = delete` | 

## Members

#### `public friend `[`Rayex`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a654c4ab4f46b43e1dac112eafb763aac) 

#### `public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a6d337866b5a344ec9f2a1c3e968f1fef)`() = default` 

#### `public  `[`~Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a2466f3fcfd89ab8d1d80eed1254950c0)`()` 

#### `public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a2876a9e34331f4d262d31ba3862eb171)`(std::shared_ptr< `[`Window`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` > window,std::shared_ptr< `[`Camera`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` > camera)` 

#### Parameters
* `window` A pointer to a window object that the API will use to display an image. 

* `camera` A pointer to a camera object that will be used to "capture" the scene.

#### `public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a75fc41dab952a0b3c08d95a30ff451a8)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &) = delete` 

#### `public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a12e91dc6ced5d4022accb12716616d11)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1afe3d5724cdbbf6e5756ec4b7c8cba6f0)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a48e629366ef5f94b8573a288c872955c)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &&) = delete` 

# class `RAYEX_NAMESPACE::Bindings` 

A utility class for managing descriptor related resources.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void `[`add`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a37856d36392e371e4fdcfe35d184fa42)`(uint32_t binding,vk::DescriptorType type,vk::ShaderStageFlags stage,uint32_t count,vk::DescriptorBindingFlags flags,vk::Sampler * pImmutableSamplers)` | Used to add a binding.
`public auto `[`initLayoutUnique`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1ac4a04461e4728f35c9146237a47e6e66)`(vk::DescriptorSetLayoutCreateFlags flags)` | Used to initialize a unique descriptor set layout.
`public auto `[`initPoolUnique`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a20edd74ad52f8a3604f7cd892f2239f6)`(uint32_t maxSets,vk::DescriptorPoolCreateFlags flags)` | Used to initialize a descriptor pool.
`public void `[`update`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a5c7e9adcda6c378e5a1b342d70d3acc5)`()` | Updates the descriptor set.
`public void `[`write`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a8ff18f3ca1c5644d8a6237eac7292928)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::WriteDescriptorSetAccelerationStructureKHR * pWriteDescriptorSetAccelerationStructureKHR)` | Used to create a descriptor write for an acceleration structure.
`public void `[`write`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1aa33d82290e77d12e483f609fe94c7c59)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::DescriptorImageInfo * pImageInfo)` | Used to create a descriptor write for an image.
`public void `[`write`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a86bc166283b66986b514920652dfbdd9)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::DescriptorBufferInfo * pBufferInfo)` | Used to create a descriptor write for a buffer.
`public void `[`write`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a504a47e05d5db66c5545bf6402ae6b98)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,const std::vector< vk::DescriptorBufferInfo > & uniformBufferInfos)` | Used to create a descriptor write for uniform buffers.
`public void `[`writeArray`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1aa7d770601a7dc6456b8d1d58d1f8e7f4)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,const vk::DescriptorBufferInfo * pBufferInfo)` | Used to create an array of descriptor writes for buffers.
`public void `[`writeArray`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1adea0ef24844370ca397ec05f53682a08)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,const vk::DescriptorImageInfo * pImageInfo)` | Used to create an array of descriptor writes for images.
`public void `[`setPoolSizes`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1adb3fefd44d2cff639f5ad5a386b40f8a)`(const std::vector< vk::DescriptorPoolSize > & poolSizes)` | Used to set pool sizes manually.
`public void `[`reset`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a6ce4c6073f217615f9212444578ca217)`()` | Resets all members.

## Members

#### `public void `[`add`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a37856d36392e371e4fdcfe35d184fa42)`(uint32_t binding,vk::DescriptorType type,vk::ShaderStageFlags stage,uint32_t count,vk::DescriptorBindingFlags flags,vk::Sampler * pImmutableSamplers)` 

Used to add a binding.

#### Parameters
* `binding` The binding's index. 

* `type` The binding's descriptor type. 

* `stage` The binding's shader type. 

* `count` The amount of descriptors for this binding. 

* `flags` The binding's descriptor binding flags.

#### `public auto `[`initLayoutUnique`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1ac4a04461e4728f35c9146237a47e6e66)`(vk::DescriptorSetLayoutCreateFlags flags)` 

Used to initialize a unique descriptor set layout.

#### Returns
Returns a descriptor set layout with a unqiue handle.

#### `public auto `[`initPoolUnique`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a20edd74ad52f8a3604f7cd892f2239f6)`(uint32_t maxSets,vk::DescriptorPoolCreateFlags flags)` 

Used to initialize a descriptor pool.

#### Parameters
* `maxSets` The maximum amount of descriptor sets that can be allocated from the pool. 

* `flags` The pool's create flags. 

#### Returns
Returns a descriptor pool with a unique handle.

#### `public void `[`update`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a5c7e9adcda6c378e5a1b342d70d3acc5)`()` 

Updates the descriptor set.

There are no descriptor set handles required for this function.

#### `public void `[`write`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a8ff18f3ca1c5644d8a6237eac7292928)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::WriteDescriptorSetAccelerationStructureKHR * pWriteDescriptorSetAccelerationStructureKHR)` 

Used to create a descriptor write for an acceleration structure.

#### Parameters
* `sets` The descriptor set handles. 

* `binding` The binding's index. 

* `pWriteDescriptorSetAccelerationStructureKHR` A pointer to a acceleration structure descriptor write.

#### `public void `[`write`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1aa33d82290e77d12e483f609fe94c7c59)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::DescriptorImageInfo * pImageInfo)` 

Used to create a descriptor write for an image.

#### Parameters
* `sets` The descriptor set handles. 

* `binding` The binding's index. 

* `pImageInfo` A pointer to an image descriptor write.

#### `public void `[`write`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a86bc166283b66986b514920652dfbdd9)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::DescriptorBufferInfo * pBufferInfo)` 

Used to create a descriptor write for a buffer.

#### Parameters
* `sets` The descriptor set handles. 

* `binding` The binding's index. 

* `pBufferInfo` A pointer to a buffer descriptor write.

#### `public void `[`write`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a504a47e05d5db66c5545bf6402ae6b98)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,const std::vector< vk::DescriptorBufferInfo > & uniformBufferInfos)` 

Used to create a descriptor write for uniform buffers.

#### Parameters
* `sets` The descriptor set handles. 

* `binding` The binding's index. 

* `uniformBufferInfos` The uniform buffer's descriptor buffer infos.

#### `public void `[`writeArray`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1aa7d770601a7dc6456b8d1d58d1f8e7f4)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,const vk::DescriptorBufferInfo * pBufferInfo)` 

Used to create an array of descriptor writes for buffers.

#### Parameters
* `sets` The descriptor set handles. 

* `binding` The binding's index. 

* `pBufferInfo` The pointer to the first element of an array of descriptor buffer infos.

#### `public void `[`writeArray`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1adea0ef24844370ca397ec05f53682a08)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,const vk::DescriptorImageInfo * pImageInfo)` 

Used to create an array of descriptor writes for images.

#### Parameters
* `sets` The descriptor set handles. 

* `binding` The binding's index. 

* `pImageInfo` The pointer to the first element of an array of descriptor image infos.

#### `public void `[`setPoolSizes`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1adb3fefd44d2cff639f5ad5a386b40f8a)`(const std::vector< vk::DescriptorPoolSize > & poolSizes)` 

Used to set pool sizes manually.

#### Parameters
* `poolSizes` The pool sizes to set. 

Must be set before initializing the descriptor pool.

#### `public void `[`reset`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings_1a6ce4c6073f217615f9212444578ca217)`()` 

Resets all members.

# class `RAYEX_NAMESPACE::Buffer` 

A wrapper class for a Vulkan buffer.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a453a0a1cab9ce09237de96956ad5084e)`() = default` | 
`public  `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a85fc3320284c9fc86390bdd03965382f)`(vk::DeviceSize size,vk::BufferUsageFlags usage,const std::vector< uint32_t > & queueFamilyIndices,vk::MemoryPropertyFlags memoryPropertyFlags,void * pNextMemory,bool initialize)` | #### Parameters
`public  `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a133961b9bd24490925aeed994ae1a4d4)`(const `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer)` & buffer)` | #### Parameters
`public  `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a2b633d1f23ddf0ae8ee27987e9373c1a)`(const `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1adccdf72dc2b5a56496881e97c683d5ca)`(const `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer)` &)` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a53ea2ff685692af62529589df9fa18a8)`(const `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer)` &&) = delete` | 
`public virtual  `[`~Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a85974a17cea14ef9048fb06a36d3be62)`() = default` | 
`public void `[`copyToBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1aded2e7ff036b1e5a5af09e4d1ffb4bb9)`(const `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer)` & buffer) const` | Copies the content of this buffer to another [RAYEX_NAMESPACE::Buffer](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer).
`public void `[`copyToBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a4b5a87dce23eab7c91448652ed8072be)`(vk::Buffer buffer,vk::Fence fence) const` | Copies the content of this buffer to another vk::Buffer.
`public void `[`copyToImage`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1acdc10ad7e5e4a12ea82c3d3e1d4493b2)`(vk::Image image,vk::Extent3D extent) const` | Copies the content of this buffer to an image.
`public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a6aa15487587ed4a275726dea52d9bd1f)`() const` | #### Returns
`public inline auto `[`getMemory`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a3e59b7d59f338fde8570cd541162cabd)`() const` | #### Returns
`public inline auto `[`getSize`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1aa51128ff5d6822bb2df9c2dcd25f99b6)`() const` | #### Returns
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1af4c1f7cd28ec3d75feccf91f704e67d3)`(vk::DeviceSize size,vk::BufferUsageFlags usage,const std::vector< uint32_t > & queueFamilyIndices,vk::MemoryPropertyFlags memoryPropertyFlags,void * pNextMemory)` | Creates the buffer and allocates memory for it.
`public template<>`  <br/>`inline void `[`fill`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a8b2ffc5a8f5476f1108b24d8986c6726)`(const std::vector< T > & data,vk::DeviceSize offset,std::optional< vk::DeviceSize > size)` | Used to fill the buffer with any kind of data.
`public template<>`  <br/>`inline void `[`fill`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a5a8e93fc789efec85376cca937660fcb)`(const T * source,vk::DeviceSize offset,std::optional< vk::DeviceSize > size)` | 
`protected vk::UniqueBuffer `[`_buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a3e7582e91c4af47f8acebae60524263b) | The buffer object with a unique handle.
`protected vk::UniqueDeviceMemory `[`_memory`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1ac6c20a052b87d02195db7cee2dd291cd) | The buffer's memory with a unique handle.
`protected vk::DeviceSize `[`_size`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a316c2534dca0f0b865308726753712f7) | The buffer's size.
`protected void * `[`_ptrToData`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a3e7f1befed2b79696954e9b3aa8ce2f1) | 
`protected bool `[`_mapped`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a6b7afa466fa6c68fc161086f5f576c00) | 

## Members

#### `public  `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a453a0a1cab9ce09237de96956ad5084e)`() = default` 

#### `public  `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a85fc3320284c9fc86390bdd03965382f)`(vk::DeviceSize size,vk::BufferUsageFlags usage,const std::vector< uint32_t > & queueFamilyIndices,vk::MemoryPropertyFlags memoryPropertyFlags,void * pNextMemory,bool initialize)` 

#### Parameters
* `size` The size of the buffer. 

* `usage` The buffer's usage flags. 

* `queueFamilyIndices` Specifies which queue family will access the buffer. 

* `memoryPropertyFlags` Flags for memory allocation. 

* `pNextMemory` Attachment to the memory's pNext chain. 

* `initialize` If true, the buffer object will be initialized right away without an additional call to [init()](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1af4c1f7cd28ec3d75feccf91f704e67d3).

#### `public  `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a133961b9bd24490925aeed994ae1a4d4)`(const `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer)` & buffer)` 

#### Parameters
* `buffer` The target for the copy operation.

#### `public  `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a2b633d1f23ddf0ae8ee27987e9373c1a)`(const `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1adccdf72dc2b5a56496881e97c683d5ca)`(const `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer)` &)` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a53ea2ff685692af62529589df9fa18a8)`(const `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer)` &&) = delete` 

#### `public virtual  `[`~Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a85974a17cea14ef9048fb06a36d3be62)`() = default` 

#### `public void `[`copyToBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1aded2e7ff036b1e5a5af09e4d1ffb4bb9)`(const `[`Buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer)` & buffer) const` 

Copies the content of this buffer to another [RAYEX_NAMESPACE::Buffer](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer).

#### Parameters
* `buffer` The target for the copy operation.

#### `public void `[`copyToBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a4b5a87dce23eab7c91448652ed8072be)`(vk::Buffer buffer,vk::Fence fence) const` 

Copies the content of this buffer to another vk::Buffer.

#### Parameters
* `buffer` The target for the copy operation.

#### `public void `[`copyToImage`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1acdc10ad7e5e4a12ea82c3d3e1d4493b2)`(vk::Image image,vk::Extent3D extent) const` 

Copies the content of this buffer to an image.

#### Parameters
* `image` The target for the copy operation. 

* `extent` The target's extent.

#### `public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a6aa15487587ed4a275726dea52d9bd1f)`() const` 

#### Returns
Returns the buffer without the unique handle.

#### `public inline auto `[`getMemory`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a3e59b7d59f338fde8570cd541162cabd)`() const` 

#### Returns
Returns the buffer's memory without the unique handle.

#### `public inline auto `[`getSize`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1aa51128ff5d6822bb2df9c2dcd25f99b6)`() const` 

#### Returns
Returns the size of the buffer.

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1af4c1f7cd28ec3d75feccf91f704e67d3)`(vk::DeviceSize size,vk::BufferUsageFlags usage,const std::vector< uint32_t > & queueFamilyIndices,vk::MemoryPropertyFlags memoryPropertyFlags,void * pNextMemory)` 

Creates the buffer and allocates memory for it.

#### Parameters
* `size` The size of the buffer. 

* `usage` The buffer's usage flags. 

* `queueFamilyIndices` Specifies which queue family will access the buffer. 

* `memoryPropertyFlags` Flags for memory allocation. 

* `pNextMemory` Attachment to the memory's pNext chain.

#### `public template<>`  <br/>`inline void `[`fill`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a8b2ffc5a8f5476f1108b24d8986c6726)`(const std::vector< T > & data,vk::DeviceSize offset,std::optional< vk::DeviceSize > size)` 

Used to fill the buffer with any kind of data.

#### Parameters
* `source` The data to fill the buffer with. 

* `offset` The data's offset within the buffer.

#### `public template<>`  <br/>`inline void `[`fill`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a5a8e93fc789efec85376cca937660fcb)`(const T * source,vk::DeviceSize offset,std::optional< vk::DeviceSize > size)` 

#### `protected vk::UniqueBuffer `[`_buffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a3e7582e91c4af47f8acebae60524263b) 

The buffer object with a unique handle.

#### `protected vk::UniqueDeviceMemory `[`_memory`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1ac6c20a052b87d02195db7cee2dd291cd) 

The buffer's memory with a unique handle.

#### `protected vk::DeviceSize `[`_size`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a316c2534dca0f0b865308726753712f7) 

The buffer's size.

#### `protected void * `[`_ptrToData`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a3e7f1befed2b79696954e9b3aa8ce2f1) 

#### `protected bool `[`_mapped`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer_1a6b7afa466fa6c68fc161086f5f576c00) 

# class `RAYEX_NAMESPACE::CommandBuffer` 

A wrapper class for Vulkan command buffers.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`CommandBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1abe361754230f783eb86464734b851ff8)`() = default` | 
`public  `[`CommandBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a3e8baaca6b363d0a78d30ee70c584a11)`(vk::CommandPool commandPool,uint32_t count,vk::CommandBufferUsageFlags usageFlags)` | Creates the command buffers and calls initializes them right away.
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a967443c6c46e162ed9a248f18a4dcd8a)`(vk::CommandPool commandPool,uint32_t count,vk::CommandBufferUsageFlags usageFlags)` | Creates the command buffers.
`public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1ab0537198c1bdf7fc123867ff04109aec)`() const` | #### Returns
`public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1adede3dee0329159e687a6b7a246ae47c)`(size_t index) const` | Returns the command buffer by some index.
`public void `[`free`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a650ea8a7a5d452734b8cefd118639936)`()` | Frees the command buffer.
`public void `[`reset`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1ad3b5f2eb9e334aeb1ce1acc170d326f3)`()` | Resets the command buffer.
`public void `[`begin`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1adeb8480e5423081dae4f34e7c756f5d0)`(size_t index)` | Used to begin the command buffer recording.
`public void `[`end`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1ad12dd1d9c545827ad5277d596f38c2a8)`(size_t index)` | Used to stop the command buffer recording.
`public void `[`submitToQueue`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1ae47a0b4e332f7b93152e09223f858f54)`(vk::Queue queue,vk::Fence fence,const std::vector< vk::Semaphore > & waitSemaphores,const std::vector< vk::Semaphore > & signalSemaphores,vk::PipelineStageFlags * waitDstStageMask)` | Submits the recorded commands to a queue.

## Members

#### `public  `[`CommandBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1abe361754230f783eb86464734b851ff8)`() = default` 

#### `public  `[`CommandBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a3e8baaca6b363d0a78d30ee70c584a11)`(vk::CommandPool commandPool,uint32_t count,vk::CommandBufferUsageFlags usageFlags)` 

Creates the command buffers and calls initializes them right away.

#### Parameters
* `commandPool` The command pool from which the command buffers will be allocated from. 

* `count` The amount of Vulkan command buffers to initialize (the same as the amount of images in the swapchain). 

* `usageFlags` Specifies what the buffer will be used for.

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a967443c6c46e162ed9a248f18a4dcd8a)`(vk::CommandPool commandPool,uint32_t count,vk::CommandBufferUsageFlags usageFlags)` 

Creates the command buffers.

#### Parameters
* `commandPool` The command pool from which the command buffers will be allocated from. 

* `count` The amount of Vulkan command buffers to initialize (the same as the amount of images in the swapchain). 

* `usageFlags` Specifies what the buffer will be used for.

#### `public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1ab0537198c1bdf7fc123867ff04109aec)`() const` 

#### Returns
Returns the vector of command buffers.

#### `public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1adede3dee0329159e687a6b7a246ae47c)`(size_t index) const` 

Returns the command buffer by some index.

#### Parameters
* `index` The index of the desired command buffer. 

#### Returns
Returns the command buffer.

#### `public void `[`free`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a650ea8a7a5d452734b8cefd118639936)`()` 

Frees the command buffer.

#### `public void `[`reset`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1ad3b5f2eb9e334aeb1ce1acc170d326f3)`()` 

Resets the command buffer.

#### `public void `[`begin`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1adeb8480e5423081dae4f34e7c756f5d0)`(size_t index)` 

Used to begin the command buffer recording.

#### Parameters
* `index` An index to a command buffer to record to.

#### `public void `[`end`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1ad12dd1d9c545827ad5277d596f38c2a8)`(size_t index)` 

Used to stop the command buffer recording.

#### Parameters
* `index` An index to a command buffer to stop recording.

#### `public void `[`submitToQueue`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1ae47a0b4e332f7b93152e09223f858f54)`(vk::Queue queue,vk::Fence fence,const std::vector< vk::Semaphore > & waitSemaphores,const std::vector< vk::Semaphore > & signalSemaphores,vk::PipelineStageFlags * waitDstStageMask)` 

Submits the recorded commands to a queue.

#### Parameters
* `queue` The queue to submit to. 

* `waitSemaphores` A vector of semaphores to wait for. 

* `signalSemaphores` A vector of semaphores to signal. 

* `waitDstStageMask` The pipeline stage where the commands will be executed.

# class `RAYEX_NAMESPACE::IndexBuffer` 

```
class RAYEX_NAMESPACE::IndexBuffer
  : public RAYEX_NAMESPACE::Buffer
```  

A specialised buffer for index data.

If not templatized then uint32_t will be used as the data's type.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline auto `[`getCount`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_index_buffer_1a0d57e869f2c4a84af697c36017229e4d)`() const` | #### Returns
`public inline void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_index_buffer_1aefa1b16f30e7f40dea305356be995aba)`(const std::vector< uint32_t > & indices)` | Creates the buffer, allocates memory for it and fills it with the provided data.

## Members

#### `public inline auto `[`getCount`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_index_buffer_1a0d57e869f2c4a84af697c36017229e4d)`() const` 

#### Returns
Returns the amount of indices in the buffer.

#### `public inline void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_index_buffer_1aefa1b16f30e7f40dea305356be995aba)`(const std::vector< uint32_t > & indices)` 

Creates the buffer, allocates memory for it and fills it with the provided data.

To optimize the procedure a temporary staging buffer will be created. 
#### Parameters
* `indices` The index data.

# class `RAYEX_NAMESPACE::StorageBuffer` 

A shader storage buffer wrapper class.

> Todo: documentation

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline auto `[`getDescriptorInfos`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_storage_buffer_1a8b2a59bc880192f2b5bd3df9a9805339)`() const` | 
`public inline void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_storage_buffer_1a2d53c9b1bf1ebdffae8934a72d050f7c)`(const std::vector< T > & data,size_t copies)` | 
`public inline void `[`upload`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_storage_buffer_1ad3db83a68972b7ea54a1dde49d272684)`(const std::vector< T > & data,std::optional< uint32_t > index)` | Uploads data to the buffer.

## Members

#### `public inline auto `[`getDescriptorInfos`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_storage_buffer_1a8b2a59bc880192f2b5bd3df9a9805339)`() const` 

#### `public inline void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_storage_buffer_1a2d53c9b1bf1ebdffae8934a72d050f7c)`(const std::vector< T > & data,size_t copies)` 

#### `public inline void `[`upload`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_storage_buffer_1ad3db83a68972b7ea54a1dde49d272684)`(const std::vector< T > & data,std::optional< uint32_t > index)` 

Uploads data to the buffer.

First, the data is being copied to the staging buffer which is visible to the host. Finally, the staging buffer is copied to the actual buffer on the device. 
#### Parameters
* `data` The data to upload.

# class `RAYEX_NAMESPACE::UniformBuffer` 

A specialised buffer for uniforms.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< vk::DescriptorBufferInfo > `[`_bufferInfos`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a5cdc856aebafa1b2d878b27d91f59ca5) | 
`public  `[`UniformBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1ae5ddd2f51f1cc616396347bda9092c47)`() = default` | 
`public template<>`  <br/>`inline  `[`UniformBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a7487ed42dd2de922006db5462e05b1f1)`(size_t swapchainImagesCount,bool initialize)` | #### Parameters
`public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a922bda1b964d1ce9ce903107e358dcf5)`() const` | #### Returns
`public auto `[`getRaw`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a616fbeb0ebf017794e591ca8e37d4b6e)`() const` | #### Returns
`public template<>`  <br/>`inline void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a98859467eee532952510437766b3da4f)`()` | Creates the uniform buffer and allocates memory for it.
`public template<>`  <br/>`inline void `[`upload`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1ad511f2a02b2a4e341b5e0d9caa3f9338)`(uint32_t imageIndex,T & ubo)` | Used to fill an image's buffer.

## Members

#### `public std::vector< vk::DescriptorBufferInfo > `[`_bufferInfos`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a5cdc856aebafa1b2d878b27d91f59ca5) 

#### `public  `[`UniformBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1ae5ddd2f51f1cc616396347bda9092c47)`() = default` 

#### `public template<>`  <br/>`inline  `[`UniformBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a7487ed42dd2de922006db5462e05b1f1)`(size_t swapchainImagesCount,bool initialize)` 

#### Parameters
* `swapchainImagesCount` The amount of images in the swapchain. 

* `initialize` If true, the uniform buffer will be initialized right away without an additional call to [init()](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a98859467eee532952510437766b3da4f).

#### `public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a922bda1b964d1ce9ce903107e358dcf5)`() const` 

#### Returns
Returns the vector of uniform buffers.

#### `public auto `[`getRaw`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a616fbeb0ebf017794e591ca8e37d4b6e)`() const` 

#### Returns
Returns the vector of uniform buffers as raw Vulkan buffer objects.

#### `public template<>`  <br/>`inline void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a98859467eee532952510437766b3da4f)`()` 

Creates the uniform buffer and allocates memory for it.

The function will create as many uniform buffers as there are images in the swapchain. Additionally, it will create the descriptor buffer infos which can be later used to write to a descriptor set. 
#### Parameters
* `swapchainImagesCount` The amount of images in the swapchain.

#### `public template<>`  <br/>`inline void `[`upload`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1ad511f2a02b2a4e341b5e0d9caa3f9338)`(uint32_t imageIndex,T & ubo)` 

Used to fill an image's buffer.

#### Parameters
* `imageIndex` The image's index in the swapchain. 

* `ubo` The actual uniform buffer object holding the data.

# class `RAYEX_NAMESPACE::VertexBuffer` 

```
class RAYEX_NAMESPACE::VertexBuffer
  : public RAYEX_NAMESPACE::Buffer
```  

A specialised buffer for vertex data.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`VertexBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1a0f50e43f64ec217b8fe9e1574341f5b5)`() = default` | 
`public  `[`VertexBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1a60c93ee617e13f0c0c75bda1b733bd9d)`(std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex)` > & vertices,bool initialize)` | #### Parameters
`public inline auto `[`getCount`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1aec17d0368843d5db4e34fe4d8b83b926)`() const` | #### Returns
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1aac3ab46655bc4f4940fc1f8c4a49d046)`(const std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex)` > & vertices)` | Creates the buffer, allocates memory for it and fills it with provided data.

## Members

#### `public  `[`VertexBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1a0f50e43f64ec217b8fe9e1574341f5b5)`() = default` 

#### `public  `[`VertexBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1a60c93ee617e13f0c0c75bda1b733bd9d)`(std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex)` > & vertices,bool initialize)` 

#### Parameters
* `vertices` The vertex data. 

* `initialize` If true, the vertex buffer will be initialized right away without an additional call to [init()](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1aac3ab46655bc4f4940fc1f8c4a49d046).

#### `public inline auto `[`getCount`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1aec17d0368843d5db4e34fe4d8b83b926)`() const` 

#### Returns
Returns the amount of vertices in the buffer.

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1aac3ab46655bc4f4940fc1f8c4a49d046)`(const std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex)` > & vertices)` 

Creates the buffer, allocates memory for it and fills it with provided data.

#### Parameters
* `vertices` The vertex data.

# class `RAYEX_NAMESPACE::Image` 

A wrapper class for a Vulkan image.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1abc8b3131fd792888cd54cae0338ebc5a)`() const` | #### Returns
`public inline auto `[`getExtent`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1a5a4ccd30feb5e47b653997faac3e4adb)`() const` | #### Returns
`public inline auto `[`getFormat`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1a189584e4d28a640f7a91e2711aac4bb4)`() const` | #### Returns
`public inline auto `[`getLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1ab3f6fda72b3f94186af3bca7230195c6)`() const` | #### Returns
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1a2e018b9b5ea0b1d6cb387357ad721593)`(const vk::ImageCreateInfo & createInfo)` | Creates the image and allocates memory for it.
`public void `[`transitionToLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1a4905385e6ab351022f1cc717dbd1cb6f)`(vk::ImageLayout layout)` | Used to transition this image's layout.
`public void `[`transitionToLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1abd46b661d5593c29cf4b9960e2b30681)`(vk::ImageLayout layout,vk::CommandBuffer commandBuffer)` | Used to transition this image's layout.
`protected vk::UniqueImage `[`_image`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1ada6c5252fc7fb31fa855c1012cc05ac9) | The Vulkan image with a unique handle.
`protected vk::UniqueDeviceMemory `[`_memory`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1aae687e65b91862f544568ef874768b8b) | The image's memory with a unique handle.
`protected vk::Extent3D `[`_extent`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1a8834ea7c4275a33ee2e25dbe26db3b4b) | The image's extent.
`protected vk::Format `[`_format`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1ab6642f61a0126ead754188b04b28842b) | The image's format.
`protected vk::ImageLayout `[`_layout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1a59216cbdf2bf251eb817c00fdff907a8) | THe image's layout.

## Members

#### `public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1abc8b3131fd792888cd54cae0338ebc5a)`() const` 

#### Returns
Returns the Vulkan image object without the unique handle.

#### `public inline auto `[`getExtent`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1a5a4ccd30feb5e47b653997faac3e4adb)`() const` 

#### Returns
Returns the iamge's extent.

#### `public inline auto `[`getFormat`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1a189584e4d28a640f7a91e2711aac4bb4)`() const` 

#### Returns
Returns the image's format.

#### `public inline auto `[`getLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1ab3f6fda72b3f94186af3bca7230195c6)`() const` 

#### Returns
Returns the image's layout.

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1a2e018b9b5ea0b1d6cb387357ad721593)`(const vk::ImageCreateInfo & createInfo)` 

Creates the image and allocates memory for it.

#### Parameters
* `createInfo` The Vulkan image create info.

#### `public void `[`transitionToLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1a4905385e6ab351022f1cc717dbd1cb6f)`(vk::ImageLayout layout)` 

Used to transition this image's layout.

#### Parameters
* `layout` The target layout. 

This function creates its own single-time usage command buffer.

#### `public void `[`transitionToLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1abd46b661d5593c29cf4b9960e2b30681)`(vk::ImageLayout layout,vk::CommandBuffer commandBuffer)` 

Used to transition this image's layout.

#### Parameters
* `layout` The target layout 

* `commandBuffer` The command buffer that will be used to set up a pipeline barrier. 

It is required to call vk::CommandBuffer::begin() or [RAYEX_NAMESPACE::CommandBuffer::begin()](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1adeb8480e5423081dae4f34e7c756f5d0) before calling this function.

#### `protected vk::UniqueImage `[`_image`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1ada6c5252fc7fb31fa855c1012cc05ac9) 

The Vulkan image with a unique handle.

#### `protected vk::UniqueDeviceMemory `[`_memory`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1aae687e65b91862f544568ef874768b8b) 

The image's memory with a unique handle.

#### `protected vk::Extent3D `[`_extent`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1a8834ea7c4275a33ee2e25dbe26db3b4b) 

The image's extent.

#### `protected vk::Format `[`_format`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1ab6642f61a0126ead754188b04b28842b) 

The image's format.

#### `protected vk::ImageLayout `[`_layout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_image_1a59216cbdf2bf251eb817c00fdff907a8) 

THe image's layout.

# class `RAYEX_NAMESPACE::Texture` 

```
class RAYEX_NAMESPACE::Texture
  : public RAYEX_NAMESPACE::Image
```  

A helper class for creating ready-to-use textures.

> Todo: textureCounter is being incremented always, even if it is not a new texture. This class should actually look inside Api::textures or sth to assign the correct index.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint32_t `[`_offset`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_texture_1ae964bda9a837a27b4a726ee27442c9fa) | 
`public inline auto `[`getImageView`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_texture_1a23e2e5a02dc202a05770c5157e00b639)`() const` | #### Returns
`public inline auto `[`getPath`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_texture_1ab60fb85070b67258d51ba665725ad92e)`() const` | #### Returns
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_texture_1acc3e7b81bbfd57b79f5bd65ff0ae674d)`(std::string_view path)` | Creates the texture.

## Members

#### `public uint32_t `[`_offset`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_texture_1ae964bda9a837a27b4a726ee27442c9fa) 

#### `public inline auto `[`getImageView`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_texture_1a23e2e5a02dc202a05770c5157e00b639)`() const` 

#### Returns
Returns the texture's image view.

#### `public inline auto `[`getPath`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_texture_1ab60fb85070b67258d51ba665725ad92e)`() const` 

#### Returns
Returns the relative path of the texture file.

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_texture_1acc3e7b81bbfd57b79f5bd65ff0ae674d)`(std::string_view path)` 

Creates the texture.

#### Parameters
* `path` The relative path to the texture file. 

> Todo: Latest changes to this class might be causing errors once texture are working again.

# class `RAYEX_NAMESPACE::Pipeline` 

A wrapper class for a Vulkan graphics pipeline.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_pipeline_1a560c14e9ae27b02d347ded573a9f1ca3)`(const std::vector< vk::DescriptorSetLayout > & descriptorSetLayouts,vk::RenderPass renderPass,vk::Viewport viewport,vk::Rect2D scissor,const `[`Settings`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings)` * settings)` | Initializes a rasterization pipeline.
`public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_pipeline_1aceb8993023c4f274bb367bc190dc2397)`() const` | #### Returns
`public inline auto `[`getLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_pipeline_1a42a996345362e5e0972ff7f7ba97b1a9)`() const` | #### Returns

## Members

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_pipeline_1a560c14e9ae27b02d347ded573a9f1ca3)`(const std::vector< vk::DescriptorSetLayout > & descriptorSetLayouts,vk::RenderPass renderPass,vk::Viewport viewport,vk::Rect2D scissor,const `[`Settings`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings)` * settings)` 

Initializes a rasterization pipeline.

#### Parameters
* `descriptorSetLayouts` A vector of descriptor set layouts that will be included in the pipeline layout. 

* `renderPass` A Vulkan render pass. 

* `viewport` The desired view port. 

* `scissor` The desired scissor dimensions. 

* `settings` Used to retrieve information like maximum instances or lights of a specific type.

#### `public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_pipeline_1aceb8993023c4f274bb367bc190dc2397)`() const` 

#### Returns
Returns the Vulkan pipeline object without the unique handle.

#### `public inline auto `[`getLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_pipeline_1a42a996345362e5e0972ff7f7ba97b1a9)`() const` 

#### Returns
Returns the Vulkan pipeline layout object without the unique handle.

# class `RAYEX_NAMESPACE::RayTracingBuilder` 

Manages the building process of the acceleration structures.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a3550f65cf36456128e878ca1890bd7b6)`() = default` | 
`public  `[`~RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a7ab7f534ea552b547e1334289f654642)`()` | Calls [destroy()](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a3189e091b6301e38a325310edf894beb).
`public  `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a39c40e281657df8226d600b21a31acdf)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &) = delete` | 
`public  `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a32e7ec906d20fbd16158e030cbfa83a5)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1ab85ed5e5008b6f83a962a768926c855a)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a5475f3045b7d1a71019d3e0169020211)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &&) = delete` | 
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1afdd8c600cef05e5e9bfd6dbb31182f4f)`()` | Retrieves the physical device's ray tracing capabilities.
`public void `[`destroy`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a3189e091b6301e38a325310edf894beb)`()` | Destroys all bottom and top level acceleration structures.
`public inline auto `[`getTlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a48aa91a99a89e50b3b6209778f04057c)`() const` | #### Returns
`public inline auto `[`getRtProperties`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a09fe2e3ccb7a99c43ee54e5a8852f779)`() const` | #### Returns
`public inline auto `[`getStorageImageView`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a768200b01d60ce411a687b95627195b5)`() const` | #### Returns
`public inline auto `[`getPipeline`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a71070812d4d677caf2dc1b12af5efe17)`() const` | #### Returns
`public inline auto `[`getPipelineLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1aa90251bb96a16dd99d82c10d993300dd)`() const` | #### Returns
`public auto `[`modelToBlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a930233f3a42a6b863b177bfc93e71777)`(const `[`VertexBuffer`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer)` & vertexBuffer,const `[`IndexBuffer`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_index_buffer)` & indexBuffer) const` | Used to convert wavefront models to a bottom level acceleration structure.
`public auto `[`instanceToVkGeometryInstanceKHR`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a60388a55e372319ce0d2158c7267ad56)`(const `[`BlasInstance`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance)` & instance)` | Used to convert a bottom level acceleration structure instance to a Vulkan geometry instance.
`public void `[`createBottomLevelAS`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a42b8ce4dcbd3a85f5e923c21432b529e)`(const std::vector< `[`VertexBuffer`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer)` > & vertexBuffers,const std::vector< `[`IndexBuffer`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_index_buffer)` > & indexBuffers)` | Used to prepare building the bottom level acceleration structures.
`public void `[`buildBlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1af900141703e8b05e7840aa4473557708)`(const std::vector< `[`Blas`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas)` > & blas_,vk::BuildAccelerationStructureFlagsKHR flags)` | Builds all bottom level acceleration structures.
`public void `[`createTopLevelAS`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a5c6a10e0124ce4356bc41f9848cf85e9)`(const std::vector< std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` >> & geometryInstances)` | Used to prepare building the top level acceleration structure.
`public void `[`buildTlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a90a77c2884d4831c6af205c37836cc4b)`(const std::vector< `[`BlasInstance`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance)` > & instances,vk::BuildAccelerationStructureFlagsKHR flags)` | Build the top level acceleration structure.
`public void `[`createStorageImage`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1ab224b9213162354a3a540fc7708c47b5)`(vk::Extent2D swapchainExtent)` | Creates the storage image which the ray tracing shaders will write to.
`public void `[`createShaderBindingTable`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1af5fb816195cf838a10d566fc4aa5fae8)`()` | Creates the shader binding tables.
`public void `[`createPipeline`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1aa3662d69cbf85434c89e831fe6bbcb0c)`(const std::vector< vk::DescriptorSetLayout > & descriptorSetLayouts,const `[`Settings`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings)` * settings)` | Used to create a ray tracing pipeline.
`public void `[`rayTrace`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1ac62d95b180821e50c060e3722c229e7a)`(vk::CommandBuffer swapchaincommandBuffer,vk::Image swapchainImage,vk::Extent2D extent)` | Used to record the actual ray tracing commands to a given command buffer.

## Members

#### `public  `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a3550f65cf36456128e878ca1890bd7b6)`() = default` 

#### `public  `[`~RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a7ab7f534ea552b547e1334289f654642)`()` 

Calls [destroy()](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a3189e091b6301e38a325310edf894beb).

#### `public  `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a39c40e281657df8226d600b21a31acdf)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &) = delete` 

#### `public  `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a32e7ec906d20fbd16158e030cbfa83a5)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1ab85ed5e5008b6f83a962a768926c855a)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a5475f3045b7d1a71019d3e0169020211)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &&) = delete` 

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1afdd8c600cef05e5e9bfd6dbb31182f4f)`()` 

Retrieves the physical device's ray tracing capabilities.

#### `public void `[`destroy`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a3189e091b6301e38a325310edf894beb)`()` 

Destroys all bottom and top level acceleration structures.

#### `public inline auto `[`getTlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a48aa91a99a89e50b3b6209778f04057c)`() const` 

#### Returns
Returns the top level acceleration structure.

#### `public inline auto `[`getRtProperties`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a09fe2e3ccb7a99c43ee54e5a8852f779)`() const` 

#### Returns
Returns the physical device's ray tracing properties.

#### `public inline auto `[`getStorageImageView`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a768200b01d60ce411a687b95627195b5)`() const` 

#### Returns
Returns the storage image's image view.

#### `public inline auto `[`getPipeline`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a71070812d4d677caf2dc1b12af5efe17)`() const` 

#### Returns
Returns the ray tracing pipeline.

#### `public inline auto `[`getPipelineLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1aa90251bb96a16dd99d82c10d993300dd)`() const` 

#### Returns
Returns the ray tracing pipeline layout.

#### `public auto `[`modelToBlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a930233f3a42a6b863b177bfc93e71777)`(const `[`VertexBuffer`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer)` & vertexBuffer,const `[`IndexBuffer`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_index_buffer)` & indexBuffer) const` 

Used to convert wavefront models to a bottom level acceleration structure.

#### Parameters
* `vertexBuffer` A vertex buffer of some geometry. 

* `indexBuffer` An index buffer of some geometry. 

#### Returns
Returns the bottom level acceleration structure.

#### `public auto `[`instanceToVkGeometryInstanceKHR`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a60388a55e372319ce0d2158c7267ad56)`(const `[`BlasInstance`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance)` & instance)` 

Used to convert a bottom level acceleration structure instance to a Vulkan geometry instance.

#### Parameters
* `instance` A bottom level acceleration structure instance. 

#### Returns
Returns the Vulkan geometry instance.

#### `public void `[`createBottomLevelAS`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a42b8ce4dcbd3a85f5e923c21432b529e)`(const std::vector< `[`VertexBuffer`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer)` > & vertexBuffers,const std::vector< `[`IndexBuffer`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_index_buffer)` > & indexBuffers)` 

Used to prepare building the bottom level acceleration structures.

#### Parameters
* `vertexBuffers` [Vertex](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex) buffers of all geometry in the scene. 

* `indexBuffers` Index buffers of all geometry in the scene.

#### `public void `[`buildBlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1af900141703e8b05e7840aa4473557708)`(const std::vector< `[`Blas`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas)` > & blas_,vk::BuildAccelerationStructureFlagsKHR flags)` 

Builds all bottom level acceleration structures.

#### Parameters
* `blas_` A vector of [RAYEX_NAMESPACE::Blas](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas) objects containing all bottom level acceleration structures prepared in [createBottomLevelAS()](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a42b8ce4dcbd3a85f5e923c21432b529e). 

* `flags` The build flags.

#### `public void `[`createTopLevelAS`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a5c6a10e0124ce4356bc41f9848cf85e9)`(const std::vector< std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` >> & geometryInstances)` 

Used to prepare building the top level acceleration structure.

#### Parameters
* `geometryInstances` All geometry instances in the scene.

#### `public void `[`buildTlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a90a77c2884d4831c6af205c37836cc4b)`(const std::vector< `[`BlasInstance`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance)` > & instances,vk::BuildAccelerationStructureFlagsKHR flags)` 

Build the top level acceleration structure.

#### Parameters
* `instances` A vector of bottom level acceleration structure instances. 

* `flags` The build flags.

#### `public void `[`createStorageImage`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1ab224b9213162354a3a540fc7708c47b5)`(vk::Extent2D swapchainExtent)` 

Creates the storage image which the ray tracing shaders will write to.

#### Parameters
* `swapchainExtent` The swapchain images' extent.

#### `public void `[`createShaderBindingTable`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1af5fb816195cf838a10d566fc4aa5fae8)`()` 

Creates the shader binding tables.

#### `public void `[`createPipeline`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1aa3662d69cbf85434c89e831fe6bbcb0c)`(const std::vector< vk::DescriptorSetLayout > & descriptorSetLayouts,const `[`Settings`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings)` * settings)` 

Used to create a ray tracing pipeline.

#### Parameters
* `descriptorSetLayouts` The descriptor set layouts for the shaders. 

* `settings` The renderer settings.

#### `public void `[`rayTrace`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1ac62d95b180821e50c060e3722c229e7a)`(vk::CommandBuffer swapchaincommandBuffer,vk::Image swapchainImage,vk::Extent2D extent)` 

Used to record the actual ray tracing commands to a given command buffer.

#### Parameters
* `swapchaincommandBuffer` The command buffer to record to. 

* `swapchainImage` The current image in the swapchain. 

* `extent` The swapchain images' extent.

# class `RAYEX_NAMESPACE::RenderPass` 

A wrapper class for a Vulkan render pass.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_render_pass_1a54b6544e81c939178fcd3204703cd795)`() const` | #### Returns
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_render_pass_1a59530fba847d5b02e6e705d5257562aa)`(const std::vector< vk::AttachmentDescription > & attachments,const std::vector< vk::SubpassDescription > & subpasses,const std::vector< vk::SubpassDependency > & dependencies)` | Initializes the Vulkan render pass.
`public void `[`begin`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_render_pass_1ae7cd1dc3852a28c45702bacd5958c533)`(vk::Framebuffer framebuffer,vk::CommandBuffer commandBuffer,vk::Rect2D renderArea,const std::vector< vk::ClearValue > & clearValues) const` | Call to begin the render pass.
`public void `[`end`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_render_pass_1a34096c7ba0418e1583a0fb255f975d02)`(vk::CommandBuffer commandBuffer) const` | Call to end the render pass.

## Members

#### `public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_render_pass_1a54b6544e81c939178fcd3204703cd795)`() const` 

#### Returns
Returns the Vulkan render pass without the unique handle.

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_render_pass_1a59530fba847d5b02e6e705d5257562aa)`(const std::vector< vk::AttachmentDescription > & attachments,const std::vector< vk::SubpassDescription > & subpasses,const std::vector< vk::SubpassDependency > & dependencies)` 

Initializes the Vulkan render pass.

#### Parameters
* `attachments` The Vulkan attachment description. 

* `subpasses` The Vulkan subpass description. 

* `dependencies` The Vulkan subpass dependencies.

#### `public void `[`begin`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_render_pass_1ae7cd1dc3852a28c45702bacd5958c533)`(vk::Framebuffer framebuffer,vk::CommandBuffer commandBuffer,vk::Rect2D renderArea,const std::vector< vk::ClearValue > & clearValues) const` 

Call to begin the render pass.

#### Parameters
* `framebuffer` The swapchain framebuffer. 

* `commandBuffer` The command buffer used to begin the render pass. 

* `renderArea` Defines the size of the render area. 

* `clearValues` The clear values. 

[RAYEX_NAMESPACE::CommandBuffer::begin()](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1adeb8480e5423081dae4f34e7c756f5d0) or vk::CommandBuffer::begin() must have been already called prior to calling this function.

#### `public void `[`end`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_render_pass_1a34096c7ba0418e1583a0fb255f975d02)`(vk::CommandBuffer commandBuffer) const` 

Call to end the render pass.

#### Parameters
* `commandBuffer`

# class `RAYEX_NAMESPACE::Surface` 

A wrapper class for a Vulkan surface.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a50507969dee14dca7f6d7caabc140f3f)`() = default` | 
`public  `[`~Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a067165bde59c846051f20c18e635ffe7)`()` | Calls destroy().
`public  `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1aff20a5ec7ec7eda1408ea424e007dabd)`(const `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface)` &) = delete` | 
`public  `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a366763190dfacc0bd80d1ee7551928c3)`(const `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1af379c8a571dcefa5494a12d27b1c2743)`(const `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface)` &) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1aec6dd8d9491bd12b5f5a3dae75844ac6)`(const `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface)` &&) = delete` | 
`public inline auto `[`getFormat`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a11570390ba2497255d964315e0e3afaf)`() const` | #### Returns
`public inline auto `[`getColorSpace`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a70902541ad7dfec48032ca084f913378)`() const` | #### Returns
`public inline auto `[`getPresentMode`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a48595aa6fcd51e7a83524e69f7498d47)`() const` | #### Returns
`public inline auto `[`getCapabilities`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a4ce2ebf931003afa86f9a0497b4b8d06)`() const` | #### Returns
`public inline auto `[`getExtent`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a0c406f4f252f4a6de19daf3f1bff8d5f)`() const` | #### Returns
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1adc4ef455c8eabfba752bfe7702ada7ff)`(std::shared_ptr< `[`Window`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` > window)` | Initializes the Vulkan surface object.
`public void `[`assessSettings`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a75d89305da7ff2a6e62619637ff7bf37)`()` | Checks if the preferred settings for format, color space and present mode are available.

## Members

#### `public  `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a50507969dee14dca7f6d7caabc140f3f)`() = default` 

#### `public  `[`~Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a067165bde59c846051f20c18e635ffe7)`()` 

Calls destroy().

#### `public  `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1aff20a5ec7ec7eda1408ea424e007dabd)`(const `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface)` &) = delete` 

#### `public  `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a366763190dfacc0bd80d1ee7551928c3)`(const `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1af379c8a571dcefa5494a12d27b1c2743)`(const `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface)` &) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1aec6dd8d9491bd12b5f5a3dae75844ac6)`(const `[`Surface`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface)` &&) = delete` 

#### `public inline auto `[`getFormat`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a11570390ba2497255d964315e0e3afaf)`() const` 

#### Returns
Returns the surface format.

#### `public inline auto `[`getColorSpace`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a70902541ad7dfec48032ca084f913378)`() const` 

#### Returns
Returns the surface's color space.

#### `public inline auto `[`getPresentMode`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a48595aa6fcd51e7a83524e69f7498d47)`() const` 

#### Returns
Returns the surface's present mode.

#### `public inline auto `[`getCapabilities`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a4ce2ebf931003afa86f9a0497b4b8d06)`() const` 

#### Returns
Returns the surface's capabilities.

#### `public inline auto `[`getExtent`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a0c406f4f252f4a6de19daf3f1bff8d5f)`() const` 

#### Returns
Returns the surface's extent.

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1adc4ef455c8eabfba752bfe7702ada7ff)`(std::shared_ptr< `[`Window`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` > window)` 

Initializes the Vulkan surface object.

#### Parameters
* `window` A window to create a surface for. 

If any of the specified format, color space and present mode are not available the function will fall back to settings that are guaranteed to be supported.

#### `public void `[`assessSettings`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface_1a75d89305da7ff2a6e62619637ff7bf37)`()` 

Checks if the preferred settings for format, color space and present mode are available.

If not, the function will set them to some fallback values. Must be called right after the enumeration of the physical device.

# class `RAYEX_NAMESPACE::Swapchain` 

A wrapper class for a Vulkan swapchain.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1ad85e687e114973ff8fc6b852db078879)`(`[`Surface`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface)` * surface,vk::RenderPass renderPass)` | Creates the swapchain, the swapchain images and their image views as well as their framebuffers.
`public void `[`destroy`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a23485ef75854f3ffb881877e18ddd2c2)`()` | Destroys the swapchain.
`public inline auto `[`getFramebuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1ab3315f20384b5fa3e1e4156bd1c638c7)`(uint32_t index) const` | #### Returns
`public inline auto `[`getCurrentImageIndex`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1ac7a765f1861061db7373460b77ddeb3e)`() const` | #### Returns
`public inline auto `[`getExtent`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a9789ac0aaa93867052947d4445f2a0e2)`() const` | #### Returns
`public inline auto `[`getImageAspect`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a58e9cbc1d98d0eb97ee273b15c1f4713)`() const` | #### Returns
`public inline auto `[`getImage`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a190ce92331100a2db14c729d67341b9f)`(size_t index) const` | Returns the swapchain image at a given index.
`public inline auto `[`getImages`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a747f3cd62d8784b05baf6e192d86002a)`() const` | #### Returns
`public inline auto `[`getImageViews`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a30b4e27c52d6fa9bb634b5cb579b7b10)`() const` | #### Returns
`public void `[`setImageAspect`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a50a497610d30300482aa3d2a5f05e6ff)`(vk::ImageAspectFlags flags)` | Used to set the desired image aspect flags.
`public void `[`setImageLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a9d4b15bff7fb5bda19cb5b7dd02783f4)`(vk::ImageLayout oldLayout,vk::ImageLayout newLayout)` | Used to transition from one layout to another.
`public void `[`acquireNextImage`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a3df5993a7b2582f01bad19a397cf0afe)`(vk::Semaphore semaphore,vk::Fence fence)` | Retrieves the next swapchain image.

## Members

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1ad85e687e114973ff8fc6b852db078879)`(`[`Surface`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface)` * surface,vk::RenderPass renderPass)` 

Creates the swapchain, the swapchain images and their image views as well as their framebuffers.

#### Parameters
* `surface` A pointer to a [RAYEX_NAMESPACE::Surface](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface) object. 

* `renderPass` The render pass to create the framebuffers.

#### `public void `[`destroy`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a23485ef75854f3ffb881877e18ddd2c2)`()` 

Destroys the swapchain.

#### `public inline auto `[`getFramebuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1ab3315f20384b5fa3e1e4156bd1c638c7)`(uint32_t index) const` 

#### Returns
Returns the swapchain framebuffer at a given index.

#### `public inline auto `[`getCurrentImageIndex`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1ac7a765f1861061db7373460b77ddeb3e)`() const` 

#### Returns
Returns the current swapchain image index.

#### `public inline auto `[`getExtent`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a9789ac0aaa93867052947d4445f2a0e2)`() const` 

#### Returns
Returns the swapchain images' extent.

#### `public inline auto `[`getImageAspect`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a58e9cbc1d98d0eb97ee273b15c1f4713)`() const` 

#### Returns
Returns the swapchain images' image aspect.

#### `public inline auto `[`getImage`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a190ce92331100a2db14c729d67341b9f)`(size_t index) const` 

Returns the swapchain image at a given index.

#### Parameters
* `index` The index of the swapchain image. 

#### Returns
The swapchain image.

#### `public inline auto `[`getImages`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a747f3cd62d8784b05baf6e192d86002a)`() const` 

#### Returns
Returns a vector containing all swapchain images.

#### `public inline auto `[`getImageViews`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a30b4e27c52d6fa9bb634b5cb579b7b10)`() const` 

#### Returns
Returns a vector containing all swapchain image views. 

> Todo: Returning by reference will result in size 0.

#### `public void `[`setImageAspect`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a50a497610d30300482aa3d2a5f05e6ff)`(vk::ImageAspectFlags flags)` 

Used to set the desired image aspect flags.

#### `public void `[`setImageLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a9d4b15bff7fb5bda19cb5b7dd02783f4)`(vk::ImageLayout oldLayout,vk::ImageLayout newLayout)` 

Used to transition from one layout to another.

#### Parameters
* `oldLayout` The swapchain images' current image layout. 

* `newLayout` The target image layout.

#### `public void `[`acquireNextImage`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a3df5993a7b2582f01bad19a397cf0afe)`(vk::Semaphore semaphore,vk::Fence fence)` 

Retrieves the next swapchain image.

#### Parameters
* `semaphore` A semaphore to signal. 

* `fence` A fence to signal.

# class `RAYEX_NAMESPACE::DebugMessenger` 

A wrapper class for a Vulkan debug utility messenger.

The class features scope-bound destruction.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a69fbeacdb2db1302c0359c274a3ad181)`() = default` | 
`public  `[`~DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a26dcf2dc1fc8b925737aa21b0fc7c278)`()` | Calls destroy().
`public  `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a9301709792a38dac2faf5ed369f40bad)`(const `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &) = delete` | 
`public  `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a02dd17d502dd32e655f8013ecdfc3c3a)`(const `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1adfe593687a69b8a15a8153f52701e9a0)`(const `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1adad322d648f63748f7aba7817459015a)`(const `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &&) = delete` | 
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1ad85d8efe16f0ef5fd0a1b632efc100ba)`(vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity,vk::DebugUtilsMessageTypeFlagsEXT messageType)` | Creates the debug messenger with the given properties.

## Members

#### `public  `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a69fbeacdb2db1302c0359c274a3ad181)`() = default` 

#### `public  `[`~DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a26dcf2dc1fc8b925737aa21b0fc7c278)`()` 

Calls destroy().

#### `public  `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a9301709792a38dac2faf5ed369f40bad)`(const `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &) = delete` 

#### `public  `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a02dd17d502dd32e655f8013ecdfc3c3a)`(const `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1adfe593687a69b8a15a8153f52701e9a0)`(const `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1adad322d648f63748f7aba7817459015a)`(const `[`DebugMessenger`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &&) = delete` 

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1ad85d8efe16f0ef5fd0a1b632efc100ba)`(vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity,vk::DebugUtilsMessageTypeFlagsEXT messageType)` 

Creates the debug messenger with the given properties.

#### Parameters
* `messageSeverity` - Specifies the type of severity of messages that will be logged. 

* `messageType` - Specifies the types of messages that will be logged.

# class `RAYEX_NAMESPACE::Gui` 

A class to create an ImGui-based GUI.

This class acts like an interface for the user to create their own GUI. It is possible to create multiple GUI objects and re-assign them to the renderer at runtime. 
```cpp
class CustomGui : public Gui
{
private:
  // Configure style and input of the GUI.
  void [configure](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a9683c711717892adbc9a5091ba5a1018)() override { }

  // Put the rendering commands in here.
  void [render](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a3e5134da1391b2ca38bc65f17895d767)() override { }
};

RayexmyRenderer;
myRenderer.init( );

// Put this line after the declaration of the RAYEX_NAMESPACE::Rayexobject.
auto myGui = std::make_shared<CustomGui>( );
myRenderer.setGui( myGui );
```

The GUI object must be declared after the renderer to ensure the correct order for destroying and freeing resources.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public virtual  `[`~Gui`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a37b52ebf2ec5b33b5bb817b6dc157169)`() = default` | 
`public inline auto `[`getCommandBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a8e3a6b5982891d85abb8dc8613f1d249)`(uint32_t index) const` | Returns the GUI's command buffer at the given index.
`public virtual void `[`configure`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a9683c711717892adbc9a5091ba5a1018)`()` | Used to configure all ImGui settings.
`public virtual void `[`render`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a3e5134da1391b2ca38bc65f17895d767)`()` | This function is for calling the individual ImGui components, e.g.
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a0f744057e610d3c5e0417664dc78d358)`(SDL_Window * window,const `[`Surface`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface)` * surface,vk::Extent2D swapchainImageExtent,const std::vector< vk::ImageView > & swapchainImageViews)` | Creates the GUI and all required Vulkan components.
`public void `[`recreate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a56569780013e4a2c2bcd78ce86ee5712)`(vk::Extent2D swapchainImageExtent,const std::vector< vk::ImageView > & swapchainImageViews)` | Used to recreate the GUI in case the window size was changed.
`public void `[`renderDrawData`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a1eef3b87927ad2b9823a70d0f8ae8c8b)`(uint32_t imageIndex)` | Records the ImGui rendering calls to the command buffer at the given image index.
`public void `[`destroy`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1acdc42f7c0d1a54a0a2e3bc9bb89df3ee)`()` | Destroys all ImGui resources.

## Members

#### `public virtual  `[`~Gui`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a37b52ebf2ec5b33b5bb817b6dc157169)`() = default` 

#### `public inline auto `[`getCommandBuffer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a8e3a6b5982891d85abb8dc8613f1d249)`(uint32_t index) const` 

Returns the GUI's command buffer at the given index.

#### Parameters
* `index` The index of the command buffer to access. 

#### Returns
Returns the Vulkan command buffer.

#### `public virtual void `[`configure`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a9683c711717892adbc9a5091ba5a1018)`()` 

Used to configure all ImGui settings.

The user should override this function if they want to change the style or various other configuration settings.

#### `public virtual void `[`render`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a3e5134da1391b2ca38bc65f17895d767)`()` 

This function is for calling the individual ImGui components, e.g.

widgets.

The user should override this function to create their own GUI.

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a0f744057e610d3c5e0417664dc78d358)`(SDL_Window * window,const `[`Surface`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface)` * surface,vk::Extent2D swapchainImageExtent,const std::vector< vk::ImageView > & swapchainImageViews)` 

Creates the GUI and all required Vulkan components.

#### Parameters
* `surface` A pointer to a [RAYEX_NAMESPACE::Surface](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_surface) object. 

* `swapchainImageExtent` The extent of the swapchain images. 

* `swapchainImageViews` The swapchain images' image views.

#### `public void `[`recreate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a56569780013e4a2c2bcd78ce86ee5712)`(vk::Extent2D swapchainImageExtent,const std::vector< vk::ImageView > & swapchainImageViews)` 

Used to recreate the GUI in case the window size was changed.

#### Parameters
* `swapchainImageExtent` The extent of the swapchain images. 

* `swapchainImageViews` The swapchain images' image views.

#### `public void `[`renderDrawData`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a1eef3b87927ad2b9823a70d0f8ae8c8b)`(uint32_t imageIndex)` 

Records the ImGui rendering calls to the command buffer at the given image index.

#### Parameters
* `imageIndex` The index addressing a command buffer.

#### `public void `[`destroy`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1acdc42f7c0d1a54a0a2e3bc9bb89df3ee)`()` 

Destroys all ImGui resources.

# struct `RAYEX_NAMESPACE::Descriptors` 

Encapsulates descriptor-related resources.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public vk::UniqueDescriptorSetLayout `[`layout`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_descriptors_1a51cfd85e56864588a7f9b5380b31cb39) | 
`public vk::UniqueDescriptorPool `[`pool`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_descriptors_1a7142e00b8b54818edec0ff40fb322671) | 
`public `[`Bindings`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings)` `[`bindings`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_descriptors_1a3563c7ff3e828a1657fc61e90501e9c5) | 

## Members

#### `public vk::UniqueDescriptorSetLayout `[`layout`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_descriptors_1a51cfd85e56864588a7f9b5380b31cb39) 

#### `public vk::UniqueDescriptorPool `[`pool`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_descriptors_1a7142e00b8b54818edec0ff40fb322671) 

#### `public `[`Bindings`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_bindings)` `[`bindings`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_descriptors_1a3563c7ff3e828a1657fc61e90501e9c5) 

# struct `RAYEX_NAMESPACE::DirectionalLightSSBO` 

A wrapper for [RAYEX_NAMESPACE::DirectionalLight](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light) matching the buffer alignment requirements.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::vec4 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light_s_s_b_o_1aab0019920e00900b5320c6a51cc6f054) | Stores ambient color (vec3) and ambient intensity (float) in a vec4.
`public glm::vec4 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light_s_s_b_o_1a0ffa18b4208e9101d569fe89f0539772) | Stores diffuse color (vec3) and diffuse intensity (float) in a vec4.
`public glm::vec4 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light_s_s_b_o_1ab6d1589a8c75bcf6a1db83ecc31ea6aa) | Stores specular color (vec3) and specular intensity (float) in a vec4.
`public glm::vec4 `[`direction`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light_s_s_b_o_1a97c0fc692504df2515640a3d3bb25527) | Stores the direction (vec3).

## Members

#### `public glm::vec4 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light_s_s_b_o_1aab0019920e00900b5320c6a51cc6f054) 

Stores ambient color (vec3) and ambient intensity (float) in a vec4.

#### `public glm::vec4 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light_s_s_b_o_1a0ffa18b4208e9101d569fe89f0539772) 

Stores diffuse color (vec3) and diffuse intensity (float) in a vec4.

#### `public glm::vec4 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light_s_s_b_o_1ab6d1589a8c75bcf6a1db83ecc31ea6aa) 

Stores specular color (vec3) and specular intensity (float) in a vec4.

#### `public glm::vec4 `[`direction`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light_s_s_b_o_1a97c0fc692504df2515640a3d3bb25527) 

Stores the direction (vec3).

# struct `RAYEX_NAMESPACE::PointLightSSBO` 

A wrapper for [RAYEX_NAMESPACE::PointLight](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light) matching the buffer alignment requirements.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::vec4 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light_s_s_b_o_1ad839bddcadcd269f6b393732d999b87a) | Stores ambient color (vec3) and ambient intensity (float) in a vec4.
`public glm::vec4 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light_s_s_b_o_1ae70e8c5423fb2da8ba975d8b514abdb7) | Stores diffuse color (vec3) and diffuse intensity (float) in a vec4.
`public glm::vec4 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light_s_s_b_o_1a73d3ca7e5f890bd225cce2f923994189) | Stores specular color (vec3) and specular intensity (float) in a vec4.
`public glm::vec4 `[`position`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light_s_s_b_o_1a67b8025d34fb9404861de4ebbabb5e5f) | Stores the position (vec3).

## Members

#### `public glm::vec4 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light_s_s_b_o_1ad839bddcadcd269f6b393732d999b87a) 

Stores ambient color (vec3) and ambient intensity (float) in a vec4.

#### `public glm::vec4 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light_s_s_b_o_1ae70e8c5423fb2da8ba975d8b514abdb7) 

Stores diffuse color (vec3) and diffuse intensity (float) in a vec4.

#### `public glm::vec4 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light_s_s_b_o_1a73d3ca7e5f890bd225cce2f923994189) 

Stores specular color (vec3) and specular intensity (float) in a vec4.

#### `public glm::vec4 `[`position`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light_s_s_b_o_1a67b8025d34fb9404861de4ebbabb5e5f) 

Stores the position (vec3).

# struct `RAYEX_NAMESPACE::MeshSSBO` 

A wrapper for [RAYEX_NAMESPACE::MeshSSBO](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o) matching the buffer alignment requirements.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::vec4 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a7ee226655334d745110ea673d3f415b7) | 
`public glm::vec4 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1ad7dfd9e1d94bf8b61d75c5cb5b96029c) | 
`public glm::vec4 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a82baebb3cacb3f4b74ef8621fe9fdb34) | 
`public glm::vec4 `[`padding0`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a7f00903c22d51da1da0c607ad9259591) | [Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).
`public glm::vec4 `[`padding1`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a5f38437cb39cd03e69e5514fbe691935) | [Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).
`public glm::vec4 `[`padding2`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1aa396e2f1a369b70c45fd9b7931255c7b) | [Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).
`public uint32_t `[`indexOffset`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a8a30eb7713b788d6abf948cbe029acdd) | Refers to the offset of this mesh inside a [Geometry::indices](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a11ecc2461cc8f940666cc5535e977c0f) container.
`public uint32_t `[`padding3`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a3357df15c77fc36d71dedd31ae8804c7) | [Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).
`public uint32_t `[`padding4`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a4869eb36dc3e6866d4921a776d0dcc98) | [Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).
`public uint32_t `[`padding5`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1aeda9c4d1309014510fc7a82caa4524c7) | [Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).

## Members

#### `public glm::vec4 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a7ee226655334d745110ea673d3f415b7) 

#### `public glm::vec4 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1ad7dfd9e1d94bf8b61d75c5cb5b96029c) 

#### `public glm::vec4 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a82baebb3cacb3f4b74ef8621fe9fdb34) 

#### `public glm::vec4 `[`padding0`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a7f00903c22d51da1da0c607ad9259591) 

[Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).

#### `public glm::vec4 `[`padding1`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a5f38437cb39cd03e69e5514fbe691935) 

[Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).

#### `public glm::vec4 `[`padding2`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1aa396e2f1a369b70c45fd9b7931255c7b) 

[Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).

#### `public uint32_t `[`indexOffset`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a8a30eb7713b788d6abf948cbe029acdd) 

Refers to the offset of this mesh inside a [Geometry::indices](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a11ecc2461cc8f940666cc5535e977c0f) container.

#### `public uint32_t `[`padding3`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a3357df15c77fc36d71dedd31ae8804c7) 

[Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).

#### `public uint32_t `[`padding4`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a4869eb36dc3e6866d4921a776d0dcc98) 

[Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).

#### `public uint32_t `[`padding5`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1aeda9c4d1309014510fc7a82caa4524c7) 

[Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).

# struct `RAYEX_NAMESPACE::GeometryInstanceSSBO` 

A wrapper for [RAYEX_NAMESPACE::GeometryInstanceSSBO](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o) matching the buffer alignment requirements.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::mat4 `[`transform`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a549ca0f380012d4039a3c9f1a622632d) | The instance's world transform matrix.
`public glm::mat4 `[`transformIT`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a1379cb3dfff56920119ce6a1bbb38205) | The inverse transpose of transform.
`public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a4899744b1f0ee202bbf9ff02af61b4c4) | Used to assign this instance a model.
`public float `[`padding0`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1ac708ce2bf5edf245a01181441632b7ca) | [Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).
`public float `[`padding1`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a11f43ebd571824166af4d080c989706b) | [Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).
`public float `[`padding2`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a9730ed86d0ee5f518395e2b6c13ea2dc) | [Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).

## Members

#### `public glm::mat4 `[`transform`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a549ca0f380012d4039a3c9f1a622632d) 

The instance's world transform matrix.

#### `public glm::mat4 `[`transformIT`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a1379cb3dfff56920119ce6a1bbb38205) 

The inverse transpose of transform.

#### `public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a4899744b1f0ee202bbf9ff02af61b4c4) 

Used to assign this instance a model.

#### `public float `[`padding0`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1ac708ce2bf5edf245a01181441632b7ca) 

[Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).

#### `public float `[`padding1`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a11f43ebd571824166af4d080c989706b) 

[Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).

#### `public float `[`padding2`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a9730ed86d0ee5f518395e2b6c13ea2dc) 

[Buffer](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_buffer) padding (ignore).

# struct `RAYEX_NAMESPACE::CameraUbo` 

A uniform buffer object for camera data.

> Todo: [Camera](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera) position could easily be a push constant, which would make this UBO much smaller in size.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::mat4 `[`view`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1aca2343d9611de4d2cbaaafd25c9a0d4c) | 
`public glm::mat4 `[`projection`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a7903a2467fb163ff5716bf08e2621ad5) | 
`public glm::mat4 `[`viewInverse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a000d7a431db8bf2cbfe5b5d8999550d3) | 
`public glm::mat4 `[`projectionInverse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a84ded90503475453e221c733388e2fdf) | 
`public glm::vec4 `[`position`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a5145cf4a6253d5788426724810bca2ae) | 

## Members

#### `public glm::mat4 `[`view`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1aca2343d9611de4d2cbaaafd25c9a0d4c) 

#### `public glm::mat4 `[`projection`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a7903a2467fb163ff5716bf08e2621ad5) 

#### `public glm::mat4 `[`viewInverse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a000d7a431db8bf2cbfe5b5d8999550d3) 

#### `public glm::mat4 `[`projectionInverse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a84ded90503475453e221c733388e2fdf) 

#### `public glm::vec4 `[`position`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a5145cf4a6253d5788426724810bca2ae) 

# struct `RAYEX_NAMESPACE::AccelerationStructure` 

A wrapper for a Vulkan acceleration Structure.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public vk::AccelerationStructureKHR `[`as`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a310d8142a50e671e29b626943fa2b6c8) | The Vulkan acceleration structure.
`public vk::DeviceMemory `[`memory`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a67c749f7c097fac7a114c947b88752c0) | The acceleration structure's memory.
`public inline void `[`destroy`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a8e189940953cc7155465cf65a2f1d4f4)`()` | Used to destroy the acceleration structure and free its memory.

## Members

#### `public vk::AccelerationStructureKHR `[`as`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a310d8142a50e671e29b626943fa2b6c8) 

The Vulkan acceleration structure.

#### `public vk::DeviceMemory `[`memory`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a67c749f7c097fac7a114c947b88752c0) 

The acceleration structure's memory.

#### `public inline void `[`destroy`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a8e189940953cc7155465cf65a2f1d4f4)`()` 

Used to destroy the acceleration structure and free its memory.

# struct `RAYEX_NAMESPACE::Tlas` 

A wrapper for a top level acceleration structure.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`AccelerationStructure`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure)` `[`as`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_tlas_1aef65925b7d115143b5e5b3b61a048566) | The [RAYEX_NAMESPACE::AccelerationStructure](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) object containing the Vulkan acceleration structure.
`public vk::BuildAccelerationStructureFlagsKHR `[`flags`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_tlas_1a45fd150956fba0341a86f8fa4e946d49) | The top level acceleration structure's build flags.

## Members

#### `public `[`AccelerationStructure`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure)` `[`as`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_tlas_1aef65925b7d115143b5e5b3b61a048566) 

The [RAYEX_NAMESPACE::AccelerationStructure](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) object containing the Vulkan acceleration structure.

#### `public vk::BuildAccelerationStructureFlagsKHR `[`flags`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_tlas_1a45fd150956fba0341a86f8fa4e946d49) 

The top level acceleration structure's build flags.

# struct `RAYEX_NAMESPACE::Blas` 

A wrapper for a bottom level acceleration structure.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`AccelerationStructure`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure)` `[`as`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1afeefd899332ab00ffc38a7641424de2d) | The [RAYEX_NAMESPACE::AccelerationStructure](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) object containing the Vulkan acceleration structure.
`public vk::BuildAccelerationStructureFlagsKHR `[`flags`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1a5735232656e5d5b3e3284475efab22b9) | The top level acceleration structure's build flags.
`public std::vector< vk::AccelerationStructureCreateGeometryTypeInfoKHR > `[`asCreateGeometryInfo`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1a0cefbce2e06e7f94be5201129d2c9507) | Specifies the shape of geometries that will be built into an acceleration structure.
`public std::vector< vk::AccelerationStructureGeometryKHR > `[`asGeometry`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1a7a270840d9f4e6b141a6f142abda68c5) | Data used to build acceleration structure geometry.
`public std::vector< vk::AccelerationStructureBuildOffsetInfoKHR > `[`asBuildOffsetInfo`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1ae30215c68a91e95453fcdb608b89d550) | The offset between acceleration structures when building.

## Members

#### `public `[`AccelerationStructure`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure)` `[`as`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1afeefd899332ab00ffc38a7641424de2d) 

The [RAYEX_NAMESPACE::AccelerationStructure](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) object containing the Vulkan acceleration structure.

#### `public vk::BuildAccelerationStructureFlagsKHR `[`flags`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1a5735232656e5d5b3e3284475efab22b9) 

The top level acceleration structure's build flags.

#### `public std::vector< vk::AccelerationStructureCreateGeometryTypeInfoKHR > `[`asCreateGeometryInfo`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1a0cefbce2e06e7f94be5201129d2c9507) 

Specifies the shape of geometries that will be built into an acceleration structure.

#### `public std::vector< vk::AccelerationStructureGeometryKHR > `[`asGeometry`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1a7a270840d9f4e6b141a6f142abda68c5) 

Data used to build acceleration structure geometry.

#### `public std::vector< vk::AccelerationStructureBuildOffsetInfoKHR > `[`asBuildOffsetInfo`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1ae30215c68a91e95453fcdb608b89d550) 

The offset between acceleration structures when building.

# struct `RAYEX_NAMESPACE::BlasInstance` 

An instance of a bottom level acceleration structure.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint32_t `[`blasId`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a17e6f6e2576131a69f83902510f0e60d) | The index of the bottom level acceleration structure in blas_.
`public uint32_t `[`instanceId`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a1dd11e314a88b4daf48b5f5616d07c1c) | The instance index (gl_InstanceID).
`public uint32_t `[`hitGroupId`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1abc119b6598b884820ac07b7afa30d6be) | The hit group index in the shader binding table.
`public uint32_t `[`mask`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a080061a774fa9881372198e1bf13630f) | The visibility mask, will be AND-ed with the ray mask.
`public vk::GeometryInstanceFlagsKHR `[`flags`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1ac7125d67baa983ea7b466a16ec36c70b) | The geometry display options.
`public glm::mat4 `[`transform`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1af521894ff0383f740d859f6fba2b8b8b) | The world transform matrix of the bottom level acceleration structure instance.

## Members

#### `public uint32_t `[`blasId`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a17e6f6e2576131a69f83902510f0e60d) 

The index of the bottom level acceleration structure in blas_.

#### `public uint32_t `[`instanceId`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a1dd11e314a88b4daf48b5f5616d07c1c) 

The instance index (gl_InstanceID).

#### `public uint32_t `[`hitGroupId`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1abc119b6598b884820ac07b7afa30d6be) 

The hit group index in the shader binding table.

#### `public uint32_t `[`mask`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a080061a774fa9881372198e1bf13630f) 

The visibility mask, will be AND-ed with the ray mask.

#### `public vk::GeometryInstanceFlagsKHR `[`flags`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1ac7125d67baa983ea7b466a16ec36c70b) 

The geometry display options.

#### `public glm::mat4 `[`transform`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1af521894ff0383f740d859f6fba2b8b8b) 

The world transform matrix of the bottom level acceleration structure instance.

# struct `RAYEX_NAMESPACE::Vertex` 

Describes a vertex and its data.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::vec3 `[`pos`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_1a2939dfc92a6910299aba3f4425be1be8) | The vertex's position in 3D space.
`public glm::vec3 `[`normal`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_1a4ac28ad5e150039b65a80a67dddb3127) | The vertex's normal vector.
`public glm::vec3 `[`color`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_1a8fff4b1de2eb2f1029b85181edea46a1) | The vertex's color.
`public glm::vec2 `[`texCoord`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_1a76904c7ad13909f64ebaaa8a81f7e903) | The vertex's texture coordinate.
`public float `[`padding0`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_1a546f7f3a2fb476408b5bf8f559f1d00f) | [Vertex](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex) padding 0.

## Members

#### `public glm::vec3 `[`pos`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_1a2939dfc92a6910299aba3f4425be1be8) 

The vertex's position in 3D space.

#### `public glm::vec3 `[`normal`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_1a4ac28ad5e150039b65a80a67dddb3127) 

The vertex's normal vector.

#### `public glm::vec3 `[`color`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_1a8fff4b1de2eb2f1029b85181edea46a1) 

The vertex's color.

#### `public glm::vec2 `[`texCoord`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_1a76904c7ad13909f64ebaaa8a81f7e903) 

The vertex's texture coordinate.

#### `public float `[`padding0`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex_1a546f7f3a2fb476408b5bf8f559f1d00f) 

[Vertex](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex) padding 0.

