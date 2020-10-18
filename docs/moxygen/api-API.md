# group `API` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`RAYEXEC_NAMESPACE::Api`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_api) | Initializes and owns all Vulkan components and displays a picture on the screen.
`class `[`RAYEXEC_NAMESPACE::Bindings`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings) | A utility class for managing descriptor related resources.
`class `[`RAYEXEC_NAMESPACE::Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer) | A wrapper class for a Vulkan buffer.
`class `[`RAYEXEC_NAMESPACE::CommandBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer) | A wrapper class for Vulkan command buffers.
`class `[`RAYEXEC_NAMESPACE::IndexBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_index_buffer) | A specialised buffer for index data.
`class `[`RAYEXEC_NAMESPACE::UniformBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer) | A specialised buffer for uniforms.
`class `[`RAYEXEC_NAMESPACE::VertexBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer) | A specialised buffer for vertex data.
`class `[`RAYEXEC_NAMESPACE::Image`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image) | A wrapper class for a Vulkan image.
`class `[`RAYEXEC_NAMESPACE::Texture`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture) | A helper class for creating ready-to-use textures.
`class `[`RAYEXEC_NAMESPACE::Pipeline`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_pipeline) | A wrapper class for a Vulkan graphics pipeline.
`class `[`RAYEXEC_NAMESPACE::RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder) | Manages the building process of the acceleration structures.
`class `[`RAYEXEC_NAMESPACE::RenderPass`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_render_pass) | A wrapper class for a Vulkan render pass.
`class `[`RAYEXEC_NAMESPACE::Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface) | A wrapper class for a Vulkan surface.
`class `[`RAYEXEC_NAMESPACE::Swapchain`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain) | A wrapper class for a Vulkan swapchain.
`class `[`RAYEXEC_NAMESPACE::DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger) | A wrapper class for a Vulkan debug utility messenger.
`class `[`RAYEXEC_NAMESPACE::Gui`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui) | A class to create an ImGui-based GUI.
`struct `[`RAYEXEC_NAMESPACE::Descriptors`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_descriptors) | Encapsulates descriptor-related resources.
`struct `[`RAYEXEC_NAMESPACE::CameraUbo`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_ubo) | A uniform buffer object for camera data.
`struct `[`RAYEXEC_NAMESPACE::LightsUbo`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_lights_ubo) | A uniform buffer object for different light types.
`struct `[`RAYEXEC_NAMESPACE::AccelerationStructure`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) | A wrapper for a Vulkan acceleration Structure.
`struct `[`RAYEXEC_NAMESPACE::Tlas`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_tlas) | A wrapper for a top level acceleration structure.
`struct `[`RAYEXEC_NAMESPACE::Blas`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas) | A wrapper for a bottom level acceleration structure.
`struct `[`RAYEXEC_NAMESPACE::BlasInstance`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance) | An instance of a bottom level acceleration structure.
`struct `[`RAYEXEC_NAMESPACE::Vertex`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex) | Describes a vertex and its data.

# class `RAYEXEC_NAMESPACE::Api` 

Initializes and owns all Vulkan components and displays a picture on the screen.

All API components and resources are freed using scope-bound destruction. 

Because of scope-bound destruction it is required to pay close attention to the order of resource allocations done by the user.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public friend `[`RayExec`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_api_1af862c9b962c5a27ad96ec325a12c91ae) | 
`public  `[`Api`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_api_1a01833704b83c33dc619485119c8a9207)`() = default` | 
`public RX_API `[`~Api`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_api_1ad9139e70bcc023ea322b0d38ed975686)`()` | 

## Members

#### `public friend `[`RayExec`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_api_1af862c9b962c5a27ad96ec325a12c91ae) 

#### `public  `[`Api`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_api_1a01833704b83c33dc619485119c8a9207)`() = default` 

#### `public RX_API `[`~Api`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_api_1ad9139e70bcc023ea322b0d38ed975686)`()` 

# class `RAYEXEC_NAMESPACE::Bindings` 

A utility class for managing descriptor related resources.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void `[`add`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a2f10ef6e872fad7a4c306ecc95c7c1fd)`(uint32_t binding,vk::DescriptorType type,vk::ShaderStageFlags stage,uint32_t count,vk::DescriptorBindingFlags flags)` | Used to add a binding.
`public auto `[`initLayoutUnique`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a70b09f22390a7fe839cc1da021acc6b0)`()` | Used to initialize a unique descriptor set layout.
`public auto `[`initPoolUnique`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a6e21f0ead6c8278daeadd2c76d3b0810)`(uint32_t maxSets,vk::DescriptorPoolCreateFlags flags)` | Used to initialize a descriptor pool.
`public void `[`update`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1ad689b3d0de08c764d41bf2b3fd6159ac)`()` | Updates the descriptor set.
`public void `[`write`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a1c3d1c52ae75baff73aefd9647ec534a)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::WriteDescriptorSetAccelerationStructureKHR * pWriteDescriptorSetAccelerationStructureKHR)` | Used to create a descriptor write for an acceleration structure.
`public void `[`write`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a1148d777220928340e00f28d65815f28)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::DescriptorImageInfo * pImageInfo)` | Used to create a descriptor write for an image.
`public void `[`write`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a5ba54bd2185e5c90a750cb5b184f114a)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::DescriptorBufferInfo * pBufferInfo)` | Used to create a descriptor write for a buffer.
`public void `[`write`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a1f1704821f8a085346f2f0a9d9db2498)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,const std::vector< vk::DescriptorBufferInfo > & uniformBufferInfos)` | Used to create a descriptor write for uniform buffers.
`public void `[`writeArray`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a8240677bf172ca55509ea6474dbca1c3)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::DescriptorBufferInfo * pBufferInfo)` | Used to create an array of descriptor writes for buffers.
`public void `[`setPoolSizes`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1ab192b011847a8caaa75b9661302d455c)`(const std::vector< vk::DescriptorPoolSize > & poolSizes)` | Used to set pool sizes manually.
`public void `[`reset`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a09e932446ea500173160e77a157764ac)`()` | Resets all members.

## Members

#### `public void `[`add`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a2f10ef6e872fad7a4c306ecc95c7c1fd)`(uint32_t binding,vk::DescriptorType type,vk::ShaderStageFlags stage,uint32_t count,vk::DescriptorBindingFlags flags)` 

Used to add a binding.

#### Parameters
* `binding` The binding's index. 

* `type` The binding's descriptor type. 

* `stage` The binding's shader type. 

* `count` The amount of descriptors for this binding. 

* `flags` The binding's descriptor binding flags.

#### `public auto `[`initLayoutUnique`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a70b09f22390a7fe839cc1da021acc6b0)`()` 

Used to initialize a unique descriptor set layout.

#### Returns
Returns a descriptor set layout with a unqiue handle.

#### `public auto `[`initPoolUnique`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a6e21f0ead6c8278daeadd2c76d3b0810)`(uint32_t maxSets,vk::DescriptorPoolCreateFlags flags)` 

Used to initialize a descriptor pool.

#### Parameters
* `maxSets` The maximum amount of descriptor sets that can be allocated from the pool. 

* `flags` The pool's create flags. 

#### Returns
Returns a descriptor pool with a unique handle.

#### `public void `[`update`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1ad689b3d0de08c764d41bf2b3fd6159ac)`()` 

Updates the descriptor set.

There are no descriptor set handles required for this function.

#### `public void `[`write`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a1c3d1c52ae75baff73aefd9647ec534a)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::WriteDescriptorSetAccelerationStructureKHR * pWriteDescriptorSetAccelerationStructureKHR)` 

Used to create a descriptor write for an acceleration structure.

#### Parameters
* `sets` The descriptor set handles. 

* `binding` The binding's index. 

* `pWriteDescriptorSetAccelerationStructureKHR` A pointer to a acceleration structure descriptor write.

#### `public void `[`write`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a1148d777220928340e00f28d65815f28)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::DescriptorImageInfo * pImageInfo)` 

Used to create a descriptor write for an image.

#### Parameters
* `sets` The descriptor set handles. 

* `binding` The binding's index. 

* `pImageInfo` A pointer to an image descriptor write.

#### `public void `[`write`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a5ba54bd2185e5c90a750cb5b184f114a)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::DescriptorBufferInfo * pBufferInfo)` 

Used to create a descriptor write for a buffer.

#### Parameters
* `sets` The descriptor set handles. 

* `binding` The binding's index. 

* `pBufferInfo` A pointer to a buffer descriptor write.

#### `public void `[`write`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a1f1704821f8a085346f2f0a9d9db2498)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,const std::vector< vk::DescriptorBufferInfo > & uniformBufferInfos)` 

Used to create a descriptor write for uniform buffers.

#### Parameters
* `sets` The descriptor set handles. 

* `binding` The binding's index. 

* `uniformBufferInfos` The uniform buffer's descriptor buffer infos.

#### `public void `[`writeArray`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a8240677bf172ca55509ea6474dbca1c3)`(const std::vector< vk::DescriptorSet > & sets,uint32_t binding,vk::DescriptorBufferInfo * pBufferInfo)` 

