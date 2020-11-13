# group `BASE` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public RX_API std::shared_ptr< Geometry > `[`loadObj`](#group___b_a_s_e_1gac348076d8b31be1511d1fc0532e75747)`(std::string_view path)`            | A commodity function for loading a wavefront (.obj) model file and allocate a geometry object from it.
`public RX_API std::shared_ptr< GeometryInstance > `[`instance`](#group___b_a_s_e_1gad880ea304e8ff3dab6ea5258df8970cf)`(std::shared_ptr< Geometry > geometry,const glm::mat4 & transform)`            | A commodity function for allocating an instance from a given geometry and set its matrices.
`public RX_API auto `[`directionalLightInstance`](#group___b_a_s_e_1ga2a069b2d151c07c55325dc008f693cb0)`(const glm::vec3 & direction)`            | A commodity function for allocating a directional light instance.
`public RX_API auto `[`pointLightInstance`](#group___b_a_s_e_1gace95101543a8d8577781044c96795140)`(const glm::vec3 & position)`            | A commodity function for allocating a point light instance.
`class `[`RAYEX_NAMESPACE::Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera) | A minimal camera implementation.
`class `[`RAYEX_NAMESPACE::Scene`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene) | Stores all geoemtry, geometry instances and light sources.
`class `[`RAYEX_NAMESPACE::Settings`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings) | Exposes all graphic settings supported by the renderer.
`class `[`RAYEX_NAMESPACE::Time`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_time) | Used to keep track of the application's timing.
`class `[`RAYEX_NAMESPACE::Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window) | Implements a SDL-based window.
`class `[`RAYEX_NAMESPACE::Rayex`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex) | The main user interface.
`struct `[`RAYEX_NAMESPACE::Material`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material) | Describes the rendering properties of a mesh.
`struct `[`RAYEX_NAMESPACE::Mesh`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh) | Describes a sub-mesh and its material.
`struct `[`RAYEX_NAMESPACE::Geometry`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry) | Describes a geometry [Rayex](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex) can render.
`struct `[`RAYEX_NAMESPACE::GeometryInstance`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance) | Describes an instance of some geometry.
`struct `[`RAYEX_NAMESPACE::Light`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light) | A common base for all light types.
`struct `[`RAYEX_NAMESPACE::DirectionalLight`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light) | A directional light.
`struct `[`RAYEX_NAMESPACE::PointLight`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light) | A point light.

## Members

#### `public RX_API std::shared_ptr< Geometry > `[`loadObj`](#group___b_a_s_e_1gac348076d8b31be1511d1fc0532e75747)`(std::string_view path)` 

A commodity function for loading a wavefront (.obj) model file and allocate a geometry object from it.

The function will attempt to find sub-meshes in the file and retrieve all materials. A user is encouraged to create their own model loader function or classes. 
#### Parameters
* `path` The model's path, relative to the path to assets. 

#### Returns
Returns a pointer to a geometry object.

#### `public RX_API std::shared_ptr< GeometryInstance > `[`instance`](#group___b_a_s_e_1gad880ea304e8ff3dab6ea5258df8970cf)`(std::shared_ptr< Geometry > geometry,const glm::mat4 & transform)` 

A commodity function for allocating an instance from a given geometry and set its matrices.

The function will also automatically set the inverse transpose matrix. 
#### Parameters
* `geometry` The geometry to create the instance from. 

* `transform` The world transform matrix of the instance. 

#### Returns
Returns a pointer to a geometry instance.

#### `public RX_API auto `[`directionalLightInstance`](#group___b_a_s_e_1ga2a069b2d151c07c55325dc008f693cb0)`(const glm::vec3 & direction)` 

A commodity function for allocating a directional light instance.

#### Parameters
* `direction` The directional lights' direction. 

#### Returns
Returns a pointer to a directional light instance.

#### `public RX_API auto `[`pointLightInstance`](#group___b_a_s_e_1gace95101543a8d8577781044c96795140)`(const glm::vec3 & position)` 

A commodity function for allocating a point light instance.

#### Parameters
* `position` The point lights' position. 

#### Returns
Returns a pointer to a point light instance.

# class `RAYEX_NAMESPACE::Camera` 

A minimal camera implementation.

This class acts like an interface for the user by providing the most important camera-related matrices as well as the camera's position, which are required by the rendering API. 

Example
```cpp
// This example requires the user to implement a custom camera class that inherits from Camera.
auto myCam = std::make_shared<CustomCamera>( 600, 500, glm::vec3{ 0.0f, 0.0f, 3.0f } );
```

The user has to handle keyboard related camera changes inside [update()](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a37c5fc1ac50d68f1494f90e5b2a569a9). 

Do not forget to re-calculate the view or projection matrix if the camera or the window have changed. See [updateViewMatrix()](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a874a4dac36c1a798d9f91927d56d4f00), [updateProjectionMatrix()](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a1897c6c6c9177d4b3c383ef8ce5ffeab) or updateView and updateProj respectively.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public bool `[`_updateView`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a0e1c547906bc7d47775e672804f895e3) | Keeps track of whether or not to udpate the view matrix.
`public bool `[`_updateProj`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a3fa6186059f5d85399d6b1c6e4303b03) | Keeps track of whether or not to udpate the projection matrix.
`public  `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ac2baf47b33fc4bb64bab2c9018388572)`(int width,int height,const glm::vec3 & position)` | #### Parameters
`public virtual  `[`~Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ae5ba5ab21b8be3e27f89f605e260cf7d)`() = default` | 
`public  `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a46421fbe0eaa8549c4610fd0a0668d05)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &) = default` | 
`public  `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a81848737678ca365e989e8f4e6585c2b)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a1d2cb96a89ed49bcf4fc3b1f103c8a0d)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &) = default` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a1211b4836b93378f4e90b1cf780cf0d7)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &&) = delete` | 
`public virtual void `[`update`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a37c5fc1ac50d68f1494f90e5b2a569a9)`()` | Is used to update camera vectors etc.
`public inline auto `[`getPosition`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1acfcf6d7449755de73dc3de2ea39bf12e)`() const` | #### Returns
`public void `[`setPosition`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a2dd871594937d080d24bb13eac812341)`(const glm::vec3 & position)` | Is used to change the camera's position.
`public void `[`setSize`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a544e60fd334db538c5dea452c57c94fb)`(int width,int height)` | Is used to set a size for the camera that fits the viewport dimensions.
`public void `[`setFov`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ab533363e7361b17dc13253fe224cd00e)`(float fov)` | Is used to set the camera's field of view.
`public void `[`setSensitivity`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a30ef8288accfbc9c43986e58ffeb2f16)`(float sensitivity)` | Is used to set the mouse sensitivity.
`public inline auto `[`getViewMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ab93f93f316bead7870322c0a6c7938d7)`() const` | #### Returns
`public inline auto `[`getProjectionMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a44a23ce5e54d9e67da5abfe53205882f)`() const` | #### Returns
`public inline auto `[`getViewInverseMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ad52515b2c20a16149c4d5652a3e0b11c)`() const` | #### Returns
`public inline auto `[`getProjectionInverseMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a428203c7dfd644026cc8cb6c94f5ed8a)`() const` | #### Returns
`public void `[`updateViewMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a874a4dac36c1a798d9f91927d56d4f00)`()` | Re-calculates the camera's view matrix as well as the inversed view matrix.
`public void `[`updateProjectionMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a1897c6c6c9177d4b3c383ef8ce5ffeab)`()` | Re-calculates the camera's projection matrix as well as the inversed projection matrix.
`public virtual void `[`processMouse`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a5c04a4f2e338a637c3ec45acdbf7dc50)`(float xOffset,float yOffset)` | Processes mouse input (default implementation).
`public inline virtual void `[`processKeyboard`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a16f11a54e2249f7b951db9c9a2a3c91f)`()` | Used to handle user-defined keyboard inputs or input events.
`protected int `[`_width`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a7e9b3a1a0f09cf18554db835ccf28f93) | The width of the viewport.
`protected int `[`_height`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aac3c7f080fd0661e2fdbe66388142fa2) | The height of the viewport.
`protected glm::vec3 `[`_position`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a832b7643b630190398a871cc897fba9e) | The camera's position.
`protected glm::mat4 `[`_view`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ac3a2c7df14d6db2ef72953c74dd756cb) | The view matrix.
`protected glm::mat4 `[`_projection`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a2ac8736247d2e36edfe4e7e2d6aecb15) | The projection matrix.
`protected glm::mat4 `[`_viewInverse`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a4293f3e4fee845435a318cf73c00a66b) | The view matrix inversed.
`protected glm::mat4 `[`_projectionInverse`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aef5d307749b930f0b4f325772363537b) | The projection matrix inversed.
`protected glm::vec3 `[`_worldUp`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a8889d943505d12a92d23ff22dc9c0ec2) | The world up vector.
`protected glm::vec3 `[`_up`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a137954d196ea4ccb344b022fd64e79e6) | The local up vector.
`protected glm::vec3 `[`_right`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aafcf6405638001f83fbff660114f3286) | The local right vector.
`protected glm::vec3 `[`_front`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aa85cc233a98dcf2fd2acbecb1886ad03) | The viewing direction.
`protected float `[`_yaw`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aba4b8adb31113415397e26d3fdd8815d) | The yaw (left and right).
`protected float `[`_pitch`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aa41d0eebc6b61e3727b425f78aaa3478) | The pitch (down and up).
`protected float `[`_sensitivity`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a11229bd8671fd515f1d53ebb58c45446) | The mouse sensitivity.
`protected float `[`_fov`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a3a5a0e5e97a893f3f189aaf8545ced4d) | The field of view.
`protected void `[`updateVectors`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a2711c78be2c02d3357e27277ba9c93de)`()` | Updates the camera vectors.

## Members

#### `public bool `[`_updateView`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a0e1c547906bc7d47775e672804f895e3) 

Keeps track of whether or not to udpate the view matrix.

#### `public bool `[`_updateProj`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a3fa6186059f5d85399d6b1c6e4303b03) 

Keeps track of whether or not to udpate the projection matrix.

#### `public  `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ac2baf47b33fc4bb64bab2c9018388572)`(int width,int height,const glm::vec3 & position)` 

#### Parameters
* `width` The width of the viewport. 

* `height` The height of the viewport. 

* `position` The position of your camera.

#### `public virtual  `[`~Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ae5ba5ab21b8be3e27f89f605e260cf7d)`() = default` 

