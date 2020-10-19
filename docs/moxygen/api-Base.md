# group `Base` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public RX_API std::shared_ptr< Geometry > `[`loadObj`](#group___base_1gac348076d8b31be1511d1fc0532e75747)`(std::string_view path)`            | A commodity function for loading a wavefront (.obj) model file and allocate a geometry object from it.
`public RX_API std::shared_ptr< GeometryInstance > `[`instance`](#group___base_1gad880ea304e8ff3dab6ea5258df8970cf)`(std::shared_ptr< Geometry > geometry,const glm::mat4 & transform)`            | A commodity function for allocating an instance from a given geometry and set its matrices.
`class `[`RAYEX_NAMESPACE::Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera) | A minimal camera implementation.
`class `[`RAYEX_NAMESPACE::Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window) | Implements a SDL-based window.
`class `[`RAYEX_NAMESPACE::Rayex`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex) | The main user interface.
`struct `[`RAYEX_NAMESPACE::Material`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material) | Describes the rendering properties of a mesh.
`struct `[`RAYEX_NAMESPACE::Mesh`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh) | Describes a sub-mesh and its material.
`struct `[`RAYEX_NAMESPACE::Geometry`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry) | Describes a geometry [Rayex](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex) can render.
`struct `[`RAYEX_NAMESPACE::GeometryInstance`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_instance) | Describes an instance of some geometry.

## Members

#### `public RX_API std::shared_ptr< Geometry > `[`loadObj`](#group___base_1gac348076d8b31be1511d1fc0532e75747)`(std::string_view path)` 

A commodity function for loading a wavefront (.obj) model file and allocate a geometry object from it.

The function will attempt to find sub-meshes in the file and retrieve all materials. A user is encouraged to create their own model loader function or classes. 
#### Parameters
* `path` The model's path, relative to the path to assets. 

#### Returns
Returns a pointer to a geometry object.

#### `public RX_API std::shared_ptr< GeometryInstance > `[`instance`](#group___base_1gad880ea304e8ff3dab6ea5258df8970cf)`(std::shared_ptr< Geometry > geometry,const glm::mat4 & transform)` 

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

The user has to handle keyboard related camera changes inside [update()](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ab53ec8ef049669ea018405645d0cb5ed). 

Do not forget to re-calculate the view or projection matrix if the camera or the window have changed. See [updateViewMatrix()](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1acc914ffb4d382f0a8fa381e8ec394ef6), [updateProjectionMatrix()](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ab77a53c918476b7e4e8f60cc02144c30) or updateView and updateProj respectively.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public bool `[`updateView`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1abd6bb5ccf88c173842750d7dc6ccb44a) | Keeps track of whether or not to udpate the view matrix.
`public bool `[`updateProj`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aefd04f562091ba752ae641181d32d2d4) | Keeps track of whether or not to udpate the projection matrix.
`public RX_API `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a68a8b11d9b44fbf607cf39ce4827247d)`(int width,int height,const glm::vec3 & position)` | #### Parameters
`public virtual RX_API `[`~Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a61e6d4d44980a2294b7eff088f700fd9)`() = default` | 
`public RX_API `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1adefdb55933d7e4557d82160f3ff36f1e)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &) = default` | 
`public  `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a81848737678ca365e989e8f4e6585c2b)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &&) = delete` | 
`public RX_API auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a18132566aef52264d1c82cb4a6b85a46)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &) = default` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a1211b4836b93378f4e90b1cf780cf0d7)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &&) = delete` | 
`public virtual RX_API void `[`update`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ab53ec8ef049669ea018405645d0cb5ed)`()` | Is used to update camera vectors etc.
`public inline RX_API auto `[`getPosition`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ad4a222fefb2845510a9a05d6eeb7507a)`() const` | #### Returns
`public RX_API void `[`setPosition`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a7df5ab78c0d7c755781183de40ce36b9)`(const glm::vec3 & position)` | Is used to change the camera's position.
`public RX_API void `[`setSize`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aec6722156001046318c55ba685d2ee18)`(int width,int height)` | Is used to set a size for the camera that fits the viewport dimensions.
`public RX_API void `[`setFov`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a2ea2da4418290b6a6b9fa7620e6bd7b9)`(float fov)` | Is used to set the camera's field of view.
`public RX_API void `[`setSensitivity`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a9fdfefdba9a848fdd71c882fc18a4134)`(float sensitivity)` | Is used to set the mouse sensitivity.
`public inline auto `[`getViewMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ab93f93f316bead7870322c0a6c7938d7)`() const` | #### Returns
`public inline auto `[`getProjectionMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a44a23ce5e54d9e67da5abfe53205882f)`() const` | #### Returns
`public inline auto `[`getViewInverseMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ad52515b2c20a16149c4d5652a3e0b11c)`() const` | #### Returns
`public inline auto `[`getProjectionInverseMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a428203c7dfd644026cc8cb6c94f5ed8a)`() const` | #### Returns
`public RX_API void `[`updateViewMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1acc914ffb4d382f0a8fa381e8ec394ef6)`()` | Re-calculates the camera's view matrix as well as the inversed view matrix.
`public RX_API void `[`updateProjectionMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ab77a53c918476b7e4e8f60cc02144c30)`()` | Re-calculates the camera's projection matrix as well as the inversed projection matrix.
`public virtual RX_API void `[`processMouse`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a5c575881c73d250e2a31779761ec7802)`(float xOffset,float yOffset)` | Processes mouse input (default implementation).
`public inline virtual RX_API void `[`processKeyboard`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ae7d3feb791abeb7b22630c5104582959)`()` | 
`protected int `[`width`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1abc547465a1f853e1f98b3b5efe6838d9) | The width of the viewport.
`protected int `[`height`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ad6da1290d6cf328a700a8155be2165e5) | The height of the viewport.
`protected glm::vec3 `[`position`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a4bcf1463171a160aa37af760fad1959b) | The camera's position.
`protected glm::mat4 `[`view`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a5ac095603a19a2fd745250f123438dd7) | The view matrix.
`protected glm::mat4 `[`projection`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a1f992a71469dc28c9b9ea52e63ee429b) | The projection matrix.
`protected glm::mat4 `[`viewInverse`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a7c50c7657eeba2989f54a35720120049) | The view matrix inversed.
`protected glm::mat4 `[`projectionInverse`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a2364ff321e4eae5893dfae520effc06c) | The projection matrix inversed.
`protected glm::vec3 `[`worldUp`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1acdbab5dcb28d135dcb4b6fadcf2fcacd) | The world up vector.
`protected glm::vec3 `[`up`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a6151963e691f56848a6b628b93fa3c4a) | The local up vector.
`protected glm::vec3 `[`right`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1afff0ad0e901baca57efc06c5a1b4f81d) | The local right vector.
`protected glm::vec3 `[`front`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a621f40b7aba2d90899bfe7b5ef8f6292) | The viewing direction.
`protected float `[`yaw`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a8623593d33d30628260e42472b9cc2b6) | The yaw (left and right).
`protected float `[`pitch`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1afe4117e810213542a6f47d8984e05605) | The pitch (down and up).
`protected float `[`sensitivity`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1afd1c0517418d1f559507da86c935e64e) | The mouse sensitivity.
`protected float `[`fov`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ac086ddc16ca93585a9e09935b022113a) | The field of view.
`protected RX_API void `[`updateVectors`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a50687e95db7b89210e02610600dcb9df)`()` | Updates the camera vectors.

## Members

#### `public bool `[`updateView`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1abd6bb5ccf88c173842750d7dc6ccb44a) 

Keeps track of whether or not to udpate the view matrix.

#### `public bool `[`updateProj`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aefd04f562091ba752ae641181d32d2d4) 

Keeps track of whether or not to udpate the projection matrix.

#### `public RX_API `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a68a8b11d9b44fbf607cf39ce4827247d)`(int width,int height,const glm::vec3 & position)` 

#### Parameters
* `width` The width of the viewport. 

* `height` The height of the viewport. 

* `position` The position of your camera.

#### `public virtual RX_API `[`~Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a61e6d4d44980a2294b7eff088f700fd9)`() = default` 