Used to create an array of descriptor writes for buffers.

#### Parameters
* `sets` The descriptor set handles. 

* `binding` The binding's index. 

* `pBufferInfo` The pointer to the first element of an array of descriptor buffer infos.

#### `public void `[`setPoolSizes`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1ab192b011847a8caaa75b9661302d455c)`(const std::vector< vk::DescriptorPoolSize > & poolSizes)` 

Used to set pool sizes manually.

#### Parameters
* `poolSizes` The pool sizes to set. 

Must be set before initializing the descriptor pool.

#### `public void `[`reset`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings_1a09e932446ea500173160e77a157764ac)`()` 

Resets all members.

# class `RAYEXEC_NAMESPACE::Buffer` 

A wrapper class for a Vulkan buffer.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a4286db555c7beb872ad62e5ae78f5a92)`() = default` | 
`public  `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a0f821dd2fb7a528f5d370c9935abb95e)`(vk::DeviceSize size,vk::BufferUsageFlags usage,const std::vector< uint32_t > & queueFamilyIndices,vk::MemoryPropertyFlags memoryPropertyFlags,void * pNextMemory,bool initialize)` | #### Parameters
`public  `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a6f29bd9f8dfecb05a9a40c5af01d1474)`(const `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer)` & buffer)` | #### Parameters
`public  `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1af8b33d8383361d81c6a89b16bdcc642c)`(const `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a112376785376333464a39d84851627bd)`(const `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer)` &)` | 
`public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a86fa3c008ace2f907d454aeea95a5c35)`(const `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer)` &&) = delete` | 
`public virtual RX_API `[`~Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a75864b69895914855537bd47ffc767dc)`() = default` | 
`public void `[`copyToBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1ae9316e2a184d003b4c15102d8c73e8d3)`(const `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer)` & buffer) const` | Copies the content of this buffer to another [RAYEXEC_NAMESPACE::Buffer](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer).
`public RX_API void `[`copyToBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a7e9ccfcec6b210f77929ae6f8e015470)`(vk::Buffer buffer) const` | Copies the content of this buffer to another vk::Buffer.
`public void `[`copyToImage`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a530b23fb6ee6affece20f959e7d12847)`(`[`Image`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image)` & image) const` | Copies the content of this buffer to an image.
`public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a20b985e8be99e64c284acaf58e5b7646)`() const` | #### Returns
`public inline auto `[`getMemory`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a320d33b561ad0629b139e20cd235c102)`() const` | #### Returns
`public inline auto `[`getSize`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a4bb5fbab956fbcf4e362e5c2268f8e2f)`() const` | #### Returns
`public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a8281cef1826a7b0f970a636e7a24703c)`(vk::DeviceSize size,vk::BufferUsageFlags usage,const std::vector< uint32_t > & queueFamilyIndices,vk::MemoryPropertyFlags memoryPropertyFlags,void * pNextMemory)` | Creates the buffer and allocates memory for it.
`public template<>`  <br/>`inline void `[`fill`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a34447d9e04323e05f3434fc290d75934)`(const T * source,vk::DeviceSize offset)` | Used to fill the buffer with any kind of data.
`protected vk::UniqueBuffer `[`buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a3dae10b0e57aaeb528ad27336a45f46c) | The buffer object with a unique handle.
`protected vk::UniqueDeviceMemory `[`memory`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1ade00c91ade501a5f85f0f157dbc87f7e) | The buffer's memory with a unique handle.
`protected vk::DeviceSize `[`size`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a154a7abcbda22be170ac9cfc3076b532) | The buffer's size.

## Members

#### `public  `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a4286db555c7beb872ad62e5ae78f5a92)`() = default` 

#### `public  `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a0f821dd2fb7a528f5d370c9935abb95e)`(vk::DeviceSize size,vk::BufferUsageFlags usage,const std::vector< uint32_t > & queueFamilyIndices,vk::MemoryPropertyFlags memoryPropertyFlags,void * pNextMemory,bool initialize)` 

#### Parameters
* `size` The size of the buffer. 

* `usage` The buffer's usage flags. 

* `queueFamilyIndices` Specifies which queue family will access the buffer. 

* `memoryPropertyFlags` Flags for memory allocation. 

* `pNextMemory` Attachment to the memory's pNext chain. 

* `initialize` If true, the buffer object will be initialized right away without an additional call to [init()](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a8281cef1826a7b0f970a636e7a24703c).

#### `public  `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a6f29bd9f8dfecb05a9a40c5af01d1474)`(const `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer)` & buffer)` 

#### Parameters
* `buffer` The target for the copy operation.

#### `public  `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1af8b33d8383361d81c6a89b16bdcc642c)`(const `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a112376785376333464a39d84851627bd)`(const `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer)` &)` 

#### `public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a86fa3c008ace2f907d454aeea95a5c35)`(const `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer)` &&) = delete` 

#### `public virtual RX_API `[`~Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a75864b69895914855537bd47ffc767dc)`() = default` 

#### `public void `[`copyToBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1ae9316e2a184d003b4c15102d8c73e8d3)`(const `[`Buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer)` & buffer) const` 

Copies the content of this buffer to another [RAYEXEC_NAMESPACE::Buffer](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer).

#### Parameters
* `buffer` The target for the copy operation.

#### `public RX_API void `[`copyToBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a7e9ccfcec6b210f77929ae6f8e015470)`(vk::Buffer buffer) const` 

Copies the content of this buffer to another vk::Buffer.

#### Parameters
* `buffer` The target for the copy operation.

#### `public void `[`copyToImage`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a530b23fb6ee6affece20f959e7d12847)`(`[`Image`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image)` & image) const` 

Copies the content of this buffer to an image.

#### Parameters
* `image` The target for the copy operation.

#### `public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a20b985e8be99e64c284acaf58e5b7646)`() const` 

#### Returns
Returns the buffer without the unique handle.

#### `public inline auto `[`getMemory`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a320d33b561ad0629b139e20cd235c102)`() const` 

#### Returns
Returns the buffer's memory without the unique handle.

#### `public inline auto `[`getSize`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a4bb5fbab956fbcf4e362e5c2268f8e2f)`() const` 

#### Returns
Returns the size of the buffer.

#### `public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a8281cef1826a7b0f970a636e7a24703c)`(vk::DeviceSize size,vk::BufferUsageFlags usage,const std::vector< uint32_t > & queueFamilyIndices,vk::MemoryPropertyFlags memoryPropertyFlags,void * pNextMemory)` 

Creates the buffer and allocates memory for it.

#### Parameters
* `size` The size of the buffer. 

* `usage` The buffer's usage flags. 

* `queueFamilyIndices` Specifies which queue family will access the buffer. 

* `memoryPropertyFlags` Flags for memory allocation. 

* `pNextMemory` Attachment to the memory's pNext chain.

#### `public template<>`  <br/>`inline void `[`fill`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a34447d9e04323e05f3434fc290d75934)`(const T * source,vk::DeviceSize offset)` 

Used to fill the buffer with any kind of data.

#### Parameters
* `source` The data to fill the buffer with. 

* `offset` The data's offset within the buffer.

#### `protected vk::UniqueBuffer `[`buffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a3dae10b0e57aaeb528ad27336a45f46c) 

The buffer object with a unique handle.

#### `protected vk::UniqueDeviceMemory `[`memory`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1ade00c91ade501a5f85f0f157dbc87f7e) 

The buffer's memory with a unique handle.

#### `protected vk::DeviceSize `[`size`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_buffer_1a154a7abcbda22be170ac9cfc3076b532) 

The buffer's size.

# class `RAYEXEC_NAMESPACE::CommandBuffer` 

A wrapper class for Vulkan command buffers.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`CommandBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a07aa8999ca5a653164ae9ce0a2b89bcb)`() = default` | 
`public  `[`CommandBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a81227115f7e6c2863d51c36834296d52)`(vk::CommandPool commandPool,uint32_t count,vk::CommandBufferUsageFlags usageFlags)` | Creates the command buffers and calls initializes them right away.
`public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a5e0bc2b488f0abb775d07e013b968074)`(vk::CommandPool commandPool,uint32_t count,vk::CommandBufferUsageFlags usageFlags)` | Creates the command buffers.
`public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1abcc9ff6272aba8b4a1afdf0602bcac4b)`() const` | #### Returns
`public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a4670aafed45e4cbe1066c785cec7a1ce)`(size_t index) const` | Returns the command buffer by some index.
`public void `[`free`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a8fff269edab25efc19a3ed181107c896)`()` | Frees the command buffer.
`public RX_API void `[`reset`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a016d2435cc62af6bcf4160bd541baf56)`()` | Resets the command buffer.
`public void `[`begin`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a3c7b641da4afce0c7df9a033c9a47d9a)`(size_t index)` | Used to begin the command buffer recording.
`public void `[`end`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a82d40d7bb295b161b68f15a08a20fa3a)`(size_t index)` | Used to stop the command buffer recording.
`public void `[`submitToQueue`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1acff16caf8b3bbe2ee906072fd5ef7462)`(vk::Queue queue,const std::vector< vk::Semaphore > & waitSemaphores,const std::vector< vk::Semaphore > & signalSemaphores,vk::PipelineStageFlags * waitDstStageMask)` | Submits the recorded commands to a queue.

## Members

#### `public  `[`CommandBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a07aa8999ca5a653164ae9ce0a2b89bcb)`() = default` 

