# group `Base` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public RX_API std::shared_ptr< Geometry > `[`loadObj`](#group___base_1ga02a95a02aa5d57299c4eecb3c6bb4d3d)`(std::string_view path)`            | A commodity function for loading a wavefront (.obj) model file and allocate a geometry object from it.
`public RX_API std::shared_ptr< GeometryInstance > `[`instance`](#group___base_1gabb8acb5237813d1ac88e190a79a1bff2)`(std::shared_ptr< Geometry > geometry,const glm::mat4 & transform)`            | A commodity function for allocating an instance from a given geometry and set its matrices.
`class `[`RAYEXEC_NAMESPACE::Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera) | A minimal camera implementation.
`class `[`RAYEXEC_NAMESPACE::Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window) | Implements a SDL-based window.
`class `[`RAYEXEC_NAMESPACE::RayExec`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec) | The main user interface.
`struct `[`RAYEXEC_NAMESPACE::Material`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_material) | Describes the rendering properties of a mesh.
`struct `[`RAYEXEC_NAMESPACE::Mesh`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_mesh) | Describes a sub-mesh and its material.
`struct `[`RAYEXEC_NAMESPACE::Geometry`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry) | Describes a geometry [RayExec](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec) can render.
`struct `[`RAYEXEC_NAMESPACE::GeometryInstance`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance) | Describes an instance of some geometry.

## Members

#### `public RX_API std::shared_ptr< Geometry > `[`loadObj`](#group___base_1ga02a95a02aa5d57299c4eecb3c6bb4d3d)`(std::string_view path)` 

A commodity function for loading a wavefront (.obj) model file and allocate a geometry object from it.

The function will attempt to find sub-meshes in the file and retrieve all materials. A user is encouraged to create their own model loader function or classes. 
#### Parameters
* `path` The model's path, relative to the path to assets. 

#### Returns
Returns a pointer to a geometry object.

#### `public RX_API std::shared_ptr< GeometryInstance > `[`instance`](#group___base_1gabb8acb5237813d1ac88e190a79a1bff2)`(std::shared_ptr< Geometry > geometry,const glm::mat4 & transform)` 

A commodity function for allocating an instance from a given geometry and set its matrices.

The function will also automatically set the inverse transpose matrix. 
#### Parameters
* `geometry` The geometry to create the instance from. 

* `transform` The world transform matrix of the instance. 

#### Returns
Returns a pointer to a geometry instance.

# class `RAYEXEC_NAMESPACE::Camera` 

A minimal camera implementation.

This class acts like an interface for the user by providing the most important camera-related matrices as well as the camera's position, which are required by the rendering API. 

Example
```cpp
// This example requires the user to implement a custom camera class that inherits from Camera.
auto myCam = std::make_shared<CustomCamera>( 600, 500, glm::vec3{ 0.0f, 0.0f, 3.0f } );
```

The user has to handle keyboard related camera changes inside [update()](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a17a52094a998d73590caa62e219064b1). 

Do not forget to re-calculate the view or projection matrix if the camera or the window have changed. See [updateViewMatrix()](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1ad857cce17e25f1a9dc5a14140deca385), [updateProjectionMatrix()](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a129b7201b6218d65b48b65525d37f924) or updateView and updateProj respectively.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public bool `[`updateView`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a8c1fbe8efac8d9a473fa1bdceddf7718) | Keeps track of whether or not to udpate the view matrix.
`public bool `[`updateProj`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a83156fc5a103492ffe389d3b0b4c18d3) | Keeps track of whether or not to udpate the projection matrix.
`public RX_API `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a490dea6c59a325694231585bcea1272f)`(int width,int height,const glm::vec3 & position)` | #### Parameters
`public virtual RX_API `[`~Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a52426e0d9e1e14ee22032864e08b8950)`() = default` | 
`public RX_API `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1ad20267869addb34295336598835ebb92)`(const `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera)` &) = default` | 
`public  `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a467c7152a8ad13deed4fe7f46f0c2e68)`(const `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera)` &&) = delete` | 
`public RX_API auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a11db4c76805ab39f525f7420f9d54934)`(const `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera)` &) = default` | 
`public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a132544defa696e77a24bc2c9e272b060)`(const `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera)` &&) = delete` | 
`public virtual RX_API void `[`update`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a17a52094a998d73590caa62e219064b1)`()` | Is used to update camera vectors etc.
`public inline RX_API auto `[`getPosition`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a9e28a26cf3530e9a35e90acd80bbdc92)`() const` | #### Returns
`public RX_API void `[`setPosition`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a760a878e62be9e6c1703189e77cc6c9e)`(const glm::vec3 & position)` | Is used to change the camera's position.
`public RX_API void `[`setSize`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a79953238e48d989a08a5a3cf6e5e341c)`(int width,int height)` | Is used to set a size for the camera that fits the viewport dimensions.
`public RX_API void `[`setFov`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1aa2f5d457dd30aa6bc4303cb434c42de3)`(float fov)` | Is used to set the camera's field of view.
`public RX_API void `[`setSensitivity`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a7a5d343f5d9b1fe99ad1dbb15d45c271)`(float sensitivity)` | Is used to set the mouse sensitivity.
`public inline auto `[`getViewMatrix`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a6903135d34ee41d5f3af63c3a00ad35a)`() const` | #### Returns
`public inline auto `[`getProjectionMatrix`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a9ff0f0ef85ce403930035cea2e2ecbaf)`() const` | #### Returns
`public inline auto `[`getViewInverseMatrix`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a730a3b81a3909dc4fd394b4faa46d1d9)`() const` | #### Returns
`public inline auto `[`getProjectionInverseMatrix`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1aad866a830b16be3939625667dd682550)`() const` | #### Returns
`public RX_API void `[`updateViewMatrix`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1ad857cce17e25f1a9dc5a14140deca385)`()` | Re-calculates the camera's view matrix as well as the inversed view matrix.
`public RX_API void `[`updateProjectionMatrix`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a129b7201b6218d65b48b65525d37f924)`()` | Re-calculates the camera's projection matrix as well as the inversed projection matrix.
`public virtual RX_API void `[`processMouse`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a831e7eafb8336182831f82a311ecff6f)`(float xOffset,float yOffset)` | Processes mouse input (default implementation).
`public inline virtual RX_API void `[`processKeyboard`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1aae0b352b60daeb87a24a49a412327b31)`()` | 
`protected int `[`width`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1ac9ff837a1fdc36a3558ed90905f6a59c) | The width of the viewport.
`protected int `[`height`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1ad3d5ad1b1e0f703237f8e515900af570) | The height of the viewport.
`protected glm::vec3 `[`position`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1af922608fc6c78b2385b9052c59904e88) | The camera's position.
`protected glm::mat4 `[`view`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a3cf917a8f2a43ac25c06f8d5a4762a8e) | The view matrix.
`protected glm::mat4 `[`projection`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a484cb7902b79ae5cfa30e3763a17712b) | The projection matrix.
`protected glm::mat4 `[`viewInverse`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a0398f31133b8ec4bfab87e28a811ab3e) | The view matrix inversed.
`protected glm::mat4 `[`projectionInverse`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a057b681aa8001944c48c3933735b8217) | The projection matrix inversed.
`protected glm::vec3 `[`worldUp`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a9fd112d108497d0418773a12dff2f502) | The world up vector.
`protected glm::vec3 `[`up`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1aa6b77663fd6679700969aea393fce5db) | The local up vector.
`protected glm::vec3 `[`right`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a4f91c61fbfcfa3a10dff3d5dbba0bad7) | The local right vector.
`protected glm::vec3 `[`front`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1ae254cca6149d934d78c888480143b24b) | The viewing direction.
`protected float `[`yaw`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a2c16425a6f274ee0906e7b76f3134374) | The yaw (left and right).
`protected float `[`pitch`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1afa1ddc44eda944d6117fbe85d382577f) | The pitch (down and up).
`protected float `[`sensitivity`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a897532a88cbd7fa33b9c090eb6ede012) | The mouse sensitivity.
`protected float `[`fov`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a45d31c4488fdeb359d36c0ee3ac7bbe2) | The field of view.
`protected RX_API void `[`updateVectors`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a118e91eafecb3bf19c9def7e55766f25)`()` | Updates the camera vectors.