#### `public RX_API `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1adefdb55933d7e4557d82160f3ff36f1e)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &) = default` 

#### `public  `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a81848737678ca365e989e8f4e6585c2b)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &&) = delete` 

#### `public RX_API auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a18132566aef52264d1c82cb4a6b85a46)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &) = default` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a1211b4836b93378f4e90b1cf780cf0d7)`(const `[`Camera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` &&) = delete` 

#### `public virtual RX_API void `[`update`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ab53ec8ef049669ea018405645d0cb5ed)`()` 

Is used to update camera vectors etc.

The user has to override this function for the camera to work like intended. The function will be called every tick.

#### `public inline RX_API auto `[`getPosition`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ad4a222fefb2845510a9a05d6eeb7507a)`() const` 

#### Returns
Returns the camera's position.

#### `public RX_API void `[`setPosition`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a7df5ab78c0d7c755781183de40ce36b9)`(const glm::vec3 & position)` 

Is used to change the camera's position.

#### Parameters
* `position` The new camera position.

#### `public RX_API void `[`setSize`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1aec6722156001046318c55ba685d2ee18)`(int width,int height)` 

Is used to set a size for the camera that fits the viewport dimensions.

#### Parameters
* `width` The width of the viewport. 

* `height` The height of the viewport.

#### `public RX_API void `[`setFov`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a2ea2da4418290b6a6b9fa7620e6bd7b9)`(float fov)` 