#### `public  `[`CommandBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a81227115f7e6c2863d51c36834296d52)`(vk::CommandPool commandPool,uint32_t count,vk::CommandBufferUsageFlags usageFlags)` 

Creates the command buffers and calls initializes them right away.

#### Parameters
* `commandPool` The command pool from which the command buffers will be allocated from. 

* `count` The amount of Vulkan command buffers to initialize (the same as the amount of images in the swapchain). 

* `usageFlags` Specifies what the buffer will be used for.

#### `public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a5e0bc2b488f0abb775d07e013b968074)`(vk::CommandPool commandPool,uint32_t count,vk::CommandBufferUsageFlags usageFlags)` 

Creates the command buffers.

#### Parameters
* `commandPool` The command pool from which the command buffers will be allocated from. 

* `count` The amount of Vulkan command buffers to initialize (the same as the amount of images in the swapchain). 

* `usageFlags` Specifies what the buffer will be used for.

#### `public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1abcc9ff6272aba8b4a1afdf0602bcac4b)`() const` 

#### Returns
Returns the vector of command buffers.

#### `public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a4670aafed45e4cbe1066c785cec7a1ce)`(size_t index) const` 

Returns the command buffer by some index.

#### Parameters
* `index` The index of the desired command buffer. 

#### Returns
Returns the command buffer.

#### `public void `[`free`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a8fff269edab25efc19a3ed181107c896)`()` 

Frees the command buffer.

#### `public RX_API void `[`reset`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a016d2435cc62af6bcf4160bd541baf56)`()` 

Resets the command buffer.

#### `public void `[`begin`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a3c7b641da4afce0c7df9a033c9a47d9a)`(size_t index)` 

Used to begin the command buffer recording.

#### Parameters
* `index` An index to a command buffer to record to.

#### `public void `[`end`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a82d40d7bb295b161b68f15a08a20fa3a)`(size_t index)` 

Used to stop the command buffer recording.

#### Parameters
* `index` An index to a command buffer to stop recording.

#### `public void `[`submitToQueue`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1acff16caf8b3bbe2ee906072fd5ef7462)`(vk::Queue queue,const std::vector< vk::Semaphore > & waitSemaphores,const std::vector< vk::Semaphore > & signalSemaphores,vk::PipelineStageFlags * waitDstStageMask)` 

Submits the recorded commands to a queue.

#### Parameters
* `queue` The queue to submit to. 

* `waitSemaphores` A vector of semaphores to wait for. 

* `signalSemaphores` A vector of semaphores to signal. 

* `waitDstStageMask` The pipeline stage where the commands will be executed.

# class `RAYEXEC_NAMESPACE::IndexBuffer` 

```
class RAYEXEC_NAMESPACE::IndexBuffer
  : public RAYEXEC_NAMESPACE::Buffer
```  

A specialised buffer for index data.

If not templatized then uint32_t will be used as the data's type.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline auto `[`getCount`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_index_buffer_1a986b966f8583271d0a7974a66ab6eca5)`() const` | #### Returns
`public inline void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_index_buffer_1a8f13a9c4f07b3592bb8791a0bb55691f)`(const std::vector< uint32_t > & indices)` | Creates the buffer, allocates memory for it and fills it with the provided data.

## Members

#### `public inline auto `[`getCount`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_index_buffer_1a986b966f8583271d0a7974a66ab6eca5)`() const` 

#### Returns
Returns the amount of indices in the buffer.

#### `public inline void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_index_buffer_1a8f13a9c4f07b3592bb8791a0bb55691f)`(const std::vector< uint32_t > & indices)` 

Creates the buffer, allocates memory for it and fills it with the provided data.

To optimize the procedure a temporary staging buffer will be created. 
#### Parameters
* `indices` The index data.

# class `RAYEXEC_NAMESPACE::UniformBuffer` 

A specialised buffer for uniforms.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< vk::DescriptorBufferInfo > `[`bufferInfos`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a9a186941817e7a9f8b2c0c82ed07051e) | 
`public  `[`UniformBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a77802f4e17e5282d91c57377de41a349)`() = default` | 
`public template<>`  <br/>`inline  `[`UniformBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a3d8a6b197212e4e0f5c914dab88645a1)`(size_t swapchainImagesCount,bool initialize)` | #### Parameters
`public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1adb2e28fe37c2e943e0911d9f63a77040)`() const` | #### Returns
`public RX_API auto `[`getRaw`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a426b9cbe9b74a83bd49503cc70577f24)`() const` | #### Returns
`public template<>`  <br/>`inline void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1adedc9d69558242b95599b8ffe7f8ff80)`()` | Creates the uniform buffer and allocates memory for it.
`public template<>`  <br/>`inline void `[`upload`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a93afba2d64a22f7b764836b6e88217bc)`(uint32_t imageIndex,T & ubo)` | Used to fill an image's buffer.

## Members

#### `public std::vector< vk::DescriptorBufferInfo > `[`bufferInfos`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a9a186941817e7a9f8b2c0c82ed07051e) 

#### `public  `[`UniformBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a77802f4e17e5282d91c57377de41a349)`() = default` 

#### `public template<>`  <br/>`inline  `[`UniformBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a3d8a6b197212e4e0f5c914dab88645a1)`(size_t swapchainImagesCount,bool initialize)` 

#### Parameters
* `swapchainImagesCount` The amount of images in the swapchain. 

* `initialize` If true, the uniform buffer will be initialized right away without an additional call to [init()](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1adedc9d69558242b95599b8ffe7f8ff80).

#### `public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1adb2e28fe37c2e943e0911d9f63a77040)`() const` 

#### Returns
Returns the vector of uniform buffers.

#### `public RX_API auto `[`getRaw`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a426b9cbe9b74a83bd49503cc70577f24)`() const` 

#### Returns
Returns the vector of uniform buffers as raw Vulkan buffer objects.

#### `public template<>`  <br/>`inline void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1adedc9d69558242b95599b8ffe7f8ff80)`()` 

Creates the uniform buffer and allocates memory for it.

The function will create as many uniform buffers as there are images in the swapchain. Additionally, it will create the descriptor buffer infos which can be later used to write to a descriptor set. 
#### Parameters
* `swapchainImagesCount` The amount of images in the swapchain.

#### `public template<>`  <br/>`inline void `[`upload`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_uniform_buffer_1a93afba2d64a22f7b764836b6e88217bc)`(uint32_t imageIndex,T & ubo)` 

Used to fill an image's buffer.

#### Parameters
* `imageIndex` The image's index in the swapchain. 

* `ubo` The actual uniform buffer object holding the data.

# class `RAYEXEC_NAMESPACE::VertexBuffer` 

```
class RAYEXEC_NAMESPACE::VertexBuffer
  : public RAYEXEC_NAMESPACE::Buffer
```  

A specialised buffer for vertex data.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`VertexBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1a2185d29248059de141e7664740cfcb15)`() = default` | 
`public  `[`VertexBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1abc05422a3077de9b989e838cd936aec9)`(std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex)` > & vertices,bool initialize)` | #### Parameters
`public inline auto `[`getCount`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1a63f5142cb1832654a1acebf799b30c8c)`() const` | #### Returns
`public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1a613e5701e6cb36b44eaf3b1b59840848)`(const std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex)` > & vertices)` | Creates the buffer, allocates memory for it and fills it with provided data.

## Members

#### `public  `[`VertexBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1a2185d29248059de141e7664740cfcb15)`() = default` 

#### `public  `[`VertexBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1abc05422a3077de9b989e838cd936aec9)`(std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex)` > & vertices,bool initialize)` 

#### Parameters
* `vertices` The vertex data. 

* `initialize` If true, the vertex buffer will be initialized right away without an additional call to [init()](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1a613e5701e6cb36b44eaf3b1b59840848).

#### `public inline auto `[`getCount`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1a63f5142cb1832654a1acebf799b30c8c)`() const` 

#### Returns
Returns the amount of vertices in the buffer.

#### `public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer_1a613e5701e6cb36b44eaf3b1b59840848)`(const std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex)` > & vertices)` 

Creates the buffer, allocates memory for it and fills it with provided data.

#### Parameters
* `vertices` The vertex data.

# class `RAYEXEC_NAMESPACE::Image` 