#### `public  `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a46421fbe0eaa8549c4610fd0a0668d05)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &) = default` 

#### `public  `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a81848737678ca365e989e8f4e6585c2b)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a1d2cb96a89ed49bcf4fc3b1f103c8a0d)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &) = default` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a1211b4836b93378f4e90b1cf780cf0d7)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &&) = delete` 

#### `public virtual void `[`update`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a37c5fc1ac50d68f1494f90e5b2a569a9)`()` 

Is used to update camera vectors etc.

The user has to override this function for the camera to work like intended. The function will be called every tick.

#### `public inline auto `[`getPosition`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1acfcf6d7449755de73dc3de2ea39bf12e)`() const` 

#### Returns
Returns the camera's position.

#### `public void `[`setPosition`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a2dd871594937d080d24bb13eac812341)`(const glm::vec3 & position)` 

Is used to change the camera's position.

#### Parameters
* `position` The new camera position.

#### `public void `[`setSize`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a544e60fd334db538c5dea452c57c94fb)`(int width,int height)` 

Is used to set a size for the camera that fits the viewport dimensions.

#### Parameters
* `width` The width of the viewport. 

* `height` The height of the viewport.

#### `public void `[`setFov`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ab533363e7361b17dc13253fe224cd00e)`(float fov)` 

Is used to set the camera's field of view.

#### Parameters
* `fov` The new field of view.

#### `public void `[`setSensitivity`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a30ef8288accfbc9c43986e58ffeb2f16)`(float sensitivity)` 

Is used to set the mouse sensitivity.

#### Parameters
* `sensitivity` The new mouse sensitivity.

#### `public inline auto `[`getViewMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ab93f93f316bead7870322c0a6c7938d7)`() const` 

#### Returns
The view matrix.

#### `public inline auto `[`getProjectionMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a44a23ce5e54d9e67da5abfe53205882f)`() const` 

#### Returns
The projection matrix.

#### `public inline auto `[`getViewInverseMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ad52515b2c20a16149c4d5652a3e0b11c)`() const` 

#### Returns
The view matrix inversed.

#### `public inline auto `[`getProjectionInverseMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a428203c7dfd644026cc8cb6c94f5ed8a)`() const` 

#### Returns
The projection matrix inversed.

#### `public void `[`updateViewMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a874a4dac36c1a798d9f91927d56d4f00)`()` 

Re-calculates the camera's view matrix as well as the inversed view matrix.

#### `public void `[`updateProjectionMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a1897c6c6c9177d4b3c383ef8ce5ffeab)`()` 

Re-calculates the camera's projection matrix as well as the inversed projection matrix.