Is used to set the camera's field of view.

#### Parameters
* `fov` The new field of view.

#### `public RX_API void `[`setSensitivity`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a9fdfefdba9a848fdd71c882fc18a4134)`(float sensitivity)` 

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

#### `public RX_API void `[`updateViewMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1acc914ffb4d382f0a8fa381e8ec394ef6)`()` 

Re-calculates the camera's view matrix as well as the inversed view matrix.

#### `public RX_API void `[`updateProjectionMatrix`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ab77a53c918476b7e4e8f60cc02144c30)`()` 

Re-calculates the camera's projection matrix as well as the inversed projection matrix.

#### `public virtual RX_API void `[`processMouse`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a5c575881c73d250e2a31779761ec7802)`(float xOffset,float yOffset)` 

Processes mouse input (default implementation).

#### Parameters
* `xOffset` The difference of the current offset on the x-axis and the previous offset. 

* `yOffset` The difference of the current offset on the y-axis and the previous offset.

#### `public inline virtual RX_API void `[`processKeyboard`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ae7d3feb791abeb7b22630c5104582959)`()` 

#### `protected int `[`width`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1abc547465a1f853e1f98b3b5efe6838d9) 

The width of the viewport.

#### `protected int `[`height`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ad6da1290d6cf328a700a8155be2165e5) 

The height of the viewport.

#### `protected glm::vec3 `[`position`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a4bcf1463171a160aa37af760fad1959b) 

The camera's position.

#### `protected glm::mat4 `[`view`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a5ac095603a19a2fd745250f123438dd7) 

The view matrix.

#### `protected glm::mat4 `[`projection`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a1f992a71469dc28c9b9ea52e63ee429b) 

The projection matrix.

#### `protected glm::mat4 `[`viewInverse`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a7c50c7657eeba2989f54a35720120049) 

The view matrix inversed.

#### `protected glm::mat4 `[`projectionInverse`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a2364ff321e4eae5893dfae520effc06c) 

The projection matrix inversed.

#### `protected glm::vec3 `[`worldUp`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1acdbab5dcb28d135dcb4b6fadcf2fcacd) 

The world up vector.

#### `protected glm::vec3 `[`up`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a6151963e691f56848a6b628b93fa3c4a) 

The local up vector.

#### `protected glm::vec3 `[`right`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1afff0ad0e901baca57efc06c5a1b4f81d) 