A wrapper class for a Vulkan image.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a80646e43d35c53446fe39ba4bbf6485a)`() const` | #### Returns
`public inline auto `[`getExtent`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a7db97a4970afbd02a50ddf33786755df)`() const` | #### Returns
`public inline auto `[`getFormat`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1ac822adab25205cb634f1c8b90b94c49f)`() const` | #### Returns
`public inline auto `[`getLayout`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1aca01fc95ad936d74b2c6089e856d7527)`() const` | #### Returns
`public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a4f698705d33948624201e5a48c19c903)`(const vk::ImageCreateInfo & createInfo)` | Creates the image and allocates memory for it.
`public void `[`transitionToLayout`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a7a54cd7bf86b60f27ae621c226d24d87)`(vk::ImageLayout layout)` | Used to transition this image's layout.
`public void `[`transitionToLayout`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1ae5dfddf79b629d661e5ccca8a5bb9abf)`(vk::ImageLayout layout,vk::CommandBuffer commandBuffer)` | Used to transition this image's layout.
`protected vk::UniqueImage `[`image`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1ae5f8fbb64f25c043b8f87aa16701e015) | The Vulkan image with a unique handle.
`protected vk::UniqueDeviceMemory `[`memory`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a55e5d566de8b86669d56baa3d2bb77a3) | The image's memory with a unique handle.
`protected vk::Extent3D `[`extent`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a7f22222c0ef6a2ee859bb18318cc23e1) | The image's extent.
`protected vk::Format `[`format`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a432098b41a183cd99a815438eb915973) | The image's format.
`protected vk::ImageLayout `[`layout`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a81b85b202ca5a475953cfc076ac5958a) | THe image's layout.

## Members

#### `public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a80646e43d35c53446fe39ba4bbf6485a)`() const` 

#### Returns
Returns the Vulkan image object without the unique handle.

#### `public inline auto `[`getExtent`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a7db97a4970afbd02a50ddf33786755df)`() const` 

#### Returns
Returns the iamge's extent.

#### `public inline auto `[`getFormat`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1ac822adab25205cb634f1c8b90b94c49f)`() const` 

#### Returns
Returns the image's format.

#### `public inline auto `[`getLayout`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1aca01fc95ad936d74b2c6089e856d7527)`() const` 

#### Returns
Returns the image's layout.

#### `public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a4f698705d33948624201e5a48c19c903)`(const vk::ImageCreateInfo & createInfo)` 

Creates the image and allocates memory for it.

#### Parameters
* `createInfo` The Vulkan image create info.

#### `public void `[`transitionToLayout`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a7a54cd7bf86b60f27ae621c226d24d87)`(vk::ImageLayout layout)` 

Used to transition this image's layout.

#### Parameters
* `layout` The target layout. 

This function creates its own single-time usage command buffer.

#### `public void `[`transitionToLayout`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1ae5dfddf79b629d661e5ccca8a5bb9abf)`(vk::ImageLayout layout,vk::CommandBuffer commandBuffer)` 

Used to transition this image's layout.

#### Parameters
* `layout` The target layout 

* `commandBuffer` The command buffer that will be used to set up a pipeline barrier. 

It is required to call vk::CommandBuffer::begin() or [RAYEXEC_NAMESPACE::CommandBuffer::begin()](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a3c7b641da4afce0c7df9a033c9a47d9a) before calling this function.

#### `protected vk::UniqueImage `[`image`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1ae5f8fbb64f25c043b8f87aa16701e015) 

The Vulkan image with a unique handle.

#### `protected vk::UniqueDeviceMemory `[`memory`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a55e5d566de8b86669d56baa3d2bb77a3) 

The image's memory with a unique handle.

#### `protected vk::Extent3D `[`extent`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a7f22222c0ef6a2ee859bb18318cc23e1) 

The image's extent.

#### `protected vk::Format `[`format`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a432098b41a183cd99a815438eb915973) 

The image's format.

#### `protected vk::ImageLayout `[`layout`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_image_1a81b85b202ca5a475953cfc076ac5958a) 

THe image's layout.

# class `RAYEXEC_NAMESPACE::Texture` 

```
class RAYEXEC_NAMESPACE::Texture
  : public RAYEXEC_NAMESPACE::Image
```  

A helper class for creating ready-to-use textures.

> Todo: textureCounter is being incremented always, even if it is not a new texture. This class should actually look inside Api::textures or sth to assign the correct index.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint32_t `[`offset`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1a5735ddd31d42045b4da9c1574d12d8a3) | 
`public RX_API `[`Texture`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1aebef820f5f3548dec6218d6f4270161b)`()` | 
`public RX_API `[`Texture`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1a72e817dd85e94d49bd68a62c7d387863)`(std::string_view path,bool initialize)` | #### Parameters
`public inline auto `[`getImageView`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1aea60c6b434bf310053a68736d0b4ed98)`()` | #### Returns
`public inline auto `[`getSampler`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1aa6374a028d7fc389b585b655f061cc74)`()` | #### Returns
`public inline auto `[`getPath`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1a20097c4db17130f19e2c0ecd326ca1f9)`() const` | #### Returns
`public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1ada93dab9c8e12bfea2736794b12c81dd)`(std::string_view path)` | Creates the texture.

## Members

#### `public uint32_t `[`offset`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1a5735ddd31d42045b4da9c1574d12d8a3) 

#### `public RX_API `[`Texture`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1aebef820f5f3548dec6218d6f4270161b)`()` 

#### `public RX_API `[`Texture`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1a72e817dd85e94d49bd68a62c7d387863)`(std::string_view path,bool initialize)` 

#### Parameters
* `path` The relative path to the texture file. 

* `initialize` If true, the texture will be initialized right away without an additional call to [init()](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1ada93dab9c8e12bfea2736794b12c81dd).

#### `public inline auto `[`getImageView`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1aea60c6b434bf310053a68736d0b4ed98)`()` 

#### Returns
Returns the texture's image view.

#### `public inline auto `[`getSampler`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1aa6374a028d7fc389b585b655f061cc74)`()` 

#### Returns
Returns the texture's sampler.

#### `public inline auto `[`getPath`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1a20097c4db17130f19e2c0ecd326ca1f9)`() const` 

#### Returns
Returns the relative path of the texture file.

#### `public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_texture_1ada93dab9c8e12bfea2736794b12c81dd)`(std::string_view path)` 

Creates the texture.

#### Parameters
* `path` The relative path to the texture file. 

> Todo: Latest changes to this class might be causing errors once texture are working again.

# class `RAYEXEC_NAMESPACE::Pipeline` 

A wrapper class for a Vulkan graphics pipeline.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_pipeline_1ad48c27749177fa7c015d7cdaa61ab2c7)`(const std::vector< vk::DescriptorSetLayout > & descriptorSetLayouts,vk::RenderPass renderPass,vk::Viewport viewport,vk::Rect2D scissor,const `[`Settings`](moxygen/api-undefined.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_settings)` * settings)` | Initializes a rasterization pipeline.
`public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_pipeline_1af46ac0be1d9a01d93abfda781cfc8e55)`(const std::vector< vk::DescriptorSetLayout > & descriptorSetLayouts,const `[`Settings`](moxygen/api-undefined.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_settings)` * settings)` | Initializes a ray tracing pipeline.
`public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_pipeline_1aa366e6212bdf0293260cd0c00909331b)`() const` | #### Returns
`public inline auto `[`getLayout`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_pipeline_1ae5f69040045769e57d7d7ab8d1f91967)`() const` | #### Returns

## Members

#### `public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_pipeline_1ad48c27749177fa7c015d7cdaa61ab2c7)`(const std::vector< vk::DescriptorSetLayout > & descriptorSetLayouts,vk::RenderPass renderPass,vk::Viewport viewport,vk::Rect2D scissor,const `[`Settings`](moxygen/api-undefined.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_settings)` * settings)` 

Initializes a rasterization pipeline.

#### Parameters
* `descriptorSetLayouts` A vector of descriptor set layouts that will be included in the pipeline layout. 

* `renderPass` A Vulkan render pass. 

* `viewport` The desired view port. 

* `scissor` The desired scissor dimensions. 

* `settings` Used to retrieve information like maximum instances or lights of a specific type.

#### `public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_pipeline_1af46ac0be1d9a01d93abfda781cfc8e55)`(const std::vector< vk::DescriptorSetLayout > & descriptorSetLayouts,const `[`Settings`](moxygen/api-undefined.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_settings)` * settings)` 

Initializes a ray tracing pipeline.

#### Parameters
* `descriptorSetLayouts` A vector of descriptor set layouts that will be included in the pipeline layout. 

* `settings` Used to retrieve information like maximum instances or lights of a specific type.

#### `public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_pipeline_1aa366e6212bdf0293260cd0c00909331b)`() const` 

#### Returns
Returns the Vulkan pipeline object without the unique handle.

#### `public inline auto `[`getLayout`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_pipeline_1ae5f69040045769e57d7d7ab8d1f91967)`() const` 

#### Returns
Returns the Vulkan pipeline layout object without the unique handle.

# class `RAYEXEC_NAMESPACE::RayTracingBuilder` 

