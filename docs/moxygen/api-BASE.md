# group `BASE` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public RX_API std::shared_ptr< Geometry > `[`loadObj`](#group___b_a_s_e_1ga2d50e1cd402880131042e1bcab773b46)`(std::string_view path,bool dynamic)`            | A commodity function for loading a wavefront (.obj) model file and allocate a geometry object from it.
`public RX_API std::shared_ptr< GeometryInstance > `[`instance`](#group___b_a_s_e_1gad880ea304e8ff3dab6ea5258df8970cf)`(std::shared_ptr< Geometry > geometry,const glm::mat4 & transform)`            | A commodity function for allocating an instance from a given geometry and set its matrices.
`class `[`RAYEX_NAMESPACE::Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera) | A minimal camera implementation.
`class `[`RAYEX_NAMESPACE::Settings`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings) | Exposes all graphic settings supported by the renderer.
`class `[`RAYEX_NAMESPACE::Time`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_time) | Used to keep track of the application's timing.
`class `[`RAYEX_NAMESPACE::Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window) | Implements a SDL-based window.
`class `[`RAYEX_NAMESPACE::Rayex`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex) | The main user interface.
`struct `[`RAYEX_NAMESPACE::Material`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material) | Describes the rendering properties of a mesh.
`struct `[`RAYEX_NAMESPACE::Mesh`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh) | Describes a sub-mesh and its material.
`struct `[`RAYEX_NAMESPACE::Geometry`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry) | Describes a geometry [Rayex](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex) can render.
`struct `[`RAYEX_NAMESPACE::GeometryInstance`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance) | Describes an instance of some geometry.

## Members

#### `public RX_API std::shared_ptr< Geometry > `[`loadObj`](#group___b_a_s_e_1ga2d50e1cd402880131042e1bcab773b46)`(std::string_view path,bool dynamic)` 

A commodity function for loading a wavefront (.obj) model file and allocate a geometry object from it.

The function will attempt to find sub-meshes in the file and retrieve all materials. A user is encouraged to create their own model loader function or classes. 
#### Parameters
* `path` The model's path, relative to the path to assets. 

* `dynamic` If true, the geometry can be animated. Otherwise the geometry is static throughout its entire lifetime. 

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
`public void `[`reset`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1acea55c6c1228dfeeb54f3c916917cd66)`()` | 
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

#### `public void `[`reset`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1acea55c6c1228dfeeb54f3c916917cd66)`()` 

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

# class `RAYEX_NAMESPACE::Settings` 

Exposes all graphic settings supported by the renderer.

Any necessary pipeline recreations and swapchain recreations will not be performed at the point of calling any setter but instead the next time the renderer will be updated. Any function that sets the maximum of a given entity needs to be called before [RAYEX_NAMESPACE::Rayex::init()](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a7ab92b96ef9c93d84dced77b0ee2cc4b).

> Todo: Add a setUseTotalPathsOnly( bool flag )

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline auto `[`getPathDepth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1af3f3b1866e7523db6be4179e63ba1e90)`() const` | #### Returns
`public void `[`setPathDepth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a2be035bb411588e1f262f323cff87731)`(uint32_t recursionDepth)` | Used to set the path depth.
`public inline auto `[`getMaxPathDepth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a2d14d58f91d69177df062bb546422cf1)`() const` | #### Returns
`public inline auto `[`getClearColor`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a3fbc4ba4e79dcb84f8065d8657cd09d8)`() const` | #### Returns
`public void `[`setClearColor`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a7c23cd33060112e22df368eeb8fc4c62)`(const glm::vec4 & clearColor)` | Used to changed the clear color.
`public inline auto `[`getAssetsPath`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a8e94a1b47ab849117f0e97bf74332bb0)`() const` | #### Returns
`public void `[`setAssetsPath`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a00b2c779dbe061631b409240d99477fb)`(int argc,char * argv)` | Used to set a path to the directory containing all assets.
`public void `[`setAssetsPath`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a63ae460c13863b4d90fd673039ea6b11)`(std::string_view path)` | Used to set a path to the directory containing all assets.
`public void `[`setAutomaticPipelineRefresh`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ac444874c6f8feb0a7c4385bc17728176)`(bool flag)` | Used to toggle the automatic pipeline recreation.
`public void `[`setGeometryLimit`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ade1e5f9edb4a9e0d845f9274c29c69ee)`(size_t amount)` | Used to set the maximum amount of geometry (3D models) that can be loaded.
`public void `[`setGeometryInstanceLimit`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a13df13c3975521216f1a89092d0ce88f)`(uint32_t amount)` | Used to set the maximum amount of geometry instances (instances of 3D models) that can be loaded.
`public void `[`setTextureLimit`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a10e2253fd1a2a09c615cc481d2936777)`(size_t amount)` | Used to set the maximum amount of textures that can be loaded.
`public void `[`setMeshLimit`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a408bd0ccc0d66b520012842c7679102e)`(size_t amount)` | Used to set the maximum amount of meshes that can be loaded.
`public void `[`setLimits`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1af7642a28e1d98e0374b55bd4a51fa7cd)`(size_t geometryLimit,size_t geometryInstanceLimit,size_t meshLimit,size_t textureLimit)` | Used to set the geometry limit, geometry instance limit, mesh limit and texture limit at once.
`public void `[`setPerPixelSampleRate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a25bcf507ecb541403562560a520f7277)`(uint32_t sampleRate)` | 
`public inline auto `[`getPerPixelSampleRate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a085992c9cf68599ca39d1d2e18f853d8)`() const` | 
`public void `[`setAccumulatingFrames`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1aee44e96b9e45b36bde83d6d1ef23aebe)`(bool flag)` | 
`public inline auto `[`isAccumulatingFrames`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ac5a799f6d25e7a02fd751b8acb37cdb7)`() const` | 
`public inline void `[`triggerPipelineRefresh`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a908de1120d121ee07d91269ade6dd4ea)`()` | 
`public inline void `[`triggerSwapchainRefresh`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1acc7b4dff13bd3a8686ba2070b939fabd)`()` | 