#### `public virtual void `[`processMouse`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a5c04a4f2e338a637c3ec45acdbf7dc50)`(float xOffset,float yOffset)` 

Processes mouse input (default implementation).

#### Parameters
* `xOffset` The difference of the current offset on the x-axis and the previous offset. 

* `yOffset` The difference of the current offset on the y-axis and the previous offset.

#### `public inline virtual void `[`processKeyboard`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a16f11a54e2249f7b951db9c9a2a3c91f)`()` 

Used to handle user-defined keyboard inputs or input events.

This function is called inside [update()](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a37c5fc1ac50d68f1494f90e5b2a569a9) and followed up by a call to [updateViewMatrix()](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a874a4dac36c1a798d9f91927d56d4f00) and does not have to be called by the user. Implementation of this function should be inside a user-defined inherited class.

#### `protected int `[`_width`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a7e9b3a1a0f09cf18554db835ccf28f93) 

The width of the viewport.

#### `protected int `[`_height`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aac3c7f080fd0661e2fdbe66388142fa2) 

The height of the viewport.

#### `protected glm::vec3 `[`_position`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a832b7643b630190398a871cc897fba9e) 

The camera's position.

#### `protected glm::mat4 `[`_view`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ac3a2c7df14d6db2ef72953c74dd756cb) 

The view matrix.

#### `protected glm::mat4 `[`_projection`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a2ac8736247d2e36edfe4e7e2d6aecb15) 

The projection matrix.

#### `protected glm::mat4 `[`_viewInverse`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a4293f3e4fee845435a318cf73c00a66b) 

The view matrix inversed.

#### `protected glm::mat4 `[`_projectionInverse`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aef5d307749b930f0b4f325772363537b) 

The projection matrix inversed.

#### `protected glm::vec3 `[`_worldUp`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a8889d943505d12a92d23ff22dc9c0ec2) 

The world up vector.

#### `protected glm::vec3 `[`_up`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a137954d196ea4ccb344b022fd64e79e6) 

The local up vector.

#### `protected glm::vec3 `[`_right`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aafcf6405638001f83fbff660114f3286) 

The local right vector.

#### `protected glm::vec3 `[`_front`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aa85cc233a98dcf2fd2acbecb1886ad03) 

The viewing direction.

#### `protected float `[`_yaw`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aba4b8adb31113415397e26d3fdd8815d) 

The yaw (left and right).

#### `protected float `[`_pitch`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aa41d0eebc6b61e3727b425f78aaa3478) 

The pitch (down and up).

#### `protected float `[`_sensitivity`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a11229bd8671fd515f1d53ebb58c45446) 

The mouse sensitivity.

#### `protected float `[`_fov`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a3a5a0e5e97a893f3f189aaf8545ced4d) 

The field of view.

#### `protected void `[`updateVectors`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a2711c78be2c02d3357e27277ba9c93de)`()` 

Updates the camera vectors.

Only needs to be called if mouse was moved.

# class `RAYEX_NAMESPACE::Scene` 

Stores all geoemtry, geometry instances and light sources.