Manages the building process of the acceleration structures.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a09f8d1d7605a67657ac7e25aa9a3412e)`() = default` | 
`public RX_API `[`~RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a2a93b0638b8cd198b90d5fb2f20626fb)`()` | Calls [destroy()](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1abb6d7ea7882129e639bb14aacdf30294).
`public  `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1ae5f98a8ab4b95cb24ad4c4dd898ac8a9)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &) = delete` | 
`public  `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1aa9828b53ca4a0797b2b7fb2ec37e279b)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a41652e5d15ed47e75a387f7450f26681)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a4c3cc3563b344d436100bd83f8bd5a3f)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &&) = delete` | 
`public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1aef2570764e775f8dd07c4255a47ccdab)`()` | Retrieves the physical device's ray tracing capabilities.
`public void `[`destroy`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1abb6d7ea7882129e639bb14aacdf30294)`()` | Destroys all bottom and top level acceleration structures.
`public inline auto `[`getTlas`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a390ccddaf2bd873fe65c438319159bf6)`() const` | #### Returns
`public inline auto `[`getRtProperties`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a99c6fb5a011da041275f54013155bc9c)`() const` | #### Returns
`public inline auto `[`getStorageImageView`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a2511786430f0bf8134a447123d421c63)`() const` | #### Returns
`public auto `[`modelToBlas`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1aed5bcefa14705b34f910e3ceb37fc8a5)`(const `[`VertexBuffer`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer)` & vertexBuffer,const `[`IndexBuffer`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_index_buffer)` & indexBuffer) const` | Used to convert wavefront models to a bottom level acceleration structure.
`public auto `[`instanceToVkGeometryInstanceKHR`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a07f5808d1c137c8104b5bc5c213b8d2c)`(const `[`BlasInstance`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance)` & instance)` | Used to convert a bottom level acceleration structure instance to a Vulkan geometry instance.
`public void `[`createBottomLevelAS`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1af229b339fa26bbedc4b06a74bd3e2a4c)`(const std::vector< `[`VertexBuffer`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer)` > & vertexBuffers,const std::vector< `[`IndexBuffer`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_index_buffer)` > & indexBuffers)` | Used to prepare building the bottom level acceleration structures.
`public void `[`buildBlas`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a7317a1937164164619a0f0d1fdc0a9b1)`(const std::vector< `[`Blas`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas)` > & blas_,vk::BuildAccelerationStructureFlagsKHR flags)` | Builds all bottom level acceleration structures.
`public void `[`createTopLevelAS`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1aeabc1211e7bf59034ee75c61c53bbaf1)`(const std::vector< std::shared_ptr< `[`GeometryInstance`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` >> & geometryInstances)` | Used to prepare building the top level acceleration structure.
`public void `[`buildTlas`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a0802140271c08f495e8c2732937196fd)`(const std::vector< `[`BlasInstance`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance)` > & instances,vk::BuildAccelerationStructureFlagsKHR flags)` | Build the top level acceleration structure.
`public void `[`createStorageImage`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a2aff232d2fc78728d364c9eba2bcafa0)`(vk::Extent2D swapchainExtent)` | Creates the storage image which the ray tracing shaders will write to.
`public void `[`createShaderBindingTable`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a14ae945c468cafeba09183f329c596e1)`(vk::Pipeline rtPipeline)` | Creates the shader binding tables.
`public void `[`rayTrace`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a311cafb7c359cc969a32d8acea1dd08d)`(vk::CommandBuffer swapchaincommandBuffer,vk::Image swapchainImage,vk::Extent2D extent)` | Used to record the actual ray tracing commands to a given command buffer.

## Members

#### `public  `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a09f8d1d7605a67657ac7e25aa9a3412e)`() = default` 

#### `public RX_API `[`~RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a2a93b0638b8cd198b90d5fb2f20626fb)`()` 

Calls [destroy()](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1abb6d7ea7882129e639bb14aacdf30294).

#### `public  `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1ae5f98a8ab4b95cb24ad4c4dd898ac8a9)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &) = delete` 

#### `public  `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1aa9828b53ca4a0797b2b7fb2ec37e279b)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a41652e5d15ed47e75a387f7450f26681)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a4c3cc3563b344d436100bd83f8bd5a3f)`(const `[`RayTracingBuilder`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder)` &&) = delete` 

#### `public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1aef2570764e775f8dd07c4255a47ccdab)`()` 

Retrieves the physical device's ray tracing capabilities.

#### `public void `[`destroy`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1abb6d7ea7882129e639bb14aacdf30294)`()` 

Destroys all bottom and top level acceleration structures.

#### `public inline auto `[`getTlas`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a390ccddaf2bd873fe65c438319159bf6)`() const` 

#### Returns
Returns the top level acceleration structure.

#### `public inline auto `[`getRtProperties`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a99c6fb5a011da041275f54013155bc9c)`() const` 

#### Returns
Returns the physical device's ray tracing properties.

#### `public inline auto `[`getStorageImageView`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a2511786430f0bf8134a447123d421c63)`() const` 

#### Returns
Returns the storage image's image view.

#### `public auto `[`modelToBlas`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1aed5bcefa14705b34f910e3ceb37fc8a5)`(const `[`VertexBuffer`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer)` & vertexBuffer,const `[`IndexBuffer`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_index_buffer)` & indexBuffer) const` 

Used to convert wavefront models to a bottom level acceleration structure.

#### Parameters
* `vertexBuffer` A vertex buffer of some geometry. 

* `indexBuffer` An index buffer of some geometry. 

#### Returns
Returns the bottom level acceleration structure.

#### `public auto `[`instanceToVkGeometryInstanceKHR`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a07f5808d1c137c8104b5bc5c213b8d2c)`(const `[`BlasInstance`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance)` & instance)` 

Used to convert a bottom level acceleration structure instance to a Vulkan geometry instance.

#### Parameters
* `instance` A bottom level acceleration structure instance. 

#### Returns
Returns the Vulkan geometry instance.

#### `public void `[`createBottomLevelAS`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1af229b339fa26bbedc4b06a74bd3e2a4c)`(const std::vector< `[`VertexBuffer`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_buffer)` > & vertexBuffers,const std::vector< `[`IndexBuffer`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_index_buffer)` > & indexBuffers)` 

Used to prepare building the bottom level acceleration structures.

#### Parameters
* `vertexBuffers` [Vertex](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex) buffers of all geometry in the scene. 

* `indexBuffers` Index buffers of all geometry in the scene.

#### `public void `[`buildBlas`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a7317a1937164164619a0f0d1fdc0a9b1)`(const std::vector< `[`Blas`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas)` > & blas_,vk::BuildAccelerationStructureFlagsKHR flags)` 

Builds all bottom level acceleration structures.

#### Parameters
* `blas_` A vector of [RAYEXEC_NAMESPACE::Blas](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas) objects containing all bottom level acceleration structures prepared in [createBottomLevelAS()](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1af229b339fa26bbedc4b06a74bd3e2a4c). 

* `flags` The build flags.

#### `public void `[`createTopLevelAS`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1aeabc1211e7bf59034ee75c61c53bbaf1)`(const std::vector< std::shared_ptr< `[`GeometryInstance`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` >> & geometryInstances)` 

Used to prepare building the top level acceleration structure.

#### Parameters
* `geometryInstances` All geometry instances in the scene.

#### `public void `[`buildTlas`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a0802140271c08f495e8c2732937196fd)`(const std::vector< `[`BlasInstance`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance)` > & instances,vk::BuildAccelerationStructureFlagsKHR flags)` 

Build the top level acceleration structure.

#### Parameters
* `instances` A vector of bottom level acceleration structure instances. 

* `flags` The build flags.

#### `public void `[`createStorageImage`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a2aff232d2fc78728d364c9eba2bcafa0)`(vk::Extent2D swapchainExtent)` 

Creates the storage image which the ray tracing shaders will write to.

#### Parameters
* `swapchainExtent` The swapchain images' extent.

#### `public void `[`createShaderBindingTable`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a14ae945c468cafeba09183f329c596e1)`(vk::Pipeline rtPipeline)` 

Creates the shader binding tables.

#### Parameters
* `rtPipeline` The ray tracing graphics pipeline.

#### `public void `[`rayTrace`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_tracing_builder_1a311cafb7c359cc969a32d8acea1dd08d)`(vk::CommandBuffer swapchaincommandBuffer,vk::Image swapchainImage,vk::Extent2D extent)` 

Used to record the actual ray tracing commands to a given command buffer.

#### Parameters
* `swapchaincommandBuffer` The command buffer to record to. 

* `swapchainImage` The current image in the swapchain. 

* `extent` The swapchain images' extent.

# class `RAYEXEC_NAMESPACE::RenderPass` 

A wrapper class for a Vulkan render pass.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_render_pass_1a8c2fe19756c46e3403500697be43bdc2)`() const` | #### Returns
`public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_render_pass_1afb29f77500194dc5e8299cb2cdce0afe)`(const std::vector< vk::AttachmentDescription > & attachments,const std::vector< vk::SubpassDescription > & subpasses,const std::vector< vk::SubpassDependency > & dependencies)` | Initializes the Vulkan render pass.
`public void `[`begin`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_render_pass_1a6161719c747189475df47f2e3dcf25aa)`(vk::Framebuffer framebuffer,vk::CommandBuffer commandBuffer,vk::Rect2D renderArea,const std::vector< vk::ClearValue > & clearValues) const` | Call to begin the render pass.
`public void `[`end`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_render_pass_1af0713427a6ebea6cb6c976cd7df69b07)`(vk::CommandBuffer commandBuffer) const` | Call to end the render pass.

## Members

#### `public inline auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_render_pass_1a8c2fe19756c46e3403500697be43bdc2)`() const` 

