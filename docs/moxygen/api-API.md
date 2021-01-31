# group `API` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`RAYEX_NAMESPACE::Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api) | Initializes and owns all Vulkan components and renders an image.
`class `[`RAYEX_NAMESPACE::PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer) | Manages the building process of the acceleration structures.
`class `[`RAYEX_NAMESPACE::PostProcessingRenderer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer) | The post processing renderer acts as a second render pass for enabling post processing operations, such as gamma correction.
`class `[`RAYEX_NAMESPACE::Gui`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui) | A class to create an ImGui-based GUI.
`class `[`RAYEX_NAMESPACE::Scene`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene) | Stores all geoemtry, geometry instances and light sources.
`struct `[`RAYEX_NAMESPACE::AccelerationStructure`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) | A wrapper for a Vulkan acceleration Structure.
`struct `[`RAYEX_NAMESPACE::Tlas`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_tlas) | A wrapper for a top level acceleration structure.
`struct `[`RAYEX_NAMESPACE::Blas`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas) | A wrapper for a bottom level acceleration structure.
`struct `[`RAYEX_NAMESPACE::Vertex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex) | Describes a vertex and its data.
`struct `[`RAYEX_NAMESPACE::CameraUbo`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_ubo) | A memory aligned uniform buffer object for camera data.
`struct `[`RAYEX_NAMESPACE::MeshSSBO`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o) | A wrapper for [MeshSSBO](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o) matching the buffer alignment requirements.
`struct `[`RAYEX_NAMESPACE::GeometryInstanceSSBO`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o) | A wrapper for [GeometryInstanceSSBO](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o) matching the buffer alignment requirements.

# class `RAYEX_NAMESPACE::Api` 

Initializes and owns all Vulkan components and renders an image.

All API components and resources are freed using scope-bound destruction. 

Because of scope-bound destruction it is required to pay close attention to the order of resource allocations.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public friend `[`Rayex`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a654c4ab4f46b43e1dac112eafb763aac) | 
`public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a6d337866b5a344ec9f2a1c3e968f1fef)`() = default` | 
`public  `[`~Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a2466f3fcfd89ab8d1d80eed1254950c0)`()` | 
`public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a75fc41dab952a0b3c08d95a30ff451a8)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &) = delete` | 
`public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a12e91dc6ced5d4022accb12716616d11)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1afe3d5724cdbbf6e5756ec4b7c8cba6f0)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a48e629366ef5f94b8573a288c872955c)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &&) = delete` | 

## Members

#### `public friend `[`Rayex`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a654c4ab4f46b43e1dac112eafb763aac) 

#### `public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a6d337866b5a344ec9f2a1c3e968f1fef)`() = default` 

#### `public  `[`~Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a2466f3fcfd89ab8d1d80eed1254950c0)`()` 

#### `public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a75fc41dab952a0b3c08d95a30ff451a8)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &) = delete` 

#### `public  `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a12e91dc6ced5d4022accb12716616d11)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1afe3d5724cdbbf6e5756ec4b7c8cba6f0)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api_1a48e629366ef5f94b8573a288c872955c)`(const `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_api)` &&) = delete` 

# class `RAYEX_NAMESPACE::PathTracer` 