Provides functions to change said data. 
> Todo: [removeGeometry()](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a8004b0c0dfb783070ea82af111378718)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public friend `[`Api`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1ae002d16199c05b908fbeecc4f7d97733) | 
`public friend `[`Rayex`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a03b7257e44db8e9af952484958a5346c) | 
`public auto `[`getGeometryInstances`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1aef804f5fc22e5d0b27530a00cd054060)`() const` | #### Returns
`public void `[`submitGeometryInstance`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a4ca1e82f10c27e433eff652710e8e700)`(std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` > geometryInstance)` | Used to submit a geometry instance for rendering.
`public void `[`setGeometryInstances`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a86b0c09d5294dabeeb550b84ef4ac0c4)`(const std::vector< std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` >> & geometryInstances)` | Used to submit multiple geometry instances for rendering, replacing all existing instances.
`public auto `[`getDirectionalLights`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1abd17c0c5eda2b09916ab74cd0c91794d)`() const` | #### Returns
`public void `[`submitDirectionalLight`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a0950f2d94e6e9fec8ae01200f001878e)`(std::shared_ptr< `[`DirectionalLight`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light)` > light)` | Used to submit a directional light.
`public void `[`removeDirectionalLight`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a8654b0489ad56a058af3647ab85c6095)`(std::shared_ptr< `[`DirectionalLight`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light)` > light)` | Used to remove a directional light.
`public auto `[`getPointLights`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1aefa4c39c0f32defc0e35019048b0bb17)`() const` | #### Returns
`public void `[`submitPointLight`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a2bf002f75dd17d8c28e0641e461ad2bd)`(std::shared_ptr< `[`PointLight`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light)` > light)` | Used to submit a point light.
`public void `[`removePointLight`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1aef891c0cb5abb107b620142ec9ccec00)`(std::shared_ptr< `[`PointLight`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light)` > light)` | Used to remove a point light.
`public void `[`removeGeometryInstance`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a11fc56eb116fe3301a59c5c594ee2bea)`(std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` > geometryInstance)` | Used to remove a geometry instance.
`public void `[`clearGeometryInstances`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a42dc12d8a200591532dc336f72a4847d)`()` | Used to remove all geometry instances.
`public void `[`popGeometryInstance`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a838076f7347dd4788ee1aaa8ffd28374)`()` | Used to remove the last geoemtry instance.
`public void `[`submitGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a53c0ef48ca0c5e0d2336869a8e75085b)`(std::shared_ptr< `[`Geometry`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry)` > geometry)` | Used to submit a geometry and set up its buffers.
`public void `[`setGeometries`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1ab191100612c6087204b476aadfd8b6db)`(const std::vector< std::shared_ptr< `[`Geometry`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry)` >> & geometries)` | Used to submit multiple geometries and set up their buffers.
`public void `[`removeGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a8004b0c0dfb783070ea82af111378718)`(std::shared_ptr< `[`Geometry`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry)` > geometry)` | Used to remove a geometry.
`public void `[`removeGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1adc0e77e152173899b4edf08fbe62ca18)`(uint32_t geometryIndex)` | Used to remove a geometry.
`public void `[`popGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1ae68fc066eddb4ff7ee2a16c26b06f5ba)`()` | Used to remove the last geometry and all its instances.
`public void `[`clearGeometries`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1ac89ff3616b43e6c2b7e6a23995784a53)`()` | Used to remove all geometries.
`public auto `[`findGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a344905cab1fbaf5a5585a6df25635bbc)`(std::string_view path) const` | Used to retrieve a geoemtry based on its path.

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

#### `public auto `[`getDirectionalLights`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1abd17c0c5eda2b09916ab74cd0c91794d)`() const` 

#### Returns
Returns all directional lights in the scene.

#### `public void `[`submitDirectionalLight`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a0950f2d94e6e9fec8ae01200f001878e)`(std::shared_ptr< `[`DirectionalLight`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light)` > light)` 

Used to submit a directional light.

#### Parameters
* `light` The directional light to submit.

#### `public void `[`removeDirectionalLight`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a8654b0489ad56a058af3647ab85c6095)`(std::shared_ptr< `[`DirectionalLight`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light)` > light)` 

Used to remove a directional light.

#### Parameters
* `light` The directional light to remove.

#### `public auto `[`getPointLights`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1aefa4c39c0f32defc0e35019048b0bb17)`() const` 

#### Returns
Returns all point lights in the scene.

#### `public void `[`submitPointLight`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a2bf002f75dd17d8c28e0641e461ad2bd)`(std::shared_ptr< `[`PointLight`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light)` > light)` 

Used to submit a point light.

#### Parameters
* `light` The point light to submit.

#### `public void `[`removePointLight`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1aef891c0cb5abb107b620142ec9ccec00)`(std::shared_ptr< `[`PointLight`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light)` > light)` 

Used to remove a point light.

#### Parameters
* `light` The point light to remove.

#### `public void `[`removeGeometryInstance`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a11fc56eb116fe3301a59c5c594ee2bea)`(std::shared_ptr< `[`GeometryInstance`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance)` > geometryInstance)` 

Used to remove a geometry instance.

Once a geometry instance was removed, it will no longer be rendered. 
#### Parameters
* `geometryInstance` The instance to remove.

#### `public void `[`clearGeometryInstances`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a42dc12d8a200591532dc336f72a4847d)`()` 

Used to remove all geometry instances.

However, geometries remain loaded and must be deleted explicitely.

#### `public void `[`popGeometryInstance`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene_1a838076f7347dd4788ee1aaa8ffd28374)`()` 

Used to remove the last geoemtry instance.

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

# class `RAYEX_NAMESPACE::Settings` 

Exposes all graphic settings supported by the renderer.

Any necessary pipeline recreations and swapchain recreations will not be performed at the point of calling any setter but instead the next time the renderer will be updated. Any function that sets the maximum of a given entity needs to be called before [RAYEX_NAMESPACE::Rayex::init()](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a7ab92b96ef9c93d84dced77b0ee2cc4b).

> Todo: Add a setUseTotalPathsOnly( bool flag )

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline auto `[`getRecursionDepth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a971ae0e67209a44e3ea350924ab22830)`() const` | #### Returns
`public void `[`setRecursionDepth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a2219f20482a7eca65a8f8cd819b75d45)`(uint32_t recursionDepth)` | Used to set the recursion depth.
`public inline auto `[`getClearColor`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a3fbc4ba4e79dcb84f8065d8657cd09d8)`() const` | #### Returns
`public void `[`setClearColor`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a7c23cd33060112e22df368eeb8fc4c62)`(const glm::vec4 & clearColor)` | Used to changed the clear color.
`public inline auto `[`getAssetsPath`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a8e94a1b47ab849117f0e97bf74332bb0)`() const` | #### Returns
`public void `[`setAssetsPath`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a00b2c779dbe061631b409240d99477fb)`(int argc,char * argv)` | Used to set a path to the directory containing all assets.
`public void `[`setAssetsPath`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a63ae460c13863b4d90fd673039ea6b11)`(std::string_view path)` | Used to set a path to the directory containing all assets.
`public inline auto `[`getRayTracingEnabled`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a2e953079038a847e3ed0cb77abb4e4fd)`() const` | #### Returns
`public void `[`setEnableRayTracing`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a86e1a739d623859c7a9f49334cc21400)`(bool flag)` | Used to enable or disable ray tracing.
`public void `[`setAutomaticPipelineRefresh`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ac444874c6f8feb0a7c4385bc17728176)`(bool flag)` | Used to toggle the automatic pipeline recreation.
`public void `[`setMaxDirectionalLights`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1aae6e3c01ef0d90c89c82c97c5eafb250)`(uint32_t amount)` | Used to set the maximum amount of directional lights that can be used.
`public void `[`setMaxPointLights`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1aa4f5d03705f7d75e752766f2136cff57)`(uint32_t amount)` | Used to set the maximum amount of point lights that can be used.
`public void `[`setMaxGeometryInstances`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ad82e2aa3d853d8b87408d808f36c72a4)`(uint32_t amount)` | Used to set the maximum amount of geometrys that can be used.
`public inline auto `[`getMaxRecursionDepth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a272c5032d8509892f7ee876d65fcdfe2)`() const` | #### Returns
`public void `[`setMaxGeoemtry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1adec13502be4c85e096764a102beb2c65)`(uint32_t amount)` | Used to set the maximum of geometry (models).
`public inline auto `[`getMaxGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1abd9b32871d745deb435e07f2b7006952)`() const` | #### Returns
`public void `[`setEnableJitterCam`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1aafdf9b286388d388b41afc6c4707cc02)`(bool flag)` | Used to toggle the jitter cam.
`public inline auto `[`getJitterCamEnabled`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a4fe503cf5a095b02606751381ea0538f)`() const` | #### Returns
`public void `[`setJitterCamSampleRate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a7c02b5affa6d65710d438a1300b369d6)`(uint32_t sampleRate)` | Used to set the jitter cam's sample rate.
`public inline auto `[`getJitterCamSampleRate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1abaab03bdd69de36f34d224933fe422c5)`() const` | #### Returns
`public void `[`setJitterCamSampleRatePerRayGen`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a6b646409950dbe255f929a0b8d07901a)`(uint32_t sampleRate)` | Used to set the jitter cam's sample rate per raygen.
`public inline auto `[`getJitterCamSampleRatePerRayGen`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ab25392e78df5f9d79224e5006bed0f12)`() const` | #### Returns
`public void `[`setEnableSsaa`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ae2aa8cc8634003231583eb3627d47de4)`(bool flag)` | Used to toggle SSAA.
`public inline auto `[`getSsaaEnabled`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1aa9344909d0bc98798245f5907e77cf96)`() const` | #### Returns
`public void `[`setSsaaSampleRate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a2f760bb865eeb7f541ff7954809a49e0)`(uint32_t sampleRate)` | Used to set the SSAA sample rate.
`public inline auto `[`getSsaaSampleRate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1af11c9ed9a5d84a89f2642b16702deaf1)`() const` | #### Returns

## Members

#### `public inline auto `[`getRecursionDepth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a971ae0e67209a44e3ea350924ab22830)`() const` 

#### Returns
Returns the recursion depth.

#### `public void `[`setRecursionDepth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a2219f20482a7eca65a8f8cd819b75d45)`(uint32_t recursionDepth)` 

Used to set the recursion depth.

The function will trigger a pipeline recreation as soon as possible unless it was explicitely disabled using [setAutomaticPipelineRefresh(bool)](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ac444874c6f8feb0a7c4385bc17728176). If a value higher than the device's maximum supported value is set, it will use the maximum value instead. 
#### Parameters
* `recursionDepth` The new value for the recursion depth.

#### `public inline auto `[`getClearColor`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a3fbc4ba4e79dcb84f8065d8657cd09d8)`() const` 

#### Returns
Returns the clear color.

#### `public void `[`setClearColor`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a7c23cd33060112e22df368eeb8fc4c62)`(const glm::vec4 & clearColor)` 

Used to changed the clear color.

The function will trigger a swapchain recreation as soon as possible unless it was explicitely disabled using [setAutomaticPipelineRefresh(bool)](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ac444874c6f8feb0a7c4385bc17728176). 
#### Parameters
* `clearColor` The new value for the clear color.

#### `public inline auto `[`getAssetsPath`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a8e94a1b47ab849117f0e97bf74332bb0)`() const` 

#### Returns
Returns the path to assets.

#### `public void `[`setAssetsPath`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a00b2c779dbe061631b409240d99477fb)`(int argc,char * argv)` 

Used to set a path to the directory containing all assets.

This path should contain all models, textures and shaders. 
#### Parameters
* `argc` The argc parameter that can be retrieved from the main-function's parameters. 

* `argv` The argv parameter that can be retrieved from the main-function's parameters.

#### `public void `[`setAssetsPath`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a63ae460c13863b4d90fd673039ea6b11)`(std::string_view path)` 

Used to set a path to the directory containing all assets.

This path should contain all models, textures and shaders. 
#### Parameters
* `path` The path to assets.

#### `public inline auto `[`getRayTracingEnabled`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a2e953079038a847e3ed0cb77abb4e4fd)`() const` 

#### Returns
Returns true if ray tracing is enabled and false if rasterization is enabled. 

> Todo: This function will be pointless once the new pipeline system is implemented.

#### `public void `[`setEnableRayTracing`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a86e1a739d623859c7a9f49334cc21400)`(bool flag)` 

Used to enable or disable ray tracing.

The function will trigger a swapchain recreation as soon as possible unless it was explicitely disabled using [setAutomaticPipelineRefresh(bool)](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ac444874c6f8feb0a7c4385bc17728176). 
#### Parameters
* `flag` If false, ray tracing will be disabled. 

> Todo: This function will be pointless once the new pipeline system is implemented.

#### `public void `[`setAutomaticPipelineRefresh`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ac444874c6f8feb0a7c4385bc17728176)`(bool flag)` 

Used to toggle the automatic pipeline recreation.

#### Parameters
* `flag` If false, the pipelines will not be recreated automatically until this function is called with true.

#### `public void `[`setMaxDirectionalLights`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1aae6e3c01ef0d90c89c82c97c5eafb250)`(uint32_t amount)` 

Used to set the maximum amount of directional lights that can be used.

#### Parameters
* `amount` The amount of maximum directional lights.

#### `public void `[`setMaxPointLights`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1aa4f5d03705f7d75e752766f2136cff57)`(uint32_t amount)` 

Used to set the maximum amount of point lights that can be used.

#### Parameters
* `amount` The amount of maximum point lights.

#### `public void `[`setMaxGeometryInstances`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ad82e2aa3d853d8b87408d808f36c72a4)`(uint32_t amount)` 

Used to set the maximum amount of geometrys that can be used.

#### Parameters
* `amount` The amount of maximum geometrys.

#### `public inline auto `[`getMaxRecursionDepth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a272c5032d8509892f7ee876d65fcdfe2)`() const` 

#### Returns
Returns the maximum recursion depth on the GPU.

#### `public void `[`setMaxGeoemtry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1adec13502be4c85e096764a102beb2c65)`(uint32_t amount)` 

Used to set the maximum of geometry (models).

Try to keep this as small as possible, as this affects performance. 
#### Parameters
* `amount` The amount of maximum geometry.

#### `public inline auto `[`getMaxGeometry`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1abd9b32871d745deb435e07f2b7006952)`() const` 

#### Returns
Returns the maximum amount of geometry.

#### `public void `[`setEnableJitterCam`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1aafdf9b286388d388b41afc6c4707cc02)`(bool flag)` 

Used to toggle the jitter cam.

A jitter cam can be used for anti aliasing a static scene. A ray that will be cast into the scene normally starts off at the exact same position. The jitter cam will jitter or scatter the ray's starting position slightly using random values. 
#### Parameters
* `flag` Enable or disable the jitter cam. 

Disables SSAA automatically.

#### `public inline auto `[`getJitterCamEnabled`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a4fe503cf5a095b02606751381ea0538f)`() const` 

#### Returns
Returns true, if jitter cam is enabled.

#### `public void `[`setJitterCamSampleRate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a7c02b5affa6d65710d438a1300b369d6)`(uint32_t sampleRate)` 

Used to set the jitter cam's sample rate.

#### Parameters
* `sampleRate` The jitter cam's sample rate. 

**See also**: [setJitterCamSampleRatePerRayGen(uint32_t)](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a6b646409950dbe255f929a0b8d07901a)

#### `public inline auto `[`getJitterCamSampleRate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1abaab03bdd69de36f34d224933fe422c5)`() const` 

#### Returns
Returns the jitter cam's sample rate .

#### `public void `[`setJitterCamSampleRatePerRayGen`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a6b646409950dbe255f929a0b8d07901a)`(uint32_t sampleRate)` 

Used to set the jitter cam's sample rate per raygen.

#### Parameters
* `sampleRate` Used to make the jitter cam faster by casting n rays directly in the raygen shader. 

It is advised to set a higher value for sampleRatePerRayGen and lowering sampleRate instead. 

**See also**: setJitterCamSamplesRate(uint32_t)

#### `public inline auto `[`getJitterCamSampleRatePerRayGen`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ab25392e78df5f9d79224e5006bed0f12)`() const` 

#### Returns
Returns the jitter cam's sample rate per raygen.

#### `public void `[`setEnableSsaa`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ae2aa8cc8634003231583eb3627d47de4)`(bool flag)` 

Used to toggle SSAA.

#### Parameters
* `flag` If true, SSAA will be activated. 

Disables jitter cam automatically.

#### `public inline auto `[`getSsaaEnabled`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1aa9344909d0bc98798245f5907e77cf96)`() const` 

#### Returns
Returns true, if SSAA is enabled.

#### `public void `[`setSsaaSampleRate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a2f760bb865eeb7f541ff7954809a49e0)`(uint32_t sampleRate)` 

Used to set the SSAA sample rate.

#### Parameters
* `sampleRate` The desired SSAA sample rate. If this value is zero, then the sample rate will be set to one.

#### `public inline auto `[`getSsaaSampleRate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1af11c9ed9a5d84a89f2642b16702deaf1)`() const` 

#### Returns
Returns the SSAA sample rate.

# class `RAYEX_NAMESPACE::Time` 

Used to keep track of the application's timing.

> Todo: Average FPS are pointless. Implement minimum FPS and frametimes instead.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public friend `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_time_1a626c5328d806f10740e91a064ceb44a2) | 

## Members

#### `public friend `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_time_1a626c5328d806f10740e91a064ceb44a2) 

# class `RAYEX_NAMESPACE::Window` 

Implements a SDL-based window.

In order to handle events the user has to create a class that inherits from [Window](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window). 

Example
```cpp
// This example requires the user to implement a custom window class that inherits from Window.
auto myWindow = std::make_shared<CustomWindow>( width, height, "Example", WINDOW_RESIZABLE | WINDOW_INPUT_FOCUS );
```

**See also**: [update()](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ab3e217cf1525ce607cb4e99d762359c1)

> Todo: To avoid the user having to link to SDL themselves requires to write a complete SDL2 wrapper.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a405832ffcf6c44daefb0428d7eff3cd1)`(int width,int height,const char * title,uint32_t flags) = default` | #### Parameters
`public virtual  `[`~Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a274c789052aedf9061232524f5eb3e7a)`()` | Destructor of [Window](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window).
`public  `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a908c6b6221e0f110467fd265f23a0097)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &) = delete` | 
`public  `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ad4d23db965bedb12755ad8d0addbcfe4)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a3b2477e338e063b1cfe3b1764a7f9de5)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1abbc5d180aaaa9c97563f5563ab0e884e)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &&) = delete` | 
`public virtual auto `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a1fd34359d64d769e18cd165520b9cdf1)`()` | Initializes the SDL-window.
`public virtual auto `[`update`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ab3e217cf1525ce607cb4e99d762359c1)`()` | Updates window-related components.
`public virtual void `[`clean`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ac90e67dfedfb9e6c4f3f7aa94572216a)`()` | Destroys the window.
`public void `[`resize`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a7c0175db4a28dfe01592d44dca4e8173)`(int width,int height)` | Resizes the window.
`public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ad1b38521eec514c9fab34c8debb99e74)`()` | #### Returns
`public auto `[`getExtent`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1aa3c82f35c8fb88146193b06f4c4bf7cc)`() const` | #### Returns
`public inline auto `[`getWidth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a26db3cec4e76fe4092c4fbdac20aea33)`() const` | #### Returns
`public inline auto `[`getHeight`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a35cf7ab1da6f4fb21dd03cd67e05715a)`() const` | #### Returns
`public auto `[`changed`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a01cf9dc8e65799fce5fe7789aa53fdbb)`()` | #### Returns
`public auto `[`minimized`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ae2a734e87d0856b5dbff63387576b5f5)`()` | #### Returns
`public auto `[`getExtensions`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a202e93a39fb804179da009f60003fc80)`() const` | #### Returns
`protected SDL_Window * `[`_window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a6081e88e96a2b2fe645bcf25d86a7b8a) | The actual SDL_Window object.
`protected uint32_t `[`_flags`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a4ab26c8f1916d145a575d6b11103455f) | The window's flags.
`protected int `[`_width`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1acfb3fe0d13d31122e4c805c520a6b630) | The window's width in pixels.
`protected int `[`_height`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a09ca97a63cd6c046a90213188c128a8d) | The window's height in pixels.
`protected const char * `[`_title`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a3af3d62580ab8f937acc0f4b102305a4) | The window's title.

## Members

#### `public  `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a405832ffcf6c44daefb0428d7eff3cd1)`(int width,int height,const char * title,uint32_t flags) = default` 

#### Parameters
* `width` The width of the window in pixels. 

* `height` The height of the window in pixels. 

* `title` The title of the window shown in the title bar. 

* `flags` The window flags (no additional flags if not specified).

#### `public virtual  `[`~Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a274c789052aedf9061232524f5eb3e7a)`()` 