#### Returns
Returns the Vulkan render pass without the unique handle.

#### `public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_render_pass_1afb29f77500194dc5e8299cb2cdce0afe)`(const std::vector< vk::AttachmentDescription > & attachments,const std::vector< vk::SubpassDescription > & subpasses,const std::vector< vk::SubpassDependency > & dependencies)` 

Initializes the Vulkan render pass.

#### Parameters
* `attachments` The Vulkan attachment description. 

* `subpasses` The Vulkan subpass description. 

* `dependencies` The Vulkan subpass dependencies.

#### `public void `[`begin`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_render_pass_1a6161719c747189475df47f2e3dcf25aa)`(vk::Framebuffer framebuffer,vk::CommandBuffer commandBuffer,vk::Rect2D renderArea,const std::vector< vk::ClearValue > & clearValues) const` 

Call to begin the render pass.

#### Parameters
* `framebuffer` The swapchain framebuffer. 

* `commandBuffer` The command buffer used to begin the render pass. 

* `renderArea` Defines the size of the render area. 

* `clearValues` The clear values. 

[RAYEXEC_NAMESPACE::CommandBuffer::begin()](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_command_buffer_1a3c7b641da4afce0c7df9a033c9a47d9a) or vk::CommandBuffer::begin() must have been already called prior to calling this function.

#### `public void `[`end`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_render_pass_1af0713427a6ebea6cb6c976cd7df69b07)`(vk::CommandBuffer commandBuffer) const` 

Call to end the render pass.

#### Parameters
* `commandBuffer`

# class `RAYEXEC_NAMESPACE::Surface` 

A wrapper class for a Vulkan surface.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a2f6bf64e9d0db9bccc2953cf8ccee8ef)`() = default` | 
`public RX_API `[`~Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a66f6aa415154d2fcb8cb52c60b1ff50c)`()` | Calls destroy().
`public  `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1ae109c3463c11f0a8618eae399eafc7a9)`(const `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface)` &) = delete` | 
`public  `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a666de7dabd145c3e485fa2d68deb631c)`(const `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a4c3ad8eee8a52780e41dbad3a5bb7017)`(const `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface)` &) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a8ec0126e1a66560bad7c8c951e110c07)`(const `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface)` &&) = delete` | 
`public inline auto `[`getFormat`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a48a85968025216109364439c33867080)`() const` | #### Returns
`public inline auto `[`getColorSpace`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a86f07a17c40ddbde618da58505188cac)`() const` | #### Returns
`public inline auto `[`getPresentMode`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1af5c6d1a091849de2f27d50313c528663)`() const` | #### Returns
`public inline auto `[`getCapabilities`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a3e5cddc4a0865d6409f3cbc3ec445ad0)`() const` | #### Returns
`public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1aab66cf348fdbaf821130405764db7f97)`()` | Initializes the Vulkan surface object.
`public void `[`assessSettings`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a110994fc7096f6fd95e7b6aafee6ca1b)`()` | Checks if the preferred settings for format, color space and present mode are available.

## Members

#### `public  `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a2f6bf64e9d0db9bccc2953cf8ccee8ef)`() = default` 

#### `public RX_API `[`~Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a66f6aa415154d2fcb8cb52c60b1ff50c)`()` 

Calls destroy().

#### `public  `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1ae109c3463c11f0a8618eae399eafc7a9)`(const `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface)` &) = delete` 

#### `public  `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a666de7dabd145c3e485fa2d68deb631c)`(const `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a4c3ad8eee8a52780e41dbad3a5bb7017)`(const `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface)` &) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a8ec0126e1a66560bad7c8c951e110c07)`(const `[`Surface`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface)` &&) = delete` 

#### `public inline auto `[`getFormat`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a48a85968025216109364439c33867080)`() const` 

#### Returns
Returns the surface format.

#### `public inline auto `[`getColorSpace`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a86f07a17c40ddbde618da58505188cac)`() const` 

#### Returns
Returns the surface's color space.

#### `public inline auto `[`getPresentMode`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1af5c6d1a091849de2f27d50313c528663)`() const` 

#### Returns
Returns the surface's present mode.

#### `public inline auto `[`getCapabilities`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a3e5cddc4a0865d6409f3cbc3ec445ad0)`() const` 

#### Returns
Returns the surface's capabilities.

#### `public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1aab66cf348fdbaf821130405764db7f97)`()` 

Initializes the Vulkan surface object.

If any of the specified format, color space and present mode are not available the function will fall back to settings that are guaranteed to be supported.

#### `public void `[`assessSettings`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface_1a110994fc7096f6fd95e7b6aafee6ca1b)`()` 

Checks if the preferred settings for format, color space and present mode are available.

If not, the function will set them to some fallback values. Must be called right after the enumeration of the physical device.

# class `RAYEXEC_NAMESPACE::Swapchain` 

A wrapper class for a Vulkan swapchain.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a2d7ff0e063ea15fe578407c7c81ac7e2)`(`[`Surface`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface)` * surface,vk::RenderPass renderPass)` | Creates the swapchain, the swapchain images and their image views as well as their framebuffers.
`public void `[`destroy`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a562180ff12b730a57e7aec3c73a534b7)`()` | Destroys the swapchain.
`public inline auto `[`getFramebuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a7423d7474a69ddcf6c069dd52d03c9fe)`(uint32_t index) const` | #### Returns
`public inline auto `[`getCurrentImageIndex`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a5978067f74d9579eef8777cefed83a6c)`() const` | #### Returns
`public inline auto `[`getExtent`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1ac1a042535549e8b435a41bbcaca06759)`() const` | #### Returns
`public inline auto `[`getImageAspect`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a514beaf04e7d7835bf4b94ee0223622c)`() const` | #### Returns
`public inline auto `[`getImage`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a8711129b1c04a4c09abb1a65817ac5a4)`(size_t index) const` | Returns the swapchain image at a given index.
`public inline auto `[`getImages`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a6e0557f4d62e28dfeb9cf6e2afb06704)`() const` | #### Returns
`public inline auto `[`getImageViews`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a5e7ed9142f249264a2cb421c92c1b2d5)`() const` | #### Returns
`public void `[`setImageAspect`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1ae727ddbc80698faab04783451ad2a250)`(vk::ImageAspectFlags flags)` | Used to set the desired image aspect flags.
`public void `[`setImageLayout`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1afe470847ca79c36a8a502dabc6ea81e6)`(vk::ImageLayout oldLayout,vk::ImageLayout newLayout)` | Used to transition from one layout to another.
`public void `[`acquireNextImage`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a2077caebb153dac7c98ccf7ea270f6e6)`(vk::Semaphore semaphore,vk::Fence fence)` | Retrieves the next swapchain image.

## Members

#### `public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a2d7ff0e063ea15fe578407c7c81ac7e2)`(`[`Surface`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface)` * surface,vk::RenderPass renderPass)` 

Creates the swapchain, the swapchain images and their image views as well as their framebuffers.

#### Parameters
* `surface` A pointer to a [RAYEXEC_NAMESPACE::Surface](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface) object. 

* `renderPass` The render pass to create the framebuffers.

#### `public void `[`destroy`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a562180ff12b730a57e7aec3c73a534b7)`()` 

Destroys the swapchain.

#### `public inline auto `[`getFramebuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a7423d7474a69ddcf6c069dd52d03c9fe)`(uint32_t index) const` 

#### Returns
Returns the swapchain framebuffer at a given index.

#### `public inline auto `[`getCurrentImageIndex`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a5978067f74d9579eef8777cefed83a6c)`() const` 

#### Returns
Returns the current swapchain image index.

#### `public inline auto `[`getExtent`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1ac1a042535549e8b435a41bbcaca06759)`() const` 

#### Returns
Returns the swapchain images' extent.

#### `public inline auto `[`getImageAspect`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a514beaf04e7d7835bf4b94ee0223622c)`() const` 

#### Returns
Returns the swapchain images' image aspect.

#### `public inline auto `[`getImage`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a8711129b1c04a4c09abb1a65817ac5a4)`(size_t index) const` 

Returns the swapchain image at a given index.

#### Parameters
* `index` The index of the swapchain image. 

#### Returns
The swapchain image.

#### `public inline auto `[`getImages`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a6e0557f4d62e28dfeb9cf6e2afb06704)`() const` 

#### Returns
Returns a vector containing all swapchain images.

#### `public inline auto `[`getImageViews`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a5e7ed9142f249264a2cb421c92c1b2d5)`() const` 

#### Returns
Returns a vector containing all swapchain image views. 

> Todo: Returning by reference will result in size 0.

#### `public void `[`setImageAspect`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1ae727ddbc80698faab04783451ad2a250)`(vk::ImageAspectFlags flags)` 