## Members

#### `public bool `[`updateView`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a8c1fbe8efac8d9a473fa1bdceddf7718) 

Keeps track of whether or not to udpate the view matrix.

#### `public bool `[`updateProj`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a83156fc5a103492ffe389d3b0b4c18d3) 

Keeps track of whether or not to udpate the projection matrix.

#### `public RX_API `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a490dea6c59a325694231585bcea1272f)`(int width,int height,const glm::vec3 & position)` 

#### Parameters
* `width` The width of the viewport. 

* `height` The height of the viewport. 

* `position` The position of your camera.

#### `public virtual RX_API `[`~Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a52426e0d9e1e14ee22032864e08b8950)`() = default` 

#### `public RX_API `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1ad20267869addb34295336598835ebb92)`(const `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera)` &) = default` 

#### `public  `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a467c7152a8ad13deed4fe7f46f0c2e68)`(const `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera)` &&) = delete` 

#### `public RX_API auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a11db4c76805ab39f525f7420f9d54934)`(const `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera)` &) = default` 

#### `public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a132544defa696e77a24bc2c9e272b060)`(const `[`Camera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera)` &&) = delete` 

#### `public virtual RX_API void `[`update`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a17a52094a998d73590caa62e219064b1)`()` 

Is used to update camera vectors etc.