Destructor of [Window](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window).

Will also clean up the object's resources. 
**See also**: [clean()](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ac90e67dfedfb9e6c4f3f7aa94572216a)

#### `public  `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a908c6b6221e0f110467fd265f23a0097)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &) = delete` 

#### `public  `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ad4d23db965bedb12755ad8d0addbcfe4)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a3b2477e338e063b1cfe3b1764a7f9de5)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1abbc5d180aaaa9c97563f5563ab0e884e)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &&) = delete` 

#### `public virtual auto `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a1fd34359d64d769e18cd165520b9cdf1)`()` 

Initializes the SDL-window.

#### `public virtual auto `[`update`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ab3e217cf1525ce607cb4e99d762359c1)`()` 

Updates window-related components.

In case the user wants to handle input events, this function should be overwritten in the inherited class. The user has to call [Window::update()](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ab3e217cf1525ce607cb4e99d762359c1) to ensure proper functionality.

#### `public virtual void `[`clean`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ac90e67dfedfb9e6c4f3f7aa94572216a)`()` 

Destroys the window.

#### `public void `[`resize`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a7c0175db4a28dfe01592d44dca4e8173)`(int width,int height)` 

Resizes the window.

#### Parameters
* `width` The desired width in pixels. 

* `height` The desired height in pixels.