Used to set the desired image aspect flags.

#### `public void `[`setImageLayout`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1afe470847ca79c36a8a502dabc6ea81e6)`(vk::ImageLayout oldLayout,vk::ImageLayout newLayout)` 

Used to transition from one layout to another.

#### Parameters
* `oldLayout` The swapchain images' current image layout. 

* `newLayout` The target image layout.

#### `public void `[`acquireNextImage`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_swapchain_1a2077caebb153dac7c98ccf7ea270f6e6)`(vk::Semaphore semaphore,vk::Fence fence)` 

Retrieves the next swapchain image.

#### Parameters
* `semaphore` A semaphore to signal. 

* `fence` A fence to signal.

# class `RAYEXEC_NAMESPACE::DebugMessenger` 

A wrapper class for a Vulkan debug utility messenger.

The class features scope-bound destruction.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a5939abe290f0346d7b1937801b524c3a)`() = default` | 
`public RX_API `[`~DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1ab89a75010513bf52e7434347b7402980)`()` | Calls destroy().
`public  `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1ad8cd7919dd10ef8575965f2a8a27c719)`(const `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &) = delete` | 
`public  `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a03aa90e8711e3c4c52914b1f3bef719f)`(const `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1adb7fd6bce2bc01d6bf175b2b940df9f0)`(const `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1ae4372ca978336a4d7a525535806e5d47)`(const `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &&) = delete` | 
`public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a4b24b4d3b73b59e11735a7dddf6a7db2)`(vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity,vk::DebugUtilsMessageTypeFlagsEXT messageType)` | Creates the debug messenger with the given properties.

## Members

#### `public  `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a5939abe290f0346d7b1937801b524c3a)`() = default` 

#### `public RX_API `[`~DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1ab89a75010513bf52e7434347b7402980)`()` 

Calls destroy().

#### `public  `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1ad8cd7919dd10ef8575965f2a8a27c719)`(const `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &) = delete` 

#### `public  `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a03aa90e8711e3c4c52914b1f3bef719f)`(const `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1adb7fd6bce2bc01d6bf175b2b940df9f0)`(const `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1ae4372ca978336a4d7a525535806e5d47)`(const `[`DebugMessenger`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger)` &&) = delete` 

#### `public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_debug_messenger_1a4b24b4d3b73b59e11735a7dddf6a7db2)`(vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity,vk::DebugUtilsMessageTypeFlagsEXT messageType)` 

Creates the debug messenger with the given properties.

#### Parameters
* `messageSeverity` - Specifies the type of severity of messages that will be logged. 

* `messageType` - Specifies the types of messages that will be logged.

# class `RAYEXEC_NAMESPACE::Gui` 

A class to create an ImGui-based GUI.

This class acts like an interface for the user to create their own GUI. It is possible to create multiple GUI objects and re-assign them to the renderer at runtime. 
```cpp
class CustomGui : public Gui
{
private:
  // Configure style and input of the GUI.
  void [configure](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1aeb8f2a8a40752cc3e8bac5a386ea8264)() override { }

  // Put the rendering commands in here.
  void [render](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a370773b570ac9e9cedf4532af673fed2)() override { }
};

RayExecmyRenderer;
myRenderer.init( );

// Put this line after the declaration of the RAYEXEC_NAMESPACE::RayExecobject.
auto myGui = std::make_shared<CustomGui>( );
myRenderer.setGui( myGui );
```

The GUI object must be declared after the renderer to ensure the correct order for destroying and freeing resources.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public virtual RX_API `[`~Gui`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a40a810587318be3d9857c3b3746eda86)`() = default` | 
`public inline auto `[`getCommandBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a42ee4d92c4f67cfffde59078d8dc2ea6)`(uint32_t index) const` | Returns the GUI's command buffer at the given index.
`public virtual RX_API void `[`configure`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1aeb8f2a8a40752cc3e8bac5a386ea8264)`()` | Used to configure all ImGui settings.
`public virtual RX_API void `[`render`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a370773b570ac9e9cedf4532af673fed2)`()` | This function is for calling the individual ImGui components, e.g.
`public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a53e3aef5201eb40b6253497ffa3007d9)`(const `[`Surface`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface)` * surface,vk::Extent2D swapchainImageExtent,const std::vector< vk::ImageView > & swapchainImageViews)` | Creates the GUI and all required Vulkan components.
`public void `[`recreate`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a3c007475218c665bf3cefc22e8aec63a)`(vk::Extent2D swapchainImageExtent,const std::vector< vk::ImageView > & swapchainImageViews)` | Used to recreate the GUI in case the window size was changed.
`public void `[`renderDrawData`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a786d4c8f378d6f025d8ca7caae8e66c4)`(uint32_t imageIndex)` | Records the ImGui rendering calls to the command buffer at the given image index.
`public RX_API void `[`destroy`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a18e47c4c4175e5ba4935c7cc6949b3df)`()` | Destroys all ImGui resources.

## Members

#### `public virtual RX_API `[`~Gui`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a40a810587318be3d9857c3b3746eda86)`() = default` 

#### `public inline auto `[`getCommandBuffer`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a42ee4d92c4f67cfffde59078d8dc2ea6)`(uint32_t index) const` 

Returns the GUI's command buffer at the given index.

#### Parameters
* `index` The index of the command buffer to access. 

#### Returns
Returns the Vulkan command buffer.

#### `public virtual RX_API void `[`configure`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1aeb8f2a8a40752cc3e8bac5a386ea8264)`()` 

Used to configure all ImGui settings.

The user should override this function if they want to change the style or various other configuration settings.

#### `public virtual RX_API void `[`render`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a370773b570ac9e9cedf4532af673fed2)`()` 

This function is for calling the individual ImGui components, e.g.

widgets.

The user should override this function to create their own GUI.

#### `public void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a53e3aef5201eb40b6253497ffa3007d9)`(const `[`Surface`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface)` * surface,vk::Extent2D swapchainImageExtent,const std::vector< vk::ImageView > & swapchainImageViews)` 

Creates the GUI and all required Vulkan components.

#### Parameters
* `surface` A pointer to a [RAYEXEC_NAMESPACE::Surface](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_surface) object. 

* `swapchainImageExtent` The extent of the swapchain images. 

* `swapchainImageViews` The swapchain images' image views.

#### `public void `[`recreate`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a3c007475218c665bf3cefc22e8aec63a)`(vk::Extent2D swapchainImageExtent,const std::vector< vk::ImageView > & swapchainImageViews)` 

Used to recreate the GUI in case the window size was changed.

#### Parameters
* `swapchainImageExtent` The extent of the swapchain images. 

* `swapchainImageViews` The swapchain images' image views.

#### `public void `[`renderDrawData`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a786d4c8f378d6f025d8ca7caae8e66c4)`(uint32_t imageIndex)` 

Records the ImGui rendering calls to the command buffer at the given image index.

#### Parameters
* `imageIndex` The index addressing a command buffer.

#### `public RX_API void `[`destroy`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui_1a18e47c4c4175e5ba4935c7cc6949b3df)`()` 

Destroys all ImGui resources.

# struct `RAYEXEC_NAMESPACE::Descriptors` 

Encapsulates descriptor-related resources.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public vk::UniqueDescriptorSetLayout `[`layout`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_descriptors_1a830c3a9d3cfa294032cfe09bf49a6a7c) | 
`public vk::UniqueDescriptorPool `[`pool`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_descriptors_1ae1b1a6510785f144cc6c656e7c8105a3) | 
`public `[`Bindings`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings)` `[`bindings`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_descriptors_1ab5c2f5b3b87adf21ce9107d4b83b95b2) | 

## Members

#### `public vk::UniqueDescriptorSetLayout `[`layout`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_descriptors_1a830c3a9d3cfa294032cfe09bf49a6a7c) 

#### `public vk::UniqueDescriptorPool `[`pool`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_descriptors_1ae1b1a6510785f144cc6c656e7c8105a3) 

#### `public `[`Bindings`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_bindings)` `[`bindings`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_descriptors_1ab5c2f5b3b87adf21ce9107d4b83b95b2) 

# struct `RAYEXEC_NAMESPACE::CameraUbo` 

A uniform buffer object for camera data.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::mat4 `[`view`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1ab06b480c6c0bb5e2245d47e836f7bfc6) | 
`public glm::mat4 `[`projection`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a962e6634f0be4aa481369cf52872ef32) | 
`public glm::mat4 `[`viewInverse`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a7607df5aea8ba3089d72a6369d1867c3) | 
`public glm::mat4 `[`projectionInverse`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a179b030928d0bff193c5c5ed42f505cd) | 
`public glm::vec3 `[`position`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1ae5078c482d2e67f80467327daacc7d39) | 

## Members

#### `public glm::mat4 `[`view`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1ab06b480c6c0bb5e2245d47e836f7bfc6) 

#### `public glm::mat4 `[`projection`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a962e6634f0be4aa481369cf52872ef32) 