The local right vector.

#### `protected glm::vec3 `[`front`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a621f40b7aba2d90899bfe7b5ef8f6292) 

The viewing direction.

#### `protected float `[`yaw`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a8623593d33d30628260e42472b9cc2b6) 

The yaw (left and right).

#### `protected float `[`pitch`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1afe4117e810213542a6f47d8984e05605) 

The pitch (down and up).

#### `protected float `[`sensitivity`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1afd1c0517418d1f559507da86c935e64e) 

The mouse sensitivity.

#### `protected float `[`fov`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ac086ddc16ca93585a9e09935b022113a) 

The field of view.

#### `protected RX_API void `[`updateVectors`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1a50687e95db7b89210e02610600dcb9df)`()` 

Updates the camera vectors.

Only needs to be called if mouse was moved.

# class `RAYEX_NAMESPACE::Window` 

Implements a SDL-based window.

In order to handle events the user has to create a class that inherits from [Window](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window). 

Example
```cpp
// This example requires the user to implement a custom window class that inherits from Window.
auto myWindow = std::make_shared<CustomWindow>( [width](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a293b96059836d62bc1f05cc374f14fca), [height](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1af90626b5a7995df6d22fcdb6b475ff7e), "Example", WINDOW_RESIZABLE | WINDOW_INPUT_FOCUS );
```

**See also**: [update()](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a8330a97de6371adc64b7d12419ad2a12)

> Todo: To avoid the user having to link to SDL themselves requires to write a complete SDL2 wrapper.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public RX_API `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a66f66ccc873712a7724fead6a0b189d6)`(int width,int height,const char * title,uint32_t flags) = default` | #### Parameters
`public virtual RX_API `[`~Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a75f43cfd4baed3056c1a1e2cda7165b9)`()` | Destructor of [Window](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window).
`public  `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a908c6b6221e0f110467fd265f23a0097)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &) = delete` | 
`public  `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ad4d23db965bedb12755ad8d0addbcfe4)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &&) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a3b2477e338e063b1cfe3b1764a7f9de5)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &) = delete` | 
`public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1abbc5d180aaaa9c97563f5563ab0e884e)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &&) = delete` | 
`public virtual RX_API auto `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a7bb527dc03f0d8d338b5e60043f45484)`()` | Initializes the SDL-window.
`public virtual RX_API auto `[`update`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a8330a97de6371adc64b7d12419ad2a12)`()` | Updates window-related components.
`public virtual RX_API void `[`clean`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1aae5d261f1dc0f4c9803636dfe9fbca2f)`()` | Destroys the window.
`public RX_API void `[`resize`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a1ef21fbb70c7f423a5329cf294ef4c56)`(int width,int height)` | Resizes the window.
`public inline RX_API auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1aa66df2f85d75100246163ebc0fda1e74)`()` | #### Returns
`public RX_API auto `[`getExtent`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ad8e960259694ee6158077512397f4ada)`() const` | #### Returns
`public inline auto `[`getWidth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a26db3cec4e76fe4092c4fbdac20aea33)`() const` | #### Returns
`public inline auto `[`getHeight`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a35cf7ab1da6f4fb21dd03cd67e05715a)`() const` | #### Returns
`public RX_API auto `[`changed`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1af7592cf50bffc8b586910b50a480c938)`()` | #### Returns
`public RX_API auto `[`minimized`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1aa102c21a2f54ffdef06c7485ec3c5194)`()` | #### Returns
`protected SDL_Window * `[`window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a9e20d9ed5a5f514233b8d7cc3f7178ae) | The actual SDL_Window object.
`protected uint32_t `[`flags`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a194f9d05fd4336372214025ba406ecc1) | The window's flags.
`protected int `[`width`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a293b96059836d62bc1f05cc374f14fca) | The window's width in pixels.
`protected int `[`height`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1af90626b5a7995df6d22fcdb6b475ff7e) | The window's height in pixels.
`protected const char * `[`title`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1af6786684e67583f97632af87568f19d3) | The window's title.

## Members

#### `public RX_API `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a66f66ccc873712a7724fead6a0b189d6)`(int width,int height,const char * title,uint32_t flags) = default` 

#### Parameters
* `width` The width of the window in pixels. 

* `height` The height of the window in pixels. 

* `title` The title of the window shown in the title bar. 

* `flags` The window flags (no additional flags if not specified).

#### `public virtual RX_API `[`~Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a75f43cfd4baed3056c1a1e2cda7165b9)`()` 