The user has to override this function for the camera to work like intended. The function will be called every tick.

#### `public inline RX_API auto `[`getPosition`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a9e28a26cf3530e9a35e90acd80bbdc92)`() const` 

#### Returns
Returns the camera's position.

#### `public RX_API void `[`setPosition`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a760a878e62be9e6c1703189e77cc6c9e)`(const glm::vec3 & position)` 

Is used to change the camera's position.

#### Parameters
* `position` The new camera position.

#### `public RX_API void `[`setSize`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a79953238e48d989a08a5a3cf6e5e341c)`(int width,int height)` 

Is used to set a size for the camera that fits the viewport dimensions.

#### Parameters
* `width` The width of the viewport. 

* `height` The height of the viewport.

#### `public RX_API void `[`setFov`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1aa2f5d457dd30aa6bc4303cb434c42de3)`(float fov)` 

Is used to set the camera's field of view.

#### Parameters
* `fov` The new field of view.

#### `public RX_API void `[`setSensitivity`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a7a5d343f5d9b1fe99ad1dbb15d45c271)`(float sensitivity)` 

Is used to set the mouse sensitivity.

#### Parameters
* `sensitivity` The new mouse sensitivity.

#### `public inline auto `[`getViewMatrix`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a6903135d34ee41d5f3af63c3a00ad35a)`() const` 

#### Returns
The view matrix.

#### `public inline auto `[`getProjectionMatrix`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a9ff0f0ef85ce403930035cea2e2ecbaf)`() const` 

#### Returns
The projection matrix.

#### `public inline auto `[`getViewInverseMatrix`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a730a3b81a3909dc4fd394b4faa46d1d9)`() const` 

#### Returns
The view matrix inversed.

#### `public inline auto `[`getProjectionInverseMatrix`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1aad866a830b16be3939625667dd682550)`() const` 

#### Returns
The projection matrix inversed.

#### `public RX_API void `[`updateViewMatrix`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1ad857cce17e25f1a9dc5a14140deca385)`()` 

Re-calculates the camera's view matrix as well as the inversed view matrix.

#### `public RX_API void `[`updateProjectionMatrix`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a129b7201b6218d65b48b65525d37f924)`()` 

Re-calculates the camera's projection matrix as well as the inversed projection matrix.

#### `public virtual RX_API void `[`processMouse`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a831e7eafb8336182831f82a311ecff6f)`(float xOffset,float yOffset)` 

Processes mouse input (default implementation).

#### Parameters
* `xOffset` The difference of the current offset on the x-axis and the previous offset. 

* `yOffset` The difference of the current offset on the y-axis and the previous offset.

#### `public inline virtual RX_API void `[`processKeyboard`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1aae0b352b60daeb87a24a49a412327b31)`()` 

#### `protected int `[`width`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1ac9ff837a1fdc36a3558ed90905f6a59c) 

The width of the viewport.

#### `protected int `[`height`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1ad3d5ad1b1e0f703237f8e515900af570) 

The height of the viewport.

#### `protected glm::vec3 `[`position`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1af922608fc6c78b2385b9052c59904e88) 

The camera's position.

#### `protected glm::mat4 `[`view`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a3cf917a8f2a43ac25c06f8d5a4762a8e) 