#### `public glm::mat4 `[`viewInverse`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a7607df5aea8ba3089d72a6369d1867c3) 

#### `public glm::mat4 `[`projectionInverse`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1a179b030928d0bff193c5c5ed42f505cd) 

#### `public glm::vec3 `[`position`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_ubo_1ae5078c482d2e67f80467327daacc7d39) 

# struct `RAYEXEC_NAMESPACE::LightsUbo` 

A uniform buffer object for different light types.

> Todo: Shouldn't this also be a storage buffer?

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::array< `[`DirectionalLight`](moxygen/api-undefined.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_directional_light)`, 10 > `[`directionalLightNodes`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_lights_ubo_1a88f9fe50e74d99c4e0df93b677fbe31a) | 
`public std::array< `[`PointLight`](moxygen/api-undefined.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_point_light)`, 10 > `[`pointLightNodes`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_lights_ubo_1aec1ca66cc37fe542c2e05af7d777f953) | 

## Members

#### `public std::array< `[`DirectionalLight`](moxygen/api-undefined.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_directional_light)`, 10 > `[`directionalLightNodes`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_lights_ubo_1a88f9fe50e74d99c4e0df93b677fbe31a) 

#### `public std::array< `[`PointLight`](moxygen/api-undefined.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_point_light)`, 10 > `[`pointLightNodes`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_lights_ubo_1aec1ca66cc37fe542c2e05af7d777f953) 

# struct `RAYEXEC_NAMESPACE::AccelerationStructure` 

A wrapper for a Vulkan acceleration Structure.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public vk::AccelerationStructureKHR `[`as`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a284bf5b154e56146173e4fc66f8789f1) | The Vulkan acceleration structure.
`public vk::DeviceMemory `[`memory`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a5080bc9aac686561c16323ce886d1011) | The acceleration structure's memory.
`public inline void `[`destroy`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1aa7d536cc32d5f2c5e1c863ad0fc3e1ba)`()` | Used to destroy the acceleration structure and free its memory.

## Members

#### `public vk::AccelerationStructureKHR `[`as`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a284bf5b154e56146173e4fc66f8789f1) 

The Vulkan acceleration structure.

#### `public vk::DeviceMemory `[`memory`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a5080bc9aac686561c16323ce886d1011) 

The acceleration structure's memory.

#### `public inline void `[`destroy`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1aa7d536cc32d5f2c5e1c863ad0fc3e1ba)`()` 

Used to destroy the acceleration structure and free its memory.

# struct `RAYEXEC_NAMESPACE::Tlas` 

A wrapper for a top level acceleration structure.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`AccelerationStructure`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure)` `[`as`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_tlas_1aca8cc9aa6ef9f0f386d35a49c1f30126) | The [RAYEXEC_NAMESPACE::AccelerationStructure](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) object containing the Vulkan acceleration structure.
`public vk::BuildAccelerationStructureFlagsKHR `[`flags`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_tlas_1a7d90d5df478fabf257b377fcfbaeb666) | The top level acceleration structure's build flags.

## Members

#### `public `[`AccelerationStructure`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure)` `[`as`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_tlas_1aca8cc9aa6ef9f0f386d35a49c1f30126) 

The [RAYEXEC_NAMESPACE::AccelerationStructure](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) object containing the Vulkan acceleration structure.

#### `public vk::BuildAccelerationStructureFlagsKHR `[`flags`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_tlas_1a7d90d5df478fabf257b377fcfbaeb666) 

The top level acceleration structure's build flags.

# struct `RAYEXEC_NAMESPACE::Blas` 

A wrapper for a bottom level acceleration structure.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`AccelerationStructure`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure)` `[`as`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_1ae8ab4960986e35375952e1ba0b82d616) | The [RAYEXEC_NAMESPACE::AccelerationStructure](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) object containing the Vulkan acceleration structure.
`public vk::BuildAccelerationStructureFlagsKHR `[`flags`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_1af0e45ff33ae348578991083cbca3c3f5) | The top level acceleration structure's build flags.
`public std::vector< vk::AccelerationStructureCreateGeometryTypeInfoKHR > `[`asCreateGeometryInfo`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_1a1fca9090c51ee4dbf978ed04c2790585) | Specifies the shape of geometries that will be built into an acceleration structure.
`public std::vector< vk::AccelerationStructureGeometryKHR > `[`asGeometry`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_1a7379c987048d8c67571581c03d91cc34) | Data used to build acceleration structure geometry.
`public std::vector< vk::AccelerationStructureBuildOffsetInfoKHR > `[`asBuildOffsetInfo`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_1ad7a31d22f326b7c471166600681082e5) | The offset between acceleration structures when building.

## Members

#### `public `[`AccelerationStructure`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure)` `[`as`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_1ae8ab4960986e35375952e1ba0b82d616) 

The [RAYEXEC_NAMESPACE::AccelerationStructure](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) object containing the Vulkan acceleration structure.

#### `public vk::BuildAccelerationStructureFlagsKHR `[`flags`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_1af0e45ff33ae348578991083cbca3c3f5) 

The top level acceleration structure's build flags.

#### `public std::vector< vk::AccelerationStructureCreateGeometryTypeInfoKHR > `[`asCreateGeometryInfo`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_1a1fca9090c51ee4dbf978ed04c2790585) 

Specifies the shape of geometries that will be built into an acceleration structure.

#### `public std::vector< vk::AccelerationStructureGeometryKHR > `[`asGeometry`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_1a7379c987048d8c67571581c03d91cc34) 

Data used to build acceleration structure geometry.

#### `public std::vector< vk::AccelerationStructureBuildOffsetInfoKHR > `[`asBuildOffsetInfo`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_1ad7a31d22f326b7c471166600681082e5) 

The offset between acceleration structures when building.

# struct `RAYEXEC_NAMESPACE::BlasInstance` 

An instance of a bottom level acceleration structure.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint32_t `[`blasId`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a592985051a3fdbdd39391066ba138738) | The index of the bottom level acceleration structure in blas_.
`public uint32_t `[`instanceId`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a639034ff0385d327079b92511f30288c) | The instance index (gl_InstanceID).
`public uint32_t `[`hitGroupId`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a7a43c59559277912939adff408153407) | The hit group index in the shader binding table.
`public uint32_t `[`mask`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a4bcef7bf26c6b3af65dc2233fab831b1) | The visibility mask, will be AND-ed with the ray mask.
`public vk::GeometryInstanceFlagsKHR `[`flags`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a798ac14aa4a5fd08ce97185d33496e97) | The geometry display options.
`public glm::mat4 `[`transform`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a9e4ac50642ddb0e9eef7f8cb61a481b4) | The world transform matrix of the bottom level acceleration structure instance.

## Members

#### `public uint32_t `[`blasId`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a592985051a3fdbdd39391066ba138738) 

The index of the bottom level acceleration structure in blas_.

#### `public uint32_t `[`instanceId`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a639034ff0385d327079b92511f30288c) 

The instance index (gl_InstanceID).

#### `public uint32_t `[`hitGroupId`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a7a43c59559277912939adff408153407) 

The hit group index in the shader binding table.

#### `public uint32_t `[`mask`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a4bcef7bf26c6b3af65dc2233fab831b1) 

The visibility mask, will be AND-ed with the ray mask.

#### `public vk::GeometryInstanceFlagsKHR `[`flags`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a798ac14aa4a5fd08ce97185d33496e97) 

The geometry display options.

#### `public glm::mat4 `[`transform`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_blas_instance_1a9e4ac50642ddb0e9eef7f8cb61a481b4) 

The world transform matrix of the bottom level acceleration structure instance.

# struct `RAYEXEC_NAMESPACE::Vertex` 

Describes a vertex and its data.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::vec3 `[`pos`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_1a675b93fd4c003438c84b68192b87bedc) | The vertex's position in 3D space.
`public glm::vec3 `[`normal`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_1a53f646753a10728ead428c9b5b6c250e) | The vertex's normal vector.
`public glm::vec3 `[`color`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_1aff8b7f0bd8933579186b601fc8c48a7c) | The vertex's color.
`public glm::vec2 `[`texCoord`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_1a9141a1b57c5ae706ddc954bcbe5a8d6a) | The vertex's texture coordinate.
`public float `[`padding0`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_1ad66f6c035b7f48392c2f19a0c9e1bd40) | [Vertex](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex) padding 0.

## Members

#### `public glm::vec3 `[`pos`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_1a675b93fd4c003438c84b68192b87bedc) 

The vertex's position in 3D space.

#### `public glm::vec3 `[`normal`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_1a53f646753a10728ead428c9b5b6c250e) 

The vertex's normal vector.

#### `public glm::vec3 `[`color`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_1aff8b7f0bd8933579186b601fc8c48a7c) 

The vertex's color.

#### `public glm::vec2 `[`texCoord`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_1a9141a1b57c5ae706ddc954bcbe5a8d6a) 

The vertex's texture coordinate.

#### `public float `[`padding0`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex_1ad66f6c035b7f48392c2f19a0c9e1bd40) 

[Vertex](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex) padding 0.