Destructor of [Window](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window).

Will also clean up the object's resources. 
**See also**: [clean()](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1aae5d261f1dc0f4c9803636dfe9fbca2f)

#### `public  `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a908c6b6221e0f110467fd265f23a0097)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &) = delete` 

#### `public  `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ad4d23db965bedb12755ad8d0addbcfe4)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &&) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a3b2477e338e063b1cfe3b1764a7f9de5)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &) = delete` 

#### `public auto `[`operator=`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1abbc5d180aaaa9c97563f5563ab0e884e)`(const `[`Window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` &&) = delete` 

#### `public virtual RX_API auto `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a7bb527dc03f0d8d338b5e60043f45484)`()` 

Initializes the SDL-window.

#### `public virtual RX_API auto `[`update`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a8330a97de6371adc64b7d12419ad2a12)`()` 

Updates window-related components.

In case the user wants to handle input events, this function should be overwritten in the inherited class. The user has to call [Window::update()](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a8330a97de6371adc64b7d12419ad2a12) to ensure proper functionality.

#### `public virtual RX_API void `[`clean`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1aae5d261f1dc0f4c9803636dfe9fbca2f)`()` 

Destroys the window.

#### `public RX_API void `[`resize`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a1ef21fbb70c7f423a5329cf294ef4c56)`(int width,int height)` 

Resizes the window.

#### Parameters
* `width` The desired width in pixels. 

* `height` The desired height in pixels.

#### `public inline RX_API auto `[`get`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1aa66df2f85d75100246163ebc0fda1e74)`()` 

#### Returns
Returns the actual SDL_Window object.

#### `public RX_API auto `[`getExtent`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1ad8e960259694ee6158077512397f4ada)`() const` 

#### Returns
Returns the window size as a Vulkan 2D extent.

#### `public inline auto `[`getWidth`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a26db3cec4e76fe4092c4fbdac20aea33)`() const` 

#### Returns
Returns the window's width in pixels.

#### `public inline auto `[`getHeight`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a35cf7ab1da6f4fb21dd03cd67e05715a)`() const` 

#### Returns
Returns the window's height in pixels.

#### `public RX_API auto `[`changed`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1af7592cf50bffc8b586910b50a480c938)`()` 

#### Returns
Returns true if the window size has changed.

#### `public RX_API auto `[`minimized`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1aa102c21a2f54ffdef06c7485ec3c5194)`()` 

#### Returns
Returns true if the window is minimized.

#### `protected SDL_Window * `[`window`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a9e20d9ed5a5f514233b8d7cc3f7178ae) 

The actual SDL_Window object.

#### `protected uint32_t `[`flags`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a194f9d05fd4336372214025ba406ecc1) 

The window's flags.

#### `protected int `[`width`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a293b96059836d62bc1f05cc374f14fca) 

The window's width in pixels.

#### `protected int `[`height`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1af90626b5a7995df6d22fcdb6b475ff7e) 

The window's height in pixels.