#### `public inline auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ad1b38521eec514c9fab34c8debb99e74)`()` 

#### Returns
Returns the actual SDL_Window object.

#### `public auto `[`getExtent`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1aa3c82f35c8fb88146193b06f4c4bf7cc)`() const` 

#### Returns
Returns the window size as a Vulkan 2D extent.

#### `public inline auto `[`getWidth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a26db3cec4e76fe4092c4fbdac20aea33)`() const` 

#### Returns
Returns the window's width in pixels.

#### `public inline auto `[`getHeight`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a35cf7ab1da6f4fb21dd03cd67e05715a)`() const` 

#### Returns
Returns the window's height in pixels.

#### `public auto `[`changed`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a01cf9dc8e65799fce5fe7789aa53fdbb)`()` 

#### Returns
Returns true if the window size has changed.

#### `public auto `[`minimized`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ae2a734e87d0856b5dbff63387576b5f5)`()` 

#### Returns
Returns true if the window is minimized.

#### `public auto `[`getExtensions`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a202e93a39fb804179da009f60003fc80)`() const` 

#### Returns
Returns the Vulkan extensions required by the SDL window.

#### `protected SDL_Window * `[`_window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a6081e88e96a2b2fe645bcf25d86a7b8a) 

The actual SDL_Window object.