## Members

#### `public inline auto `[`getPathDepth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1af3f3b1866e7523db6be4179e63ba1e90)`() const` 

#### Returns
Returns the path depth.

#### `public void `[`setPathDepth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a2be035bb411588e1f262f323cff87731)`(uint32_t recursionDepth)` 

Used to set the path depth.

The function will trigger a pipeline recreation as soon as possible unless it was explicitely disabled using [setAutomaticPipelineRefresh(bool)](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ac444874c6f8feb0a7c4385bc17728176). If a value higher than the device's maximum supported value is set, it will use the maximum value instead. 
#### Parameters
* `recursionDepth` The new value for the recursion depth.

#### `public inline auto `[`getMaxPathDepth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a2d14d58f91d69177df062bb546422cf1)`() const` 

#### Returns
Returns the maximum path depth on the GPU.

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

#### `public void `[`setAutomaticPipelineRefresh`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ac444874c6f8feb0a7c4385bc17728176)`(bool flag)` 

Used to toggle the automatic pipeline recreation.

#### Parameters
* `flag` If false, the pipelines will not be recreated automatically until this function is called with true.

#### `public void `[`setGeometryLimit`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ade1e5f9edb4a9e0d845f9274c29c69ee)`(size_t amount)` 

Used to set the maximum amount of geometry (3D models) that can be loaded.

#### `public void `[`setGeometryInstanceLimit`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a13df13c3975521216f1a89092d0ce88f)`(uint32_t amount)` 

Used to set the maximum amount of geometry instances (instances of 3D models) that can be loaded.

#### `public void `[`setTextureLimit`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a10e2253fd1a2a09c615cc481d2936777)`(size_t amount)` 

Used to set the maximum amount of textures that can be loaded.

#### `public void `[`setMeshLimit`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a408bd0ccc0d66b520012842c7679102e)`(size_t amount)` 

Used to set the maximum amount of meshes that can be loaded.

This value is not strictly related to the amount of geometries. Each geometry can have an arbitrary amount of meshes or sub-mehes. To optimize the buffer size it is recommended to make at least a rough estimate on the amount of submeshes that are going to be in the scene.

#### `public void `[`setLimits`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1af7642a28e1d98e0374b55bd4a51fa7cd)`(size_t geometryLimit,size_t geometryInstanceLimit,size_t meshLimit,size_t textureLimit)` 