The view matrix.

#### `protected glm::mat4 `[`projection`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a484cb7902b79ae5cfa30e3763a17712b) 

The projection matrix.

#### `protected glm::mat4 `[`viewInverse`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a0398f31133b8ec4bfab87e28a811ab3e) 

The view matrix inversed.

#### `protected glm::mat4 `[`projectionInverse`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a057b681aa8001944c48c3933735b8217) 

The projection matrix inversed.

#### `protected glm::vec3 `[`worldUp`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a9fd112d108497d0418773a12dff2f502) 

The world up vector.

#### `protected glm::vec3 `[`up`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1aa6b77663fd6679700969aea393fce5db) 

The local up vector.

#### `protected glm::vec3 `[`right`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a4f91c61fbfcfa3a10dff3d5dbba0bad7) 

The local right vector.

#### `protected glm::vec3 `[`front`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1ae254cca6149d934d78c888480143b24b) 

The viewing direction.

#### `protected float `[`yaw`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a2c16425a6f274ee0906e7b76f3134374) 

The yaw (left and right).

#### `protected float `[`pitch`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1afa1ddc44eda944d6117fbe85d382577f) 

The pitch (down and up).

#### `protected float `[`sensitivity`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a897532a88cbd7fa33b9c090eb6ede012) 

The mouse sensitivity.

#### `protected float `[`fov`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a45d31c4488fdeb359d36c0ee3ac7bbe2) 

The field of view.

#### `protected RX_API void `[`updateVectors`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a118e91eafecb3bf19c9def7e55766f25)`()` 

Updates the camera vectors.

Only needs to be called if mouse was moved.

# class `RAYEXEC_NAMESPACE::Window` 

Implements a SDL-based window.

In order to handle events the user has to create a class that inherits from [Window](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window). 

Example
```cpp
// This example requires the user to implement a custom window class that inherits from Window.
auto myWindow = std::make_shared<CustomWindow>( [width](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a5c3593b500b23e7caa7fab8be33fc9f5), [height](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1ac05ed1f5db515ce709c038130c06ae3a), "Example", WINDOW_RESIZABLE | WINDOW_INPUT_FOCUS );
```

**See also**: [update()](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1aa038bb7bf14b31b0d1c3cff6daf07285)

> Todo: To avoid the user having to link to SDL themselves requires to write a complete SDL2 wrapper.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public RX_API `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a605d01de99b761411a51560f659b5ccd)`(int width,int height,const char * title,uint32_t flags) = default` | #### Parameters
`public virtual RX_API `[`~Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a4aabfbbcd46b15e96661e5990704ed75)`()` | Destructor of [Window](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window).
`public  `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a3de9e4815cc97d30787058b683f80c56)`(const `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window)` &) = delete` | 
`public  `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1ae6072e2dfb9cb8d2738befb1bf4fcc0f)`(const `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1adfbf2437f3931c8b235783466ee61230)`(const `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window)` &) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a7f1b2cdbdec544d883be6d7f5ee02fcc)`(const `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window)` &&) = delete` | 
`public virtual RX_API auto `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a1465803d1631aeb85f3c304617d7f2c7)`()` | Initializes the SDL-window.
`public virtual RX_API auto `[`update`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1aa038bb7bf14b31b0d1c3cff6daf07285)`()` | Updates window-related components.
`public virtual RX_API void `[`clean`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a18c7d4abe800472a8e79bd4f18cfb63e)`()` | Destroys the window.
`public RX_API void `[`resize`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1ab051017a633806871575e8ec129cfd8a)`(int width,int height)` | Resizes the window.
`public inline RX_API auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a2499009d44ee72861fccea9923a69f04)`()` | #### Returns
`public RX_API auto `[`getExtent`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1af9cf0260c8125403ab94097965ef2955)`() const` | #### Returns
`public inline auto `[`getWidth`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a3493050f0c87cc92dd5367086fa4232d)`() const` | #### Returns
`public inline auto `[`getHeight`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1acb5f534b676c8cd3d640f47ac4444499)`() const` | #### Returns
`public RX_API auto `[`changed`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a3bf6eadc1f1aa70f3335d505edf30ff8)`()` | #### Returns
`public RX_API auto `[`minimized`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a073466b019e4961a3823a50019f66776)`()` | #### Returns
`protected SDL_Window * `[`window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a9383029971853d1c18924d71aeb0b30a) | The actual SDL_Window object.
`protected uint32_t `[`flags`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1ac50c4ef112a5cb3e51147451e4451038) | The window's flags.
`protected int `[`width`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a5c3593b500b23e7caa7fab8be33fc9f5) | The window's width in pixels.
`protected int `[`height`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1ac05ed1f5db515ce709c038130c06ae3a) | The window's height in pixels.
`protected const char * `[`title`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1ac7deb99f097cae43dfac4ba664b0b71e) | The window's title.

## Members

#### `public RX_API `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a605d01de99b761411a51560f659b5ccd)`(int width,int height,const char * title,uint32_t flags) = default` 

#### Parameters
* `width` The width of the window in pixels. 

* `height` The height of the window in pixels. 

* `title` The title of the window shown in the title bar. 

* `flags` The window flags (no additional flags if not specified).

#### `public virtual RX_API `[`~Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a4aabfbbcd46b15e96661e5990704ed75)`()` 