#### `protected const char * `[`title`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1af6786684e67583f97632af87568f19d3) 

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
`public `[`Settings`](moxygen/api-undefined.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings)` `[`settings`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1aeffc2a614d26af4c71880c5687cbb4d1) | Used to modify any interal rendering settings.
`public `[`Scene`](moxygen/api-undefined.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene)` `[`scene`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a1a12eb0392e6415a346d2f875290ca78) | Contains all meshes, all mesh instances and all light sources.
`public RX_API void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a603b64e7ad5ea501077642cd417836a6)`()` | Initializes the renderer.
`public RX_API void `[`run`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1ae3073d73c91714b647d95731b0a26885)`()` | A single function to execute all subcomponents in order.
`public RX_API auto `[`isRunning`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1aac607ac7aa791bc2cbd3e52cb09c609d)`() const` | #### Returns
`public RX_API void `[`setCamera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1ab58f15e6b8f034662ec7207d22296b3d)`(std::shared_ptr< `[`Camera`](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` > camera)` | Used to set a custom camera.
`public RX_API void `[`setWindow`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1adb2244d970b239304b994fb34cb0cbd8)`(std::shared_ptr< `[`Window`](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` > window)` | Used to set a custom window.
`public RX_API void `[`setGui`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1afa47f7f0f0687a5eccd67feed5c8b2f6)`(std::shared_ptr< `[`Gui`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui)` > gui)` | Used to set a custom GUI.
`public inline RX_API auto `[`getWindow`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a3be83e8f0afa19c75faeffd7889863dc)`() const` | #### Returns
`public inline RX_API auto `[`getCamera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a4550a4c84e09b25d252c74d3d2bfbcf2)`() const` | #### Returns

## Members

#### `public `[`Settings`](moxygen/api-undefined.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings)` `[`settings`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1aeffc2a614d26af4c71880c5687cbb4d1) 

Used to modify any interal rendering settings.

**See also**: [RAYEX_NAMESPACE::Settings](moxygen/api-undefined.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_settings)

#### `public `[`Scene`](moxygen/api-undefined.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_scene)` `[`scene`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a1a12eb0392e6415a346d2f875290ca78) 

Contains all meshes, all mesh instances and all light sources.

Used to access the scene directly by adding or removing elements.

#### `public RX_API void `[`init`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a603b64e7ad5ea501077642cd417836a6)`()` 

Initializes the renderer.

This function initializes the window subcomponent as well as the API.

#### `public RX_API void `[`run`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1ae3073d73c91714b647d95731b0a26885)`()` 

A single function to execute all subcomponents in order.

This function updates the window and the camera components and calls the update and render functions of the API. 
**See also**: [RAYEX_NAMESPACE::Camera::update()](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera_1ab53ec8ef049669ea018405645d0cb5ed), [RAYEX_NAMESPACE::Window::update()](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window_1a8330a97de6371adc64b7d12419ad2a12), RAYEX_NAMESPACE::Api::update(), RAYEX_NAMESPACE::Api::render()

#### `public RX_API auto `[`isRunning`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1aac607ac7aa791bc2cbd3e52cb09c609d)`() const` 

#### Returns
Returns true if the application is still running and false if the application has stopped.

#### `public RX_API void `[`setCamera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1ab58f15e6b8f034662ec7207d22296b3d)`(std::shared_ptr< `[`Camera`](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera)` > camera)` 

Used to set a custom camera.

#### Parameters
* `camera` A pointer to a [RAYEX_NAMESPACE::Camera](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_camera) object.

#### `public RX_API void `[`setWindow`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1adb2244d970b239304b994fb34cb0cbd8)`(std::shared_ptr< `[`Window`](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window)` > window)` 

Used to set a custom window.

#### Parameters
* `window` A pointer to a [RAYEX_NAMESPACE::Window](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_window) object.

#### `public RX_API void `[`setGui`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1afa47f7f0f0687a5eccd67feed5c8b2f6)`(std::shared_ptr< `[`Gui`](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui)` > gui)` 

Used to set a custom GUI.

The GUI can be changed multiple times. Even during runtime. 
#### Parameters
* `gui` A pointer to a [RAYEX_NAMESPACE::Gui](moxygen/api-API.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_gui) object.

#### `public inline RX_API auto `[`getWindow`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a3be83e8f0afa19c75faeffd7889863dc)`() const` 

#### Returns
Returns a pointer to the renderer's window.

#### `public inline RX_API auto `[`getCamera`](#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex_1a4550a4c84e09b25d252c74d3d2bfbcf2)`() const` 

#### Returns
Returns a pointer to the renderer's camera.

# struct `RAYEX_NAMESPACE::Material` 

Describes the rendering properties of a mesh.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public glm::vec4 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a116f68a573129b5992107114caa22734) | 
`public glm::vec4 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a2bb5c447a67dbf43d068aab437f95659) | 
`public glm::vec4 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a2b3e419b571eabc95ec760eea7025c22) | 