Manages the building process of the acceleration structures.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1ac98b96323e113ca2ac96c7d08ffaf624)`() = default` | 
`public  `[`~PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1aef8229369df0e2e75fea08d0163c0bfc)`()` | Calls [destroy()](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a29bafe2e2c61bd1ef76999885efa9188).
`public  `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a962dbd0ccd7400ccb8bba85f93dc263c)`(const `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer)` &) = delete` | 
`public  `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a4ebf5206c031cf0532596adb9b68fc3b)`(const `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a01b3b0262634e7993b1173c289f0db02)`(const `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer)` &) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1abc9fc7f8c1a71ef1cb056c5f5531af42)`(const `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer)` &&) = delete` | 
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1aec2798527165512a51c4028be6dd5da0)`()` | Retrieves the physical device's path tracing capabilities.
`public void `[`destroy`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a29bafe2e2c61bd1ef76999885efa9188)`()` | Destroys all bottom and top level acceleration structures.
`public inline auto `[`getTlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1aa57a79727d5cab887d198231377d0c7e)`() const` | #### Returns
`public inline auto `[`getStorageImageInfo`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1abde61bef57499d13e9df3cc3db0b8e39)`() const` | 
`public inline auto `[`getCapabilities`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a88b082f3c08f346b4bfa71836a9e3f81)`() const` | 
`public inline auto `[`getStorageImageView`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a148b5a6c45676515d7db5efe50e67428)`() const` | 
`public inline auto `[`getPipeline`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a87b1e74178484ae54e891f62b897b079)`() const` | 
`public inline auto `[`getPipelineLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a88b7351717bcb2731cda5254316384a3)`() const` | 
`public inline auto `[`getDescriptorSetLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a37d390a924dc52e0195248cedd5fe670)`()` | 
`public inline auto `[`getDescriptorSet`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a1305b431a280fa0bb99a15ea2d49eed6)`(size_t index)` | 
`public auto `[`modelToBlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1afe9a29b1bff77d11f7eb77a56d635356)`(const vkCore::StorageBuffer< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex)` > & vertexBuffer,const vkCore::StorageBuffer< uint32_t > & indexBuffer,bool allowTransforms) const` | Used to convert wavefront models to a bottom level acceleration structure.
`public auto `[`geometryInstanceToAccelerationStructureInstance`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1ae092e3687d1f24b48bcf056e5b8cb481)`(std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` > & geometryInstance)` | Used to convert a bottom level acceleration structure instance to a Vulkan geometry instance.
`public void `[`createBottomLevelAS`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a01e426560fbcea32ea1a1fb0b8fc3a6a)`(std::vector< vkCore::StorageBuffer< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex)` >> & vertexBuffers,const std::vector< vkCore::StorageBuffer< uint32_t >> & indexBuffers,const std::vector< std::shared_ptr< `[`Geometry`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry)` >> & geometries)` | Used to prepare building the bottom level acceleration structures.
`public void `[`buildBlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a5df109c2da39664245ad141e2d9c0e4a)`(vk::BuildAccelerationStructureFlagsKHR flags)` | Builds all bottom level acceleration structures.
`public void `[`buildTlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a3c2f2045a533d9fe53faafdb885f5831)`(const std::vector< std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` >> & geometryInstances,vk::BuildAccelerationStructureFlagsKHR flags,bool reuse)` | Build the top level acceleration structure.
`public void `[`createStorageImage`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a504cc5e1802f0174272f637ad6e8f4c6)`(vk::Extent2D swapchainExtent)` | Creates the storage image which the path tracing shaders will write to.
`public void `[`createShaderBindingTable`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1aef800dfb214e83264e2a573039a5aed3)`()` | Creates the shader binding tables.
`public void `[`createPipeline`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1af72332595cb39c940f0a6c9afee439ef)`(const std::vector< vk::DescriptorSetLayout > & descriptorSetLayouts,`[`Settings`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings)` * settings)` | Used to create a path tracing pipeline.
`public void `[`pathTrace`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1aed6907560423957efa3aec52c69aa240)`(vk::CommandBuffer swapchaincommandBuffer,vk::Image swapchainImage,vk::Extent2D extent)` | Used to record the actual path tracing commands to a given command buffer.
`public void `[`initDescriptorSet`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a30d8d1231355d7493d29b8226cfe728a)`()` | 
`public void `[`updateDescriptors`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a76874c6e9156b8c8f1624b43265fbc73)`()` | 

## Members

#### `public  `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1ac98b96323e113ca2ac96c7d08ffaf624)`() = default` 

#### `public  `[`~PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1aef8229369df0e2e75fea08d0163c0bfc)`()` 

Calls [destroy()](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a29bafe2e2c61bd1ef76999885efa9188).

#### `public  `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a962dbd0ccd7400ccb8bba85f93dc263c)`(const `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer)` &) = delete` 

#### `public  `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a4ebf5206c031cf0532596adb9b68fc3b)`(const `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a01b3b0262634e7993b1173c289f0db02)`(const `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer)` &) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1abc9fc7f8c1a71ef1cb056c5f5531af42)`(const `[`PathTracer`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer)` &&) = delete` 

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1aec2798527165512a51c4028be6dd5da0)`()` 

Retrieves the physical device's path tracing capabilities.

#### `public void `[`destroy`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a29bafe2e2c61bd1ef76999885efa9188)`()` 

Destroys all bottom and top level acceleration structures.

#### `public inline auto `[`getTlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1aa57a79727d5cab887d198231377d0c7e)`() const` 

#### Returns
Returns the top level acceleration structure.

#### `public inline auto `[`getStorageImageInfo`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1abde61bef57499d13e9df3cc3db0b8e39)`() const` 

#### `public inline auto `[`getCapabilities`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a88b082f3c08f346b4bfa71836a9e3f81)`() const` 

#### `public inline auto `[`getStorageImageView`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a148b5a6c45676515d7db5efe50e67428)`() const` 

#### `public inline auto `[`getPipeline`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a87b1e74178484ae54e891f62b897b079)`() const` 

#### `public inline auto `[`getPipelineLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a88b7351717bcb2731cda5254316384a3)`() const` 

#### `public inline auto `[`getDescriptorSetLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a37d390a924dc52e0195248cedd5fe670)`()` 

#### `public inline auto `[`getDescriptorSet`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a1305b431a280fa0bb99a15ea2d49eed6)`(size_t index)` 

#### `public auto `[`modelToBlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1afe9a29b1bff77d11f7eb77a56d635356)`(const vkCore::StorageBuffer< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex)` > & vertexBuffer,const vkCore::StorageBuffer< uint32_t > & indexBuffer,bool allowTransforms) const` 

Used to convert wavefront models to a bottom level acceleration structure.

#### Parameters
* `vertexBuffer` A vertex buffer of some geometry. 

* `indexBuffer` An index buffer of some geometry. 

#### Returns
Returns the bottom level acceleration structure.

#### `public auto `[`geometryInstanceToAccelerationStructureInstance`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1ae092e3687d1f24b48bcf056e5b8cb481)`(std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` > & geometryInstance)` 

Used to convert a bottom level acceleration structure instance to a Vulkan geometry instance.

#### Parameters
* `instance` A bottom level acceleration structure instance. 

#### Returns
Returns the Vulkan geometry instance.

#### `public void `[`createBottomLevelAS`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a01e426560fbcea32ea1a1fb0b8fc3a6a)`(std::vector< vkCore::StorageBuffer< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex)` >> & vertexBuffers,const std::vector< vkCore::StorageBuffer< uint32_t >> & indexBuffers,const std::vector< std::shared_ptr< `[`Geometry`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry)` >> & geometries)` 

Used to prepare building the bottom level acceleration structures.

#### Parameters
* `vertexBuffers` [Vertex](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex) buffers of all geometry in the scene. 

* `indexBuffers` Index buffers of all geometry in the scene.

#### `public void `[`buildBlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a5df109c2da39664245ad141e2d9c0e4a)`(vk::BuildAccelerationStructureFlagsKHR flags)` 

Builds all bottom level acceleration structures.

#### Parameters
* `blas_` A vector of [RAYEX_NAMESPACE::Blas](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas) objects containing all bottom level acceleration structures prepared in [createBottomLevelAS()](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a01e426560fbcea32ea1a1fb0b8fc3a6a). 

* `flags` The build flags.

#### `public void `[`buildTlas`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a3c2f2045a533d9fe53faafdb885f5831)`(const std::vector< std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` >> & geometryInstances,vk::BuildAccelerationStructureFlagsKHR flags,bool reuse)` 

Build the top level acceleration structure.

#### Parameters
* `instances` A vector of bottom level acceleration structure instances. 

* `flags` The build flags.

#### `public void `[`createStorageImage`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a504cc5e1802f0174272f637ad6e8f4c6)`(vk::Extent2D swapchainExtent)` 

Creates the storage image which the path tracing shaders will write to.

#### Parameters
* `swapchainExtent` The swapchain images' extent.

#### `public void `[`createShaderBindingTable`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1aef800dfb214e83264e2a573039a5aed3)`()` 

Creates the shader binding tables.

#### `public void `[`createPipeline`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1af72332595cb39c940f0a6c9afee439ef)`(const std::vector< vk::DescriptorSetLayout > & descriptorSetLayouts,`[`Settings`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings)` * settings)` 

Used to create a path tracing pipeline.

#### Parameters
* `descriptorSetLayouts` The descriptor set layouts for the shaders. 

* `settings` The renderer settings.

#### `public void `[`pathTrace`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1aed6907560423957efa3aec52c69aa240)`(vk::CommandBuffer swapchaincommandBuffer,vk::Image swapchainImage,vk::Extent2D extent)` 

Used to record the actual path tracing commands to a given command buffer.

#### Parameters
* `swapchaincommandBuffer` The command buffer to record to. 

* `swapchainImage` The current image in the swapchain. 

* `extent` The swapchain images' extent.

#### `public void `[`initDescriptorSet`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a30d8d1231355d7493d29b8226cfe728a)`()` 

#### `public void `[`updateDescriptors`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_path_tracer_1a76874c6e9156b8c8f1624b43265fbc73)`()` 

# class `RAYEX_NAMESPACE::PostProcessingRenderer` 

The post processing renderer acts as a second render pass for enabling post processing operations, such as gamma correction.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline auto `[`getRenderPass`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a2d3bd5259c28c3da4bc035017393fc2c)`() const` | 
`public inline auto `[`getPipeline`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a743e500ac21ab9b6b40005694dd6d738)`() const` | 
`public inline auto `[`getPipelineLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a011a18a3e3704b7a1db9538e090db1b1)`() const` | 
`public void `[`initDepthImage`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a01e2c8a445d8ea4f6df86cd05f3ab637)`(vk::Extent2D extent)` | 
`public void `[`initRenderPass`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a0d7a6f02d459e8f7197e75e0a54b795a)`(vk::Format format)` | 
`public void `[`initDescriptorSet`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a72d0b080d0e74207c556ce50959c4474)`()` | 
`public void `[`updateDescriptors`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a226a9f0e524c8901ed48d31959e4c692)`(const vk::DescriptorImageInfo & imageInfo)` | #### Parameters
`public void `[`initPipeline`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a8d2842efa22f36b3c680eef8a23b3048)`()` | 
`public void `[`beginRenderPass`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1acceeba217e38d1e290a3d39fe76f7be1)`(vk::CommandBuffer commandBuffer,vk::Framebuffer framebuffer,vk::Extent2D size)` | 
`public void `[`endRenderPass`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1ad609d5474fe35a3c56fcb219f37ec2ed)`(vk::CommandBuffer commandBuffer)` | 
`public void `[`render`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1aaf6b1d01d5a03b30ef04a6e491ee09ee)`(vk::CommandBuffer commandBuffer,vk::Extent2D size,size_t imageIndex)` | Records the draw calls to a given command buffer.