Destructor of [Window](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window).

Will also clean up the object's resources. 
**See also**: [clean()](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a18c7d4abe800472a8e79bd4f18cfb63e)

#### `public  `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a3de9e4815cc97d30787058b683f80c56)`(const `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window)` &) = delete` 

#### `public  `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1ae6072e2dfb9cb8d2738befb1bf4fcc0f)`(const `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1adfbf2437f3931c8b235783466ee61230)`(const `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window)` &) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a7f1b2cdbdec544d883be6d7f5ee02fcc)`(const `[`Window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window)` &&) = delete` 

#### `public virtual RX_API auto `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a1465803d1631aeb85f3c304617d7f2c7)`()` 

Initializes the SDL-window.

#### `public virtual RX_API auto `[`update`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1aa038bb7bf14b31b0d1c3cff6daf07285)`()` 

Updates window-related components.

In case the user wants to handle input events, this function should be overwritten in the inherited class. The user has to call [Window::update()](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1aa038bb7bf14b31b0d1c3cff6daf07285) to ensure proper functionality.

#### `public virtual RX_API void `[`clean`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a18c7d4abe800472a8e79bd4f18cfb63e)`()` 

Destroys the window.

#### `public RX_API void `[`resize`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1ab051017a633806871575e8ec129cfd8a)`(int width,int height)` 

Resizes the window.

#### Parameters
* `width` The desired width in pixels. 

* `height` The desired height in pixels.

#### `public inline RX_API auto `[`get`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a2499009d44ee72861fccea9923a69f04)`()` 

#### Returns
Returns the actual SDL_Window object.

#### `public RX_API auto `[`getExtent`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1af9cf0260c8125403ab94097965ef2955)`() const` 

#### Returns
Returns the window size as a Vulkan 2D extent.

#### `public inline auto `[`getWidth`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a3493050f0c87cc92dd5367086fa4232d)`() const` 

#### Returns
Returns the window's width in pixels.

#### `public inline auto `[`getHeight`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1acb5f534b676c8cd3d640f47ac4444499)`() const` 

#### Returns
Returns the window's height in pixels.

#### `public RX_API auto `[`changed`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a3bf6eadc1f1aa70f3335d505edf30ff8)`()` 

#### Returns
Returns true if the window size has changed.

#### `public RX_API auto `[`minimized`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a073466b019e4961a3823a50019f66776)`()` 

#### Returns
Returns true if the window is minimized.

#### `protected SDL_Window * `[`window`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a9383029971853d1c18924d71aeb0b30a) 

The actual SDL_Window object.

#### `protected uint32_t `[`flags`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1ac50c4ef112a5cb3e51147451e4451038) 

The window's flags.

#### `protected int `[`width`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1a5c3593b500b23e7caa7fab8be33fc9f5) 

The window's width in pixels.

#### `protected int `[`height`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1ac05ed1f5db515ce709c038130c06ae3a) 

The window's height in pixels.

#### `protected const char * `[`title`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1ac7deb99f097cae43dfac4ba664b0b71e) 

The window's title.

# class `RAYEXEC_NAMESPACE::RayExec` 

The main user interface.

This class provides everything to set up a main loop and fill the scene with geometry and light sources. Furthermore, the renderer's settings can be changed to better fit the user's purpose. 