#### `protected uint32_t `[`_flags`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a4ab26c8f1916d145a575d6b11103455f) 

The window's flags.

#### `protected int `[`_width`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1acfb3fe0d13d31122e4c805c520a6b630) 

The window's width in pixels.

#### `protected int `[`_height`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a09ca97a63cd6c046a90213188c128a8d) 

The window's height in pixels.

#### `protected const char * `[`_title`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a3af3d62580ab8f937acc0f4b102305a4) 

The window's title.

# class `RAYEX_NAMESPACE::Rayex` 

The main user interface.

This class provides everything to set up a main loop and fill the scene with geometry and light sources. Furthermore, the renderer's settings can be changed to better fit the user's purpose. 

Example
```cpp
// The following example renders a cube in less than 10 lines of code.
Rayex myRenderer;
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
`public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a7ab92b96ef9c93d84dced77b0ee2cc4b)`()` | Initializes the renderer.
`public void `[`run`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a8939d31603f84682c2309b784b3a82ac)`()` | A single function to execute all subcomponents in order.
`public auto `[`isRunning`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a3a1d0e4831044f2f7708365c984902bc)`() const` | #### Returns
`public void `[`setCamera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a0f4a81c790bed2dc5243288d9f9e57b4)`(std::shared_ptr< `[`Camera`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` > camera)` | Used to set a custom camera.
`public void `[`setWindow`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a364c4dcdb67baf57b1591394386ec3af)`(std::shared_ptr< `[`Window`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` > window)` | Used to set a custom window.
`public void `[`setGui`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a9c80416850a65f7a7968f58a9b8666c3)`(std::shared_ptr< `[`Gui`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui)` > gui)` | Used to set a custom GUI.
`public inline auto `[`getWindow`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a6a3eaad6809863354889f00ad3972b07)`() const` | #### Returns
`public inline auto `[`getCamera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a46b366bc4157a8cdc31738a505eabbbe)`() const` | #### Returns
`public inline auto `[`settings`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1af39e7ea862f949f987aae25faa392d29)`()` | Used to modify any interal rendering settings.
`public inline auto `[`scene`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1ac4de2be6f21929ce1c9cebdbeef56aa6)`()` | Used to access the scene directly by adding or removing elements.

## Members

#### `public void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a7ab92b96ef9c93d84dced77b0ee2cc4b)`()` 

Initializes the renderer.

This function initializes the window subcomponent as well as the API.

#### `public void `[`run`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a8939d31603f84682c2309b784b3a82ac)`()` 

A single function to execute all subcomponents in order.

This function updates the window and the camera components and calls the update and render functions of the API. 
**See also**: [RAYEX_NAMESPACE::Camera::update()](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a37c5fc1ac50d68f1494f90e5b2a569a9), [RAYEX_NAMESPACE::Window::update()](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ab3e217cf1525ce607cb4e99d762359c1), RAYEX_NAMESPACE::Api::update(), RAYEX_NAMESPACE::Api::render()

#### `public auto `[`isRunning`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a3a1d0e4831044f2f7708365c984902bc)`() const` 

#### Returns
Returns true if the application is still running and false if the application has stopped.

#### `public void `[`setCamera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a0f4a81c790bed2dc5243288d9f9e57b4)`(std::shared_ptr< `[`Camera`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` > camera)` 

Used to set a custom camera.

#### Parameters
* `camera` A pointer to a [RAYEX_NAMESPACE::Camera](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera) object.

#### `public void `[`setWindow`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a364c4dcdb67baf57b1591394386ec3af)`(std::shared_ptr< `[`Window`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` > window)` 

Used to set a custom window.

#### Parameters
* `window` A pointer to a [RAYEX_NAMESPACE::Window](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window) object.

#### `public void `[`setGui`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a9c80416850a65f7a7968f58a9b8666c3)`(std::shared_ptr< `[`Gui`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui)` > gui)` 

Used to set a custom GUI.

The GUI can be changed multiple times. Even during runtime. 
#### Parameters
* `gui` A pointer to a [RAYEX_NAMESPACE::Gui](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui) object.

#### `public inline auto `[`getWindow`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a6a3eaad6809863354889f00ad3972b07)`() const` 

#### Returns
Returns a pointer to the renderer's window.

#### `public inline auto `[`getCamera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a46b366bc4157a8cdc31738a505eabbbe)`() const` 

#### Returns
Returns a pointer to the renderer's camera.

#### `public inline auto `[`settings`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1af39e7ea862f949f987aae25faa392d29)`()` 

Used to modify any interal rendering settings.

#### Returns
Returns the settings.

#### `public inline auto `[`scene`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1ac4de2be6f21929ce1c9cebdbeef56aa6)`()` 

Used to access the scene directly by adding or removing elements.

#### Returns
Returns the scene.

# struct `RAYEX_NAMESPACE::Material` 

Describes the rendering properties of a mesh.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::vec3 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1aed286d4563c3708e031c22b1dfdd0850) | 
`public glm::vec3 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1afa1db16e87d06229251a605fed642e31) | 
`public glm::vec3 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a2add0cd69a003e0a01faead2ade81446) | 
`public std::string `[`ambientTexPath`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a8948b3f56c7dcadf9e70a2af417f3e59) | 
`public std::string `[`diffuseTexPath`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a58d73159efe7b4974c952671d50f2531) | 
`public std::string `[`specularTexPath`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1adaf2168f7e03c0c1c263f4145c6c9a88) | 

## Members

#### `public glm::vec3 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1aed286d4563c3708e031c22b1dfdd0850) 

#### `public glm::vec3 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1afa1db16e87d06229251a605fed642e31) 

#### `public glm::vec3 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a2add0cd69a003e0a01faead2ade81446) 

#### `public std::string `[`ambientTexPath`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a8948b3f56c7dcadf9e70a2af417f3e59) 

#### `public std::string `[`diffuseTexPath`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a58d73159efe7b4974c952671d50f2531) 

#### `public std::string `[`specularTexPath`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1adaf2168f7e03c0c1c263f4145c6c9a88) 

# struct `RAYEX_NAMESPACE::Mesh` 