## Members

#### `public inline auto `[`getRenderPass`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a2d3bd5259c28c3da4bc035017393fc2c)`() const` 

#### `public inline auto `[`getPipeline`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a743e500ac21ab9b6b40005694dd6d738)`() const` 

#### `public inline auto `[`getPipelineLayout`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a011a18a3e3704b7a1db9538e090db1b1)`() const` 

#### `public void `[`initDepthImage`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a01e2c8a445d8ea4f6df86cd05f3ab637)`(vk::Extent2D extent)` 

#### `public void `[`initRenderPass`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a0d7a6f02d459e8f7197e75e0a54b795a)`(vk::Format format)` 

#### `public void `[`initDescriptorSet`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a72d0b080d0e74207c556ce50959c4474)`()` 

#### `public void `[`updateDescriptors`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a226a9f0e524c8901ed48d31959e4c692)`(const vk::DescriptorImageInfo & imageInfo)` 

#### Parameters
* `imageInfo` The descriptor image info of the path tracer's storage image.

#### `public void `[`initPipeline`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1a8d2842efa22f36b3c680eef8a23b3048)`()` 

#### `public void `[`beginRenderPass`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1acceeba217e38d1e290a3d39fe76f7be1)`(vk::CommandBuffer commandBuffer,vk::Framebuffer framebuffer,vk::Extent2D size)` 

#### `public void `[`endRenderPass`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1ad609d5474fe35a3c56fcb219f37ec2ed)`(vk::CommandBuffer commandBuffer)` 

#### `public void `[`render`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_post_processing_renderer_1aaf6b1d01d5a03b30ef04a6e491ee09ee)`(vk::CommandBuffer commandBuffer,vk::Extent2D size,size_t imageIndex)` 

Records the draw calls to a given command buffer.

#### Parameters
* `imageIndex` The current swapchain image index for selecting the correct descriptor set.

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
`public virtual void `[`configure`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a9683c711717892adbc9a5091ba5a1018)`()` | Used to configure all ImGui settings.
`public virtual void `[`render`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a3e5134da1391b2ca38bc65f17895d767)`()` | This function is for calling the individual ImGui components, e.g.
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1ab572cd6afb74d6d3303d3393dbaf6c06)`(SDL_Window * window,const vkCore::Surface * surface,vk::Extent2D swapchainImageExtent,vk::RenderPass renderPass)` | Creates the GUI and all required Vulkan components.
`public void `[`recreate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1af1bbc6bf62822168f9d28e42fd1345e5)`(vk::Extent2D swapchainImageExtent)` | Used to recreate the GUI in case the window size was changed.
`public void `[`renderDrawData`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1aec9cbe4d9c91740401c8781e760b7e49)`(vk::CommandBuffer commandBuffer)` | Records the ImGui rendering calls to the command buffer at the given image index.
`public void `[`destroy`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1acdc42f7c0d1a54a0a2e3bc9bb89df3ee)`()` | Destroys all ImGui resources.

## Members

#### `public virtual  `[`~Gui`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a37b52ebf2ec5b33b5bb817b6dc157169)`() = default` 

#### `public virtual void `[`configure`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a9683c711717892adbc9a5091ba5a1018)`()` 

Used to configure all ImGui settings.

The user should override this function if they want to change the style or various other configuration settings.

#### `public virtual void `[`render`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1a3e5134da1391b2ca38bc65f17895d767)`()` 

This function is for calling the individual ImGui components, e.g.

widgets.

The user should override this function to create their own GUI.

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1ab572cd6afb74d6d3303d3393dbaf6c06)`(SDL_Window * window,const vkCore::Surface * surface,vk::Extent2D swapchainImageExtent,vk::RenderPass renderPass)` 

Creates the GUI and all required Vulkan components.

#### Parameters
* `surface` A pointer to a RAYEX_NAMESPACE::Surface object. 

* `swapchainImageExtent` The extent of the swapchain images.

#### `public void `[`recreate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1af1bbc6bf62822168f9d28e42fd1345e5)`(vk::Extent2D swapchainImageExtent)` 