Example
```cpp
// The following example renders a cube in less than 10 lines of code.
RayExec myRenderer;
myRenderer.init( );

myRenderer.pushNode( std::make_shared<GeometryNode>( "models/cube.obj", Material( "textures/metal.png" ) ) );

while ( myRenderer.isRunning( ) )
{
  myRenderer.run( );
}
```

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Settings`](moxygen/api-undefined.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_settings)` `[`settings`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1ab71c208dbdcdf6cc03d962c95ce3e1d4) | Used to modify any interal rendering settings.
`public RX_API void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a4a77cb838d74cbf29b35e41733f702a9)`()` | Initializes the renderer.
`public RX_API void `[`run`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a4364aeb145d7ae3685cac2d126d1d304)`()` | A single function to execute all subcomponents in order.
`public RX_API auto `[`isRunning`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1af3094ea443ca1f685d3888cca6a51ff9)`() const` | #### Returns
`public RX_API void `[`setCamera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a41b6ca1d0db7fdb06806a6ce97beae84)`(std::shared_ptr< `[`Camera`](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera)` > camera)` | Used to set a custom camera.
`public RX_API void `[`setWindow`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1ab632b1740be14c4ea543d2d58dcd7bd3)`(std::shared_ptr< `[`Window`](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window)` > window)` | Used to set a custom window.
`public RX_API void `[`setGui`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a42df80fe179d395b350eddcd015ddc88)`(std::shared_ptr< `[`Gui`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui)` > gui)` | Used to set a custom GUI.
`public RX_API void `[`submitGeometryInstance`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a3ef30ee07220e343ca8a9d88b7734f3c)`(std::shared_ptr< `[`GeometryInstance`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` > geometryInstance)` | Used to submit a geometry instance for rendering.
`public RX_API void `[`setGeometryInstances`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a2bff08870b1f64db618f69c737dba40f)`(const std::vector< std::shared_ptr< `[`GeometryInstance`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` >> & geometryInstances)` | Used to submit multiple geometry instances for rendering, replacing all existing instances.
`public RX_API void `[`removeGeometryInstance`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1afc92af88a74dab31911a9b88a818dc61)`(std::shared_ptr< `[`GeometryInstance`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` > geometryInstance)` | Used to remove a geometry instance.
`public RX_API void `[`submitGeometry`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1ae64ec0603f573c599fbe2d4312736912)`(std::shared_ptr< `[`Geometry`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry)` > geometry)` | Used to submit a geometry and set up its buffers.
`public RX_API void `[`setGeometries`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a82259ba090dbf6319e92dee23b6b5b3d)`(const std::vector< std::shared_ptr< `[`Geometry`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry)` >> & geometries)` | Used to submit multiple geometries and set up their buffers.
`public RX_API auto `[`findGeometry`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a0a44717cc13a7b840c806049e7560705)`(std::string_view path) const` | Used to retrieve a geoemtry based on its path.
`public inline RX_API auto `[`getWindow`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a8b37457e5868d51ca5183b176a9ec5dd)`() const` | #### Returns
`public inline RX_API auto `[`getCamera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1aa6f5eea39b1abc9b0c3ba0ee16dc8e13)`() const` | #### Returns

## Members

#### `public `[`Settings`](moxygen/api-undefined.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_settings)` `[`settings`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1ab71c208dbdcdf6cc03d962c95ce3e1d4) 

Used to modify any interal rendering settings.

**See also**: [RAYEXEC_NAMESPACE::Settings](moxygen/api-undefined.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_settings)

#### `public RX_API void `[`init`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a4a77cb838d74cbf29b35e41733f702a9)`()` 

Initializes the renderer.

This function initializes the window subcomponent as well as the API.

#### `public RX_API void `[`run`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a4364aeb145d7ae3685cac2d126d1d304)`()` 

A single function to execute all subcomponents in order.

This function updates the window and the camera components and calls the update and render functions of the API. 
**See also**: [RAYEXEC_NAMESPACE::Camera::update()](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera_1a17a52094a998d73590caa62e219064b1), [RAYEXEC_NAMESPACE::Window::update()](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window_1aa038bb7bf14b31b0d1c3cff6daf07285), RAYEXEC_NAMESPACE::Api::update(), RAYEXEC_NAMESPACE::Api::render()