Describes a sub-mesh and its material.

If indexOffset is not set correctly the mesh can not be displayed properly. Take a look at [loadObj(std::string_view)](moxygen/api-undefined.md#group___b_a_s_e_1gac348076d8b31be1511d1fc0532e75747) for a working example.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Material`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material)` `[`material`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_1a004a9c0382dad39d04524fe4f82929ae) | The mesh's material.
`public uint32_t `[`indexOffset`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_1a6d96ab3ebede16df86654d7d25d8435b) | Refers to the offset of this mesh inside a [Geometry::indices](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a11ecc2461cc8f940666cc5535e977c0f) container.

## Members

#### `public `[`Material`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material)` `[`material`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_1a004a9c0382dad39d04524fe4f82929ae) 

The mesh's material.

#### `public uint32_t `[`indexOffset`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_1a6d96ab3ebede16df86654d7d25d8435b) 

Refers to the offset of this mesh inside a [Geometry::indices](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a11ecc2461cc8f940666cc5535e977c0f) container.

# struct `RAYEX_NAMESPACE::Geometry` 

Describes a geometry [Rayex](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex) can render.

Even if a model consists out of multiple sub-meshes, all vertices and indices must be stored together in their respective containers. geometryIndex must be incremented everytime a new model is created.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex)` > `[`vertices`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a9b4d99a41250521028e9406ef1bf9c15) | Contains all vertices of the geometry.
`public std::vector< uint32_t > `[`indices`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a11ecc2461cc8f940666cc5535e977c0f) | Contains all indices of the geometry.
`public std::vector< `[`Mesh`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh)` > `[`meshes`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a7d3f368a8e407c602170ce356b3c5945) | Contains all sub-meshes and their respective materials.
`public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1ab185f8dac7b31f826e611b5a24cb30ce) | A unique index required by the acceleration structures.
`public std::string `[`path`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1ab1abb0b9dda1f43c9b2f92fe83a08588) | The model's path, relative to the path to assets.
`public bool `[`initialized`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1af7124a02f0e2ab740618cf30c1ea748c) | Keeps track of whether or not the geometry was initialized.

## Members

#### `public std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex)` > `[`vertices`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a9b4d99a41250521028e9406ef1bf9c15) 

Contains all vertices of the geometry.

#### `public std::vector< uint32_t > `[`indices`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a11ecc2461cc8f940666cc5535e977c0f) 

Contains all indices of the geometry.

#### `public std::vector< `[`Mesh`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh)` > `[`meshes`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a7d3f368a8e407c602170ce356b3c5945) 

Contains all sub-meshes and their respective materials.

#### `public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1ab185f8dac7b31f826e611b5a24cb30ce) 

A unique index required by the acceleration structures.

#### `public std::string `[`path`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1ab1abb0b9dda1f43c9b2f92fe83a08588) 

The model's path, relative to the path to assets.

#### `public bool `[`initialized`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1af7124a02f0e2ab740618cf30c1ea748c) 

Keeps track of whether or not the geometry was initialized.

# struct `RAYEX_NAMESPACE::GeometryInstance` 

Describes an instance of some geometry.

To assign a specific geometry to an instance, both must have the same value for geometryIndex.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::mat4 `[`transform`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a6d910049d3ae5f62b582fd1f576246a7) | The instance's world transform matrix.
`public glm::mat4 `[`transformIT`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a70a32d8d77163c71da2e8ded720dd30c) | The inverse transpose of transform.
`public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a6104e92fc78364e8f3fb395caf466e39) | Used to assign this instance a model.

## Members

#### `public glm::mat4 `[`transform`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a6d910049d3ae5f62b582fd1f576246a7) 

The instance's world transform matrix.

#### `public glm::mat4 `[`transformIT`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a70a32d8d77163c71da2e8ded720dd30c) 

The inverse transpose of transform.

#### `public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a6104e92fc78364e8f3fb395caf466e39) 

Used to assign this instance a model.

# struct `RAYEX_NAMESPACE::Light` 

A common base for all light types.

Instances of this type should not be used anywhere.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::vec3 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1a7caf60014c05f686a4ca072641d2e8db) | The ambient color.
`public glm::vec3 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1a4159b357e9900158ab92b85ee50746d0) | The diffuse color.
`public glm::vec3 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1a0bc8780a2ab3d6133dc260e2538a2d45) | The specular color.
`public float `[`ambientIntensity`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1ab64fe09dfc039b13271159eef7a93a38) | The intensity of the ambient color.
`public float `[`diffuseIntensity`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1ac4c1d48c802b6b78d5acbf1d0e7fd396) | The intensity of the diffuse color.
`public float `[`specularIntensity`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1ab133a240d8b342dccd16d34752464309) | The intensity of the specular color.
`public bool `[`update`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1a9099dc2317d71a5766dbacdf6003428b) | If set to true the light will be re-uploaded to the physical device.

## Members

#### `public glm::vec3 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1a7caf60014c05f686a4ca072641d2e8db) 

The ambient color.

#### `public glm::vec3 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1a4159b357e9900158ab92b85ee50746d0) 

The diffuse color.

#### `public glm::vec3 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1a0bc8780a2ab3d6133dc260e2538a2d45) 

The specular color.

#### `public float `[`ambientIntensity`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1ab64fe09dfc039b13271159eef7a93a38) 

The intensity of the ambient color.

#### `public float `[`diffuseIntensity`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1ac4c1d48c802b6b78d5acbf1d0e7fd396) 

The intensity of the diffuse color.

#### `public float `[`specularIntensity`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1ab133a240d8b342dccd16d34752464309) 

The intensity of the specular color.

#### `public bool `[`update`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_light_1a9099dc2317d71a5766dbacdf6003428b) 

If set to true the light will be re-uploaded to the physical device.

# struct `RAYEX_NAMESPACE::DirectionalLight` 

```
struct RAYEX_NAMESPACE::DirectionalLight
  : public RAYEX_NAMESPACE::Light
```  

A directional light.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::vec3 `[`direction`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light_1a292ade676ead3692bc3d2dd418a765f8) | 

## Members

#### `public glm::vec3 `[`direction`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_directional_light_1a292ade676ead3692bc3d2dd418a765f8) 

# struct `RAYEX_NAMESPACE::PointLight` 

```
struct RAYEX_NAMESPACE::PointLight
  : public RAYEX_NAMESPACE::Light
```  

A point light.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::vec3 `[`position`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light_1a61392310ead7d93a498c26839d1f25f1) | 

## Members

#### `public glm::vec3 `[`position`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_point_light_1a61392310ead7d93a498c26839d1f25f1) 