Used to recreate the GUI in case the window size was changed.

#### Parameters
* `swapchainImageExtent` The extent of the swapchain images. 

* `swapchainImageViews` The swapchain images' image views.

#### `public void `[`renderDrawData`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1aec9cbe4d9c91740401c8781e760b7e49)`(vk::CommandBuffer commandBuffer)` 

Records the ImGui rendering calls to the command buffer at the given image index.

#### Parameters
* `imageIndex` The index addressing a command buffer.

#### `public void `[`destroy`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui_1acdc42f7c0d1a54a0a2e3bc9bb89df3ee)`()` 

Destroys all ImGui resources.

# class `RAYEX_NAMESPACE::Scene` 

Stores all geoemtry, geometry instances and light sources.

Provides functions to change said data. 
> Todo: [removeGeometry()](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a8004b0c0dfb783070ea82af111378718)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public friend `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1ae002d16199c05b908fbeecc4f7d97733) | 
`public friend `[`Rayex`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a03b7257e44db8e9af952484958a5346c) | 
`public auto `[`getGeometryInstances`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1aef804f5fc22e5d0b27530a00cd054060)`() const` | #### Returns
`public void `[`submitGeometryInstance`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a4ca1e82f10c27e433eff652710e8e700)`(std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` > geometryInstance)` | Used to submit a geometry instance for rendering.
`public void `[`setGeometryInstances`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a86b0c09d5294dabeeb550b84ef4ac0c4)`(const std::vector< std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` >> & geometryInstances)` | Used to submit multiple geometry instances for rendering, replacing all existing instances.
`public void `[`removeGeometryInstance`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a11fc56eb116fe3301a59c5c594ee2bea)`(std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` > geometryInstance)` | Used to remove a geometry instance.
`public void `[`clearGeometryInstances`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a42dc12d8a200591532dc336f72a4847d)`()` | Used to remove all geometry instances.
`public void `[`popGeometryInstance`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a838076f7347dd4788ee1aaa8ffd28374)`()` | Used to remove the last geometry instance.
`public void `[`submitGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a53c0ef48ca0c5e0d2336869a8e75085b)`(std::shared_ptr< `[`Geometry`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry)` > geometry)` | Used to submit a geometry and set up its buffers.
`public void `[`setGeometries`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1ab191100612c6087204b476aadfd8b6db)`(const std::vector< std::shared_ptr< `[`Geometry`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry)` >> & geometries)` | Used to submit multiple geometries and set up their buffers.
`public void `[`removeGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a8004b0c0dfb783070ea82af111378718)`(std::shared_ptr< `[`Geometry`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry)` > geometry)` | Used to remove a geometry.
`public void `[`removeGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1adc0e77e152173899b4edf08fbe62ca18)`(uint32_t geometryIndex)` | Used to remove a geometry.
`public void `[`popGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1ae68fc066eddb4ff7ee2a16c26b06f5ba)`()` | Used to remove the last geometry and all its instances.
`public void `[`clearGeometries`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1ac89ff3616b43e6c2b7e6a23995784a53)`()` | Used to remove all geometries.
`public auto `[`findGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a344905cab1fbaf5a5585a6df25635bbc)`(std::string_view path) const` | Used to retrieve a geoemtry based on its path.
`public void `[`setEnvironmentMap`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a87356635d94226e8c69857c0b48d3ad1)`(std::string_view path)` | 
`public void `[`removeEnvironmentMap`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a76b628140f11ee0ad272ac1da0e6c0ff)`()` | 
`public void `[`load`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a7f8086b8e638a0ad2089255fa76d127e)`(const std::string & path)` | 
`public void `[`setCamera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1aa3d8bb2201e4529f67dd8138d7f0e538)`(std::shared_ptr< `[`Camera`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` > camera)` | Used to set a custom camera.
`public inline auto `[`getCamera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a93bff45acd035aa5c72445bca9604075)`() const` | #### Returns

## Members

#### `public friend `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1ae002d16199c05b908fbeecc4f7d97733) 

#### `public friend `[`Rayex`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a03b7257e44db8e9af952484958a5346c) 

#### `public auto `[`getGeometryInstances`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1aef804f5fc22e5d0b27530a00cd054060)`() const` 

#### Returns
Returns all geometry instances in the scene.

#### `public void `[`submitGeometryInstance`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a4ca1e82f10c27e433eff652710e8e700)`(std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` > geometryInstance)` 

Used to submit a geometry instance for rendering.

#### Parameters
* `geometryInstance` The instance to queue for rendering. 

This function does not invoke any draw calls.

#### `public void `[`setGeometryInstances`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a86b0c09d5294dabeeb550b84ef4ac0c4)`(const std::vector< std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` >> & geometryInstances)` 

Used to submit multiple geometry instances for rendering, replacing all existing instances.

#### Parameters
* `geometryInstances` The instances to queue for rendering. 

This function does not invoke any draw calls.

#### `public void `[`removeGeometryInstance`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a11fc56eb116fe3301a59c5c594ee2bea)`(std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` > geometryInstance)` 

Used to remove a geometry instance.

Once a geometry instance was removed, it will no longer be rendered. 
#### Parameters
* `geometryInstance` The instance to remove.

#### `public void `[`clearGeometryInstances`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a42dc12d8a200591532dc336f72a4847d)`()` 

Used to remove all geometry instances.

However, geometries remain loaded and must be deleted explicitely.

#### `public void `[`popGeometryInstance`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a838076f7347dd4788ee1aaa8ffd28374)`()` 

Used to remove the last geometry instance.

#### `public void `[`submitGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a53c0ef48ca0c5e0d2336869a8e75085b)`(std::shared_ptr< `[`Geometry`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry)` > geometry)` 

Used to submit a geometry and set up its buffers.

Once a geometry was submitted, geometry instances referencing this particular geometry can be drawn. 
#### Parameters
* `geometry` The geometry to submit.

#### `public void `[`setGeometries`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1ab191100612c6087204b476aadfd8b6db)`(const std::vector< std::shared_ptr< `[`Geometry`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry)` >> & geometries)` 

Used to submit multiple geometries and set up their buffers.

Once a geometry was submitted, geometry instances referencing this particular geometry can be drawn. 
#### Parameters
* `geometries` The geometries to submit.

#### `public void `[`removeGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a8004b0c0dfb783070ea82af111378718)`(std::shared_ptr< `[`Geometry`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry)` > geometry)` 

Used to remove a geometry.

#### Parameters
* `geometry` The geometry to remove.

#### `public void `[`removeGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1adc0e77e152173899b4edf08fbe62ca18)`(uint32_t geometryIndex)` 

Used to remove a geometry.

#### Parameters
* `geometryIndex` The geometry's index.

#### `public void `[`popGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1ae68fc066eddb4ff7ee2a16c26b06f5ba)`()` 

Used to remove the last geometry and all its instances.

#### `public void `[`clearGeometries`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1ac89ff3616b43e6c2b7e6a23995784a53)`()` 

Used to remove all geometries.

#### `public auto `[`findGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a344905cab1fbaf5a5585a6df25635bbc)`(std::string_view path) const` 

Used to retrieve a geoemtry based on its path.

#### Parameters
* `path` The geometry's model's path, relative to the path to assets.

#### `public void `[`setEnvironmentMap`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a87356635d94226e8c69857c0b48d3ad1)`(std::string_view path)` 

#### `public void `[`removeEnvironmentMap`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a76b628140f11ee0ad272ac1da0e6c0ff)`()` 

#### `public void `[`load`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a7f8086b8e638a0ad2089255fa76d127e)`(const std::string & path)` 

#### `public void `[`setCamera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1aa3d8bb2201e4529f67dd8138d7f0e538)`(std::shared_ptr< `[`Camera`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` > camera)` 

Used to set a custom camera.

#### Parameters
* `camera` A pointer to a [RAYEX_NAMESPACE::Camera](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera) object.

#### `public inline auto `[`getCamera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a93bff45acd035aa5c72445bca9604075)`() const` 

#### Returns
Returns a pointer to the renderer's camera.

# struct `RAYEX_NAMESPACE::AccelerationStructure` 

A wrapper for a Vulkan acceleration Structure.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public vk::AccelerationStructureKHR `[`as`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a310d8142a50e671e29b626943fa2b6c8) | The Vulkan acceleration structure.
`public vk::DeviceMemory `[`memory`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a67c749f7c097fac7a114c947b88752c0) | The acceleration structure's memory.
`public vk::Buffer `[`buffer`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a6add5ce884f6fb46a60a89bfb565fc09) | 
`public void `[`destroy`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a8e189940953cc7155465cf65a2f1d4f4)`()` | Used to destroy the acceleration structure and free its memory.

## Members

#### `public vk::AccelerationStructureKHR `[`as`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a310d8142a50e671e29b626943fa2b6c8) 

The Vulkan acceleration structure.

#### `public vk::DeviceMemory `[`memory`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a67c749f7c097fac7a114c947b88752c0) 

The acceleration structure's memory.

#### `public vk::Buffer `[`buffer`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a6add5ce884f6fb46a60a89bfb565fc09) 

#### `public void `[`destroy`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure_1a8e189940953cc7155465cf65a2f1d4f4)`()` 

Used to destroy the acceleration structure and free its memory.

# struct `RAYEX_NAMESPACE::Tlas` 

A wrapper for a top level acceleration structure.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`AccelerationStructure`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure)` `[`as`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_tlas_1aef65925b7d115143b5e5b3b61a048566) | The [RAYEX_NAMESPACE::AccelerationStructure](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) object containing the Vulkan acceleration structure.

## Members

#### `public `[`AccelerationStructure`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure)` `[`as`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_tlas_1aef65925b7d115143b5e5b3b61a048566) 

The [RAYEX_NAMESPACE::AccelerationStructure](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) object containing the Vulkan acceleration structure.

# struct `RAYEX_NAMESPACE::Blas` 

A wrapper for a bottom level acceleration structure.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`AccelerationStructure`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure)` `[`as`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1afeefd899332ab00ffc38a7641424de2d) | The [RAYEX_NAMESPACE::AccelerationStructure](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) object containing the Vulkan acceleration structure.
`public std::vector< vk::AccelerationStructureGeometryKHR > `[`asGeometry`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1a7a270840d9f4e6b141a6f142abda68c5) | Data used to build acceleration structure geometry.
`public std::vector< vk::AccelerationStructureBuildRangeInfoKHR > `[`asBuildRangeInfo`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1a612e8ebe1c04fd44ab8e9c22fc7c9787) | The offset between acceleration structures when building.

## Members

#### `public `[`AccelerationStructure`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure)` `[`as`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1afeefd899332ab00ffc38a7641424de2d) 

The [RAYEX_NAMESPACE::AccelerationStructure](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_acceleration_structure) object containing the Vulkan acceleration structure.

#### `public std::vector< vk::AccelerationStructureGeometryKHR > `[`asGeometry`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1a7a270840d9f4e6b141a6f142abda68c5) 

Data used to build acceleration structure geometry.

#### `public std::vector< vk::AccelerationStructureBuildRangeInfoKHR > `[`asBuildRangeInfo`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_blas_1a612e8ebe1c04fd44ab8e9c22fc7c9787) 

The offset between acceleration structures when building.

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

# struct `RAYEX_NAMESPACE::CameraUbo` 

A memory aligned uniform buffer object for camera data.

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

# struct `RAYEX_NAMESPACE::MeshSSBO` 

A wrapper for [MeshSSBO](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o) matching the buffer alignment requirements.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::vec4 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a7ee226655334d745110ea673d3f415b7) | vec3 ambient + vec1 texture index
`public glm::vec4 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1ad7dfd9e1d94bf8b61d75c5cb5b96029c) | vec3 diffuse + vec1 texture index
`public glm::vec4 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a82baebb3cacb3f4b74ef8621fe9fdb34) | vec3 specular + vec1 texture index
`public glm::vec4 `[`emission`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a7fcf00acc1ec69abc1b1cd9a14149b5c) | vec3 emission + vec1 texture index
`public uint32_t `[`illum`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1ac234ac13f69f4325c449a78fa5d04925) | 
`public float `[`d`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a7a21ad8a0b5301090fb445dc5de3027a) | 
`public float `[`ns`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1afe758e896b9c1ea5656c3402b35462ba) | 
`public float `[`ni`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1aac714eae32d19dcfa68661ffa006520c) | 
`public uint32_t `[`indexOffset`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a8a30eb7713b788d6abf948cbe029acdd) | Refers to the offset of this mesh inside a [Geometry::indices](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a11ecc2461cc8f940666cc5535e977c0f) container.
`public uint32_t `[`padding0`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a0c0eaca095bc79dcaf63e60d0f71db7e) | Buffer padding (ignore).
`public uint32_t `[`padding1`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a54e54f44292cedf0a51415bbb34ea9fb) | Buffer padding (ignore).
`public uint32_t `[`padding2`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a4f5c5d76b5f953139f857b130e34e40d) | Buffer padding (ignore).
`public  `[`MeshSSBO`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1ad8e3bd702c57644a9b5ca13e0dd79c9b)`() = default` | 
`public inline  `[`MeshSSBO`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a2f11eb9c12cbbb7424d020c1acc0beee)`(const `[`Mesh`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh)` & mesh,float diffuseTexIndex)` | 
`public inline void `[`set`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a39423de79f0f958e9b23053c3b0f7387)`(const `[`Mesh`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh)` & mesh,float diffuseTexIndex)` | 

## Members

#### `public glm::vec4 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a7ee226655334d745110ea673d3f415b7) 

vec3 ambient + vec1 texture index

#### `public glm::vec4 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1ad7dfd9e1d94bf8b61d75c5cb5b96029c) 

vec3 diffuse + vec1 texture index

#### `public glm::vec4 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a82baebb3cacb3f4b74ef8621fe9fdb34) 

vec3 specular + vec1 texture index

#### `public glm::vec4 `[`emission`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a7fcf00acc1ec69abc1b1cd9a14149b5c) 

vec3 emission + vec1 texture index

#### `public uint32_t `[`illum`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1ac234ac13f69f4325c449a78fa5d04925) 

#### `public float `[`d`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a7a21ad8a0b5301090fb445dc5de3027a) 

#### `public float `[`ns`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1afe758e896b9c1ea5656c3402b35462ba) 

#### `public float `[`ni`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1aac714eae32d19dcfa68661ffa006520c) 

#### `public uint32_t `[`indexOffset`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a8a30eb7713b788d6abf948cbe029acdd) 

Refers to the offset of this mesh inside a [Geometry::indices](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a11ecc2461cc8f940666cc5535e977c0f) container.

#### `public uint32_t `[`padding0`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a0c0eaca095bc79dcaf63e60d0f71db7e) 

Buffer padding (ignore).

#### `public uint32_t `[`padding1`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a54e54f44292cedf0a51415bbb34ea9fb) 

Buffer padding (ignore).

#### `public uint32_t `[`padding2`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a4f5c5d76b5f953139f857b130e34e40d) 

Buffer padding (ignore).

#### `public  `[`MeshSSBO`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1ad8e3bd702c57644a9b5ca13e0dd79c9b)`() = default` 

#### `public inline  `[`MeshSSBO`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a2f11eb9c12cbbb7424d020c1acc0beee)`(const `[`Mesh`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh)` & mesh,float diffuseTexIndex)` 

#### `public inline void `[`set`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_s_s_b_o_1a39423de79f0f958e9b23053c3b0f7387)`(const `[`Mesh`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh)` & mesh,float diffuseTexIndex)` 

# struct `RAYEX_NAMESPACE::GeometryInstanceSSBO` 

A wrapper for [GeometryInstanceSSBO](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o) matching the buffer alignment requirements.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::mat4 `[`transform`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a549ca0f380012d4039a3c9f1a622632d) | The instance's world transform matrix.
`public glm::mat4 `[`transformIT`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a1379cb3dfff56920119ce6a1bbb38205) | The inverse transpose of transform.
`public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a4899744b1f0ee202bbf9ff02af61b4c4) | Used to assign this instance a model.
`public float `[`padding0`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1ac708ce2bf5edf245a01181441632b7ca) | Buffer padding (ignore).
`public float `[`padding1`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a11f43ebd571824166af4d080c989706b) | Buffer padding (ignore).
`public float `[`padding2`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a9730ed86d0ee5f518395e2b6c13ea2dc) | Buffer padding (ignore).

## Members

#### `public glm::mat4 `[`transform`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a549ca0f380012d4039a3c9f1a622632d) 

The instance's world transform matrix.

#### `public glm::mat4 `[`transformIT`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a1379cb3dfff56920119ce6a1bbb38205) 

The inverse transpose of transform.

#### `public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a4899744b1f0ee202bbf9ff02af61b4c4) 

Used to assign this instance a model.

#### `public float `[`padding0`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1ac708ce2bf5edf245a01181441632b7ca) 

Buffer padding (ignore).

#### `public float `[`padding1`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a11f43ebd571824166af4d080c989706b) 

Buffer padding (ignore).

#### `public float `[`padding2`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_s_s_b_o_1a9730ed86d0ee5f518395e2b6c13ea2dc) 

Buffer padding (ignore).