#### `public RX_API auto `[`isRunning`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1af3094ea443ca1f685d3888cca6a51ff9)`() const` 

#### Returns
Returns true if the application is still running and false if the application has stopped.

#### `public RX_API void `[`setCamera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a41b6ca1d0db7fdb06806a6ce97beae84)`(std::shared_ptr< `[`Camera`](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera)` > camera)` 

Used to set a custom camera.

#### Parameters
* `camera` A pointer to a [RAYEXEC_NAMESPACE::Camera](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_camera) object.

#### `public RX_API void `[`setWindow`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1ab632b1740be14c4ea543d2d58dcd7bd3)`(std::shared_ptr< `[`Window`](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window)` > window)` 

Used to set a custom window.

#### Parameters
* `window` A pointer to a [RAYEXEC_NAMESPACE::Window](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_window) object.

#### `public RX_API void `[`setGui`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a42df80fe179d395b350eddcd015ddc88)`(std::shared_ptr< `[`Gui`](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui)` > gui)` 

Used to set a custom GUI.

The GUI can be changed multiple times. Even during runtime. 
#### Parameters
* `gui` A pointer to a [RAYEXEC_NAMESPACE::Gui](moxygen/api-API.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_gui) object.

#### `public RX_API void `[`submitGeometryInstance`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a3ef30ee07220e343ca8a9d88b7734f3c)`(std::shared_ptr< `[`GeometryInstance`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` > geometryInstance)` 

Used to submit a geometry instance for rendering.

#### Parameters
* `geometryInstance` The instance to queue for rendering. 

This function does not invoke any draw calls.

#### `public RX_API void `[`setGeometryInstances`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a2bff08870b1f64db618f69c737dba40f)`(const std::vector< std::shared_ptr< `[`GeometryInstance`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` >> & geometryInstances)` 

Used to submit multiple geometry instances for rendering, replacing all existing instances.

#### Parameters
* `geometryInstances` The instances to queue for rendering. 

This function does not invoke any draw calls.

#### `public RX_API void `[`removeGeometryInstance`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1afc92af88a74dab31911a9b88a818dc61)`(std::shared_ptr< `[`GeometryInstance`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` > geometryInstance)` 

Used to remove a geometry instance.

Once a geometry instance was removed, it will no longer be rendered. 
#### Parameters
* `geometryInstance` The instance to remove.

#### `public RX_API void `[`submitGeometry`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1ae64ec0603f573c599fbe2d4312736912)`(std::shared_ptr< `[`Geometry`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry)` > geometry)` 

Used to submit a geometry and set up its buffers.

Once a geometry was submitted, geometry instances referencing this particular geometry can be drawn. 
#### Parameters
* `geometry` The geometry to submit.

#### `public RX_API void `[`setGeometries`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a82259ba090dbf6319e92dee23b6b5b3d)`(const std::vector< std::shared_ptr< `[`Geometry`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry)` >> & geometries)` 

Used to submit multiple geometries and set up their buffers.

Once a geometry was submitted, geometry instances referencing this particular geometry can be drawn. 
#### Parameters
* `geometries` The geometries to submit.

#### `public RX_API auto `[`findGeometry`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a0a44717cc13a7b840c806049e7560705)`(std::string_view path) const` 

Used to retrieve a geoemtry based on its path.

#### Parameters
* `path` The geometry's model's path, relative to the path to assets.

#### `public inline RX_API auto `[`getWindow`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1a8b37457e5868d51ca5183b176a9ec5dd)`() const` 

#### Returns
Returns a pointer to the renderer's window.

#### `public inline RX_API auto `[`getCamera`](#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec_1aa6f5eea39b1abc9b0c3ba0ee16dc8e13)`() const` 

#### Returns
Returns a pointer to the renderer's camera.

# struct `RAYEXEC_NAMESPACE::Material` 