## Members

#### `public glm::vec4 `[`ambient`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a116f68a573129b5992107114caa22734) 

#### `public glm::vec4 `[`diffuse`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a2bb5c447a67dbf43d068aab437f95659) 

#### `public glm::vec4 `[`specular`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material_1a2b3e419b571eabc95ec760eea7025c22) 

# struct `RAYEX_NAMESPACE::Mesh` 

Describes a sub-mesh and its material.

If indexOffset is not set correctly the mesh can not be displayed properly. Take a look at [loadObj(std::string_view)](moxygen/api-undefined.md#group___base_1gac348076d8b31be1511d1fc0532e75747) for a working example.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Material`](moxygen/api-Base.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material)` `[`material`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_1a004a9c0382dad39d04524fe4f82929ae) | The mesh's material.
`public uint32_t `[`indexOffset`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_1a6d96ab3ebede16df86654d7d25d8435b) | Refers to the offset of this mesh inside a [Geometry::indices](moxygen/api-Base.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a11ecc2461cc8f940666cc5535e977c0f) container.

## Members

#### `public `[`Material`](moxygen/api-Base.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_material)` `[`material`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_1a004a9c0382dad39d04524fe4f82929ae) 

The mesh's material.

#### `public uint32_t `[`indexOffset`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh_1a6d96ab3ebede16df86654d7d25d8435b) 

Refers to the offset of this mesh inside a [Geometry::indices](moxygen/api-Base.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a11ecc2461cc8f940666cc5535e977c0f) container.

# struct `RAYEX_NAMESPACE::Geometry` 

Describes a geometry [Rayex](moxygen/api-Base.md#class_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_rayex) can render.

Even if a model consists out of multiple sub-meshes, all vertices and indices must be stored together in their respective containers. geometryIndex must be incremented everytime a new model is created.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex)` > `[`vertices`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a9b4d99a41250521028e9406ef1bf9c15) | Contains all vertices of the geometry.
`public std::vector< uint32_t > `[`indices`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a11ecc2461cc8f940666cc5535e977c0f) | Contains all indices of the geometry.
`public std::vector< `[`Mesh`](moxygen/api-Base.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh)` > `[`meshes`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a7d3f368a8e407c602170ce356b3c5945) | Contains all sub-meshes and their respective materials.
`public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1ab185f8dac7b31f826e611b5a24cb30ce) | A unique index required by the acceleration structures.
`public std::string_view `[`path`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a503d828c8cd38560e2506ace136b829e) | The model's path, relative to the path to assets.

## Members

#### `public std::vector< `[`Vertex`](moxygen/api-API.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_vertex)` > `[`vertices`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a9b4d99a41250521028e9406ef1bf9c15) 

Contains all vertices of the geometry.

#### `public std::vector< uint32_t > `[`indices`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a11ecc2461cc8f940666cc5535e977c0f) 

Contains all indices of the geometry.

#### `public std::vector< `[`Mesh`](moxygen/api-Base.md#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_mesh)` > `[`meshes`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a7d3f368a8e407c602170ce356b3c5945) 

Contains all sub-meshes and their respective materials.

#### `public uint32_t `[`geometryIndex`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1ab185f8dac7b31f826e611b5a24cb30ce) 

A unique index required by the acceleration structures.

#### `public std::string_view `[`path`](#struct_r_a_y_e_x___n_a_m_e_s_p_a_c_e_1_1_geometry_1a503d828c8cd38560e2506ace136b829e) 

The model's path, relative to the path to assets.

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