Used to set the geometry limit, geometry instance limit, mesh limit and texture limit at once.

**See also**: [setGeometryLimit(size_t)](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ade1e5f9edb4a9e0d845f9274c29c69ee), setGeometryInstanceLimit(size_t), [setMeshLimit(size_t)](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a408bd0ccc0d66b520012842c7679102e) and [setTextureLimit(size_t)](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a10e2253fd1a2a09c615cc481d2936777).

#### `public void `[`setPerPixelSampleRate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a25bcf507ecb541403562560a520f7277)`(uint32_t sampleRate)` 

#### `public inline auto `[`getPerPixelSampleRate`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a085992c9cf68599ca39d1d2e18f853d8)`() const` 

#### `public void `[`setAccumulatingFrames`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1aee44e96b9e45b36bde83d6d1ef23aebe)`(bool flag)` 

#### `public inline auto `[`isAccumulatingFrames`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1ac5a799f6d25e7a02fd751b8acb37cdb7)`() const` 

#### `public inline void `[`triggerPipelineRefresh`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1a908de1120d121ee07d91269ade6dd4ea)`()` 

#### `public inline void `[`triggerSwapchainRefresh`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings_1acc7b4dff13bd3a8686ba2070b939fabd)`()` 

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
`public void `[`setWindow`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a364c4dcdb67baf57b1591394386ec3af)`(std::shared_ptr< `[`Window`](moxygen/api-BASE.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` > window)` | Used to set a custom window.
`public void `[`setGui`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a9c80416850a65f7a7968f58a9b8666c3)`(std::shared_ptr< `[`Gui`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui)` > gui)` | Used to set a custom GUI.
`public inline auto `[`getWindow`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a6a3eaad6809863354889f00ad3972b07)`() const` | #### Returns
`public inline auto `[`settings`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1af39e7ea862f949f987aae25faa392d29)`()` | Used to modify any internal rendering settings.
`public inline auto `[`scene`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1ac4de2be6f21929ce1c9cebdbeef56aa6)`()` | Used to access the scene directly by adding or removing elements.
`public void `[`reset`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a64614dc316d4da644a62ff9463fca0dd)`()` | 

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

#### `public inline auto `[`settings`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1af39e7ea862f949f987aae25faa392d29)`()` 

Used to modify any internal rendering settings.

#### Returns
Returns the settings.

#### `public inline auto `[`scene`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1ac4de2be6f21929ce1c9cebdbeef56aa6)`()` 

Used to access the scene directly by adding or removing elements.

#### Returns
Returns the scene.

#### `public void `[`reset`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a64614dc316d4da644a62ff9463fca0dd)`()` 

# struct `RAYEX_NAMESPACE::Material` 

Describes the rendering properties of a mesh.

Property descriptions copied from [https://www.loc.gov/preservation/digital/formats/fdd/fdd000508.shtml](https://www.loc.gov/preservation/digital/formats/fdd/fdd000508.shtml).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::vec3 `[`ka`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a7450ad0dc89bbcfd69a89d64b46eec77) | 
`public glm::vec3 `[`kd`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a9018051af4ed4bfd9299ba4051f93827) | Ambient color.
`public glm::vec3 `[`ks`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a5ef48ca2998c12c026b08eecf951e1d4) | Diffuse color.
`public std::string `[`ambientTexPath`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a8948b3f56c7dcadf9e70a2af417f3e59) | Specular color.
`public std::string `[`diffuseTexPath`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a58d73159efe7b4974c952671d50f2531) | 
`public std::string `[`specularTexPath`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1adaf2168f7e03c0c1c263f4145c6c9a88) | 
`public glm::vec3 `[`emission`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a7c4c5f7a82461a66b18c2119411073d3) | 
`public uint32_t `[`illum`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a2985375b4f62c77adc4f5aae8f884ed9) | Illumination model.
`public float `[`d`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a4aadf85f61539cbd7e335754621f8fd3) | Specifies a factor for dissolve, how much this material dissolves into the background. A factor of 1.0 is fully opaque. A factor of 0.0 is completely transparent.
`public float `[`ns`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a309eafdbd92cf69e4a7daa6a59c93573) | Focus of the specular light (aka shininess). Ranges from 0 to 1000, with a high value resulting in a tight, concentrated highlight.
`public float `[`ni`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a4a2874571c4c3307df4235f57c5c6639) | Optical density (aka index of refraction). Ranges from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.

## Members

#### `public glm::vec3 `[`ka`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a7450ad0dc89bbcfd69a89d64b46eec77) 

#### `public glm::vec3 `[`kd`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a9018051af4ed4bfd9299ba4051f93827) 

Ambient color.

#### `public glm::vec3 `[`ks`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a5ef48ca2998c12c026b08eecf951e1d4) 

Diffuse color.

#### `public std::string `[`ambientTexPath`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a8948b3f56c7dcadf9e70a2af417f3e59) 

Specular color.

#### `public std::string `[`diffuseTexPath`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a58d73159efe7b4974c952671d50f2531) 

#### `public std::string `[`specularTexPath`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1adaf2168f7e03c0c1c263f4145c6c9a88) 

#### `public glm::vec3 `[`emission`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a7c4c5f7a82461a66b18c2119411073d3) 

#### `public uint32_t `[`illum`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a2985375b4f62c77adc4f5aae8f884ed9) 

Illumination model.

0 - A constant color illumination model, using the Kd for the material. 1 - A diffuse illumination model using Lambertian shading, taking into account Ka, Kd, the intensity and position of each light source and the angle at which it strikes the surface. 2 - A diffuse and specular illumination model using Lambertian shading and Blinn's interpretation of Phong's specular illumination model, taking into account Ka, Kd, Ks, and the intensity and position of each light source and the angle at which it strikes the surface.

#### `public float `[`d`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a4aadf85f61539cbd7e335754621f8fd3) 

Specifies a factor for dissolve, how much this material dissolves into the background. A factor of 1.0 is fully opaque. A factor of 0.0 is completely transparent.

#### `public float `[`ns`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a309eafdbd92cf69e4a7daa6a59c93573) 

Focus of the specular light (aka shininess). Ranges from 0 to 1000, with a high value resulting in a tight, concentrated highlight.

#### `public float `[`ni`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a4a2874571c4c3307df4235f57c5c6639) 

Optical density (aka index of refraction). Ranges from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.

# struct `RAYEX_NAMESPACE::Mesh` 

Describes a sub-mesh and its material.

If indexOffset is not set correctly the mesh can not be displayed properly. Take a look at loadObj(std::string_view) for a working example.

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
`public bool `[`dynamic`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a4a926e0a1c48cf58f445b501c79bebb1) | Keeps track of whether or not the geometry is dynamic or static.
`public RX_API void `[`setMaterial`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a20ea3640e900acebb235adc345de1bd0)`(const `[`Material`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material)` & material)` | 

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

#### `public bool `[`dynamic`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a4a926e0a1c48cf58f445b501c79bebb1) 

Keeps track of whether or not the geometry is dynamic or static.

#### `public RX_API void `[`setMaterial`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a20ea3640e900acebb235adc345de1bd0)`(const `[`Material`](moxygen/api-BASE.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material)` & material)` 

# struct `RAYEX_NAMESPACE::GeometryInstance` 

Describes an instance of some geometry.

To assign a specific geometry to an instance, both must have the same value for geometryIndex.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::mat4 `[`transform`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a6d910049d3ae5f62b582fd1f576246a7) | The instance's world transform matrix.
`public glm::mat4 `[`transformIT`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a70a32d8d77163c71da2e8ded720dd30c) | The inverse transpose of transform.
`public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a6104e92fc78364e8f3fb395caf466e39) | Used to assign this instance a model.
`public RX_API void `[`setTransform`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a165e07ad30882bac25ab130fd1116dd5)`(const glm::mat4 & transform)` | 

## Members

#### `public glm::mat4 `[`transform`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a6d910049d3ae5f62b582fd1f576246a7) 

The instance's world transform matrix.

#### `public glm::mat4 `[`transformIT`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a70a32d8d77163c71da2e8ded720dd30c) 

The inverse transpose of transform.

#### `public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a6104e92fc78364e8f3fb395caf466e39) 

Used to assign this instance a model.

#### `public RX_API void `[`setTransform`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance_1a165e07ad30882bac25ab130fd1116dd5)`(const glm::mat4 & transform)` 