Describes the rendering properties of a mesh.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::vec4 `[`ambient`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_material_1a7471265ce8bdb979bab3c4528536e199) | 
`public glm::vec4 `[`diffuse`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_material_1aff50515831c1f24beef8572e72c84de7) | 
`public glm::vec4 `[`specular`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_material_1a9f58d236deb9cad8e93cf6d8a9d74a85) | 

## Members

#### `public glm::vec4 `[`ambient`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_material_1a7471265ce8bdb979bab3c4528536e199) 

#### `public glm::vec4 `[`diffuse`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_material_1aff50515831c1f24beef8572e72c84de7) 

#### `public glm::vec4 `[`specular`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_material_1a9f58d236deb9cad8e93cf6d8a9d74a85) 

# struct `RAYEXEC_NAMESPACE::Mesh` 

Describes a sub-mesh and its material.

If indexOffset is not set correctly the mesh can not be displayed properly. Take a look at [loadObj(std::string_view)](moxygen/api-undefined.md#group___base_1ga02a95a02aa5d57299c4eecb3c6bb4d3d) for a working example.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Material`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_material)` `[`material`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_mesh_1a79b2065d2745cf71155866a2ee66fd5f) | The mesh's material.
`public uint32_t `[`indexOffset`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_mesh_1ae2b89db86a0ec2ad689f24a51bd40825) | Refers to the offset of this mesh inside a [Geometry::indices](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_1aa2bba9de381e5d1ffd028d41a2649368) container.

## Members

#### `public `[`Material`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_material)` `[`material`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_mesh_1a79b2065d2745cf71155866a2ee66fd5f) 

The mesh's material.

#### `public uint32_t `[`indexOffset`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_mesh_1ae2b89db86a0ec2ad689f24a51bd40825) 

Refers to the offset of this mesh inside a [Geometry::indices](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_1aa2bba9de381e5d1ffd028d41a2649368) container.

# struct `RAYEXEC_NAMESPACE::Geometry` 

Describes a geometry [RayExec](moxygen/api-Base.md#class_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_ray_exec) can render.

Even if a model consists out of multiple sub-meshes, all vertices and indices must be stored together in their respective containers. geometryIndex must be incremented everytime a new model is created.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex)` > `[`vertices`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_1a9c8a6ea09b0072f363a000e383a79451) | Contains all vertices of the geometry.
`public std::vector< uint32_t > `[`indices`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_1aa2bba9de381e5d1ffd028d41a2649368) | Contains all indices of the geometry.
`public std::vector< `[`Mesh`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_mesh)` > `[`meshes`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_1a445e952a68b533af348c193650362ed4) | Contains all sub-meshes and their respective materials.
`public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_1ad22d12e756c7f16bf0462bdbf600a38d) | A unique index required by the acceleration structures.
`public std::string_view `[`path`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_1a43c7a0b13af3e5416dec4c6caf4a0dcb) | The model's path, relative to the path to assets.

## Members

#### `public std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_vertex)` > `[`vertices`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_1a9c8a6ea09b0072f363a000e383a79451) 

Contains all vertices of the geometry.

#### `public std::vector< uint32_t > `[`indices`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_1aa2bba9de381e5d1ffd028d41a2649368) 

Contains all indices of the geometry.

#### `public std::vector< `[`Mesh`](moxygen/api-Base.md#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_mesh)` > `[`meshes`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_1a445e952a68b533af348c193650362ed4) 

Contains all sub-meshes and their respective materials.

#### `public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_1ad22d12e756c7f16bf0462bdbf600a38d) 

A unique index required by the acceleration structures.

#### `public std::string_view `[`path`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_1a43c7a0b13af3e5416dec4c6caf4a0dcb) 

The model's path, relative to the path to assets.

# struct `RAYEXEC_NAMESPACE::GeometryInstance` 

Describes an instance of some geometry.

To assign a specific geometry to an instance, both must have the same value for geometryIndex.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::mat4 `[`transform`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a2dc80022089dd5b7f0fdd06c1d05b33b) | The instance's world transform matrix.
`public glm::mat4 `[`transformIT`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a768f40a8f4558235b01b43c486da1973) | The inverse transpose of transform.
`public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1ab288a423b9a4441877eb0811dcd3bb80) | Used to assign this instance a model.

## Members

#### `public glm::mat4 `[`transform`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a2dc80022089dd5b7f0fdd06c1d05b33b) 

The instance's world transform matrix.

#### `public glm::mat4 `[`transformIT`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a768f40a8f4558235b01b43c486da1973) 

The inverse transpose of transform.

#### `public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x_e_c___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1ab288a423b9a4441877eb0811dcd3bb80) 

Used to assign this instance a model.

