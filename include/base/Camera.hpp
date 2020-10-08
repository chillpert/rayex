#ifndef CAMERA_BASE_HPP
#define CAMERA_BASE_HPP

#include "pch/stdafx.hpp"

namespace RAYEXEC_NAMESPACE
{
  /// A minimal camera implementation.
  ///
  /// This class acts like an interface for the user by providing the most important camera-related matrices as well as the camera's position, which are required by the rendering API.
  /// ### Example
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
  /// // This example requires the user to implement a custom camera class that inherits from Camera.
  /// auto myCam = std::make_shared<CustomCamera>( 600, 500, glm::vec3{ 0.0f, 0.0f, 3.0f } );
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// @note The user has to handle keyboard related camera changes inside update().
  /// @warning Do not forget to re-calculate the view or projection matrix if the camera or the window have changed. See updateViewMatrix(), updateProjectionMatrix() or updateView and updateProj respectively.
  /// @ingroup Base
  class Camera
  {
  public:
    /// @param width The width of the viewport.
    /// @param height The height of the viewport.
    /// @param position The position of your camera.
    RX_API Camera( int width, int height, const glm::vec3& position = { 0.0F, 0.0F, 3.0F } );

    RX_API virtual ~Camera( ) = default;

    RX_API Camera( const Camera& ) = default;
    Camera( const Camera&& )       = delete;

    RX_API auto operator=( const Camera& ) -> Camera& = default;
    auto operator=( const Camera && ) -> Camera& = delete;

    /// Is used to update camera vectors etc.
    ///
    /// The user has to override this function for the camera to work like intended.
    /// @note The function will be called every tick.
    virtual void update( ) { };

    /// @return Returns the camera's position.
    [[nodiscard]] RX_API inline auto getPosition( ) const -> const glm::vec3& { return position; }

    /// Is used to change the camera's position.
    /// @param position The new camera position.
    RX_API void setPosition( const glm::vec3& position );

    /// Is used to set a size for the camera that fits the viewport dimensions.
    /// @param width The width of the viewport.
    /// @param height The height of the viewport.
    RX_API void setSize( int width, int height );

    /// Is used to set the camera's field of view.
    /// @param fov The new field of view.
    RX_API void setFov( float fov );

    /// Is used to set the mouse sensitivity.
    /// @param sensitivity The new mouse sensitivity.
    RX_API void setSensitivity( float sensitivity );

    /// @return The view matrix.
    [[nodiscard]] inline auto getViewMatrix( ) const -> const glm::mat4& { return view; }

    /// @return The projection matrix.
    [[nodiscard]] inline auto getProjectionMatrix( ) const -> const glm::mat4& { return projection; }

    /// @return The view matrix inversed.
    [[nodiscard]] inline auto getViewInverseMatrix( ) const -> const glm::mat4& { return viewInverse; }

    /// @return The projection matrix inversed.
    [[nodiscard]] inline auto getProjectionInverseMatrix( ) const -> const glm::mat4& { return projectionInverse; }

    /// Re-calculates the camera's view matrix as well as the inversed view matrix.
    RX_API void updateViewMatrix( );

    /// Re-calculates the camera's projection matrix as well as the inversed projection matrix.
    RX_API void updateProjectionMatrix( );

    /// Processes mouse input (default implementation).
    /// @param xOffset The difference of the current offset on the x-axis and the previous offset.
    /// @param yOffset The difference of the current offset on the y-axis and the previous offset.
    RX_API virtual void processMouse( float xOffset, float yOffset );

    bool updateView = true; ///< Keeps track of whether or not to udpate the view matrix.
    bool updateProj = true; ///< Keeps track of whether or not to udpate the projection matrix.

  protected:
    /// Updates the camera vectors.
    /// @note Only needs to be called if mouse was moved.
    RX_API void updateVectors( );

    int width;  ///< The width of the viewport.
    int height; ///< The height of the viewport.

    glm::vec3 position; ///< The camera's position.

    glm::mat4 view       = glm::mat4( 1.0F ); ///< The view matrix.
    glm::mat4 projection = glm::mat4( 1.0F ); ///< The projection matrix

    glm::mat4 viewInverse       = glm::mat4( 1.0F ); ///< The view matrix inversed.
    glm::mat4 projectionInverse = glm::mat4( 1.0F ); ///< The projection matrix inversed.

    glm::vec3 worldUp = { 0.0F, 1.0F, 0.0F }; ///< The world up vector.
    glm::vec3 up      = { };                  ///< The local up vector.
    glm::vec3 right   = { };                  ///< The local right vector.
    glm::vec3 front   = { };                  ///< The viewing direction.

    float yaw         = -90.0F; ///< The yaw (left and right).
    float pitch       = 0.0F;   ///< The pitch (down and up).
    float sensitivity = 0.06F;  ///< The mouse sensitivity.
    float fov         = 45.0F;  ///< The field of view.
  };
} // namespace RAYEXEC_NAMESPACE

#endif // CAMERA_BASE_HPP