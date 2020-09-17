#ifndef CAMERA_BASE_HPP
#define CAMERA_BASE_HPP

#include "pch/stdafx.hpp"

namespace RENDERER_NAMESPACE
{
  /// A minimal camera implementation.
  /// 
  /// This class acts like an interface for the client by providing the most important camera-related matrices as well as the camera's position, which are required by the rendering API.
  /// ### Example
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.cpp
  /// // This example requires the client to implement a custom camera class that inherits from Camera.
  /// auto myCam = std::make_shared<CustomCamera>( 600, 500, glm::vec3{ 0.0f, 0.0f, 3.0f } );
  /// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /// @note The client has to handle keyboard related camera changes inside update().
  /// @warning Do not forget to re-calculate the view or projection matrix if the camera or the window have changed.
  /// @see updateViewMatrix(), updateProjectionMatrix()
  /// @ingroup Base
  class Camera
  {
  public:
    /// @param width The width of the viewport.
    /// @param height The height of the viewport.
    /// @param position The position of your camera.
    RX_API Camera( int width, int height, const glm::vec3& position = { 0.0f, 0.0f, 3.0f } );

    RX_API ~Camera( ) = default;

    /// Is used to update camera vectors etc.
    /// 
    /// The client has to implement this function to work like intended.
    /// @note The function will be called every tick.
    virtual void update( ) { };

    /// Returns the camera's current position.
    /// @return The camera's position.
    RX_API inline const glm::vec3& getPosition( ) const { return m_position; }

    /// Is used to set the camera's position.
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
    inline const glm::mat4& getViewMatrix( ) const { return m_view; }

    /// @return The projection matrix.
    inline const glm::mat4& getProjectionMatrix( ) const { return m_projection; }

    /// @return The view matrix inversed.
    inline const glm::mat4& getViewInverseMatrix( ) const { return m_viewInverse; }
    
    /// @return The projection matrix inversed.
    inline const glm::mat4& getProjectionInverseMatrix( ) const { return m_projectionInverse; }
    
    /// Re-calculates the camera's view matrix as well as the inversed view matrix.
    RX_API void updateViewMatrix( );

    /// Re-calculates the camera's projection matrix as well as the inversed projection matrix.
    RX_API void updateProjectionMatrix( );

    /// Processes mouse input (default implementation).
    /// @param xOffset The difference of the offset on the x-axis and the previous offset.
    /// @param yOffset The difference of the offset on the y-axis and the previous offset.
    RX_API virtual void processMouse( float xOffset, float yOffset );

    bool m_updateView = true;
    bool m_updateProj = true;

  protected:
    /// Updates the camera vectors.
    /// @note Only needs to be called if mouse was moved.
    RX_API void updateVectors( );

    int m_width; ///< The width of the viewport.
    int m_height; ///< The height of the viewport.

    glm::vec3 m_position; ///< The camera's position.
    
    glm::mat4 m_view = glm::mat4( 1.0f ); ///< The view matrix.
    glm::mat4 m_projection = glm::mat4( 1.0f ); ///< The projection matrix

    glm::mat4 m_viewInverse = glm::mat4( 1.0f ); ///< The view matrix inversed.
    glm::mat4 m_projectionInverse = glm::mat4( 1.0f ); ///< The projection matrix inversed.

    glm::vec3 m_worldUp = { 0.0f, 1.0f, 0.0f }; ///< The world up vector.
    glm::vec3 m_up = { }; ///< The local up vector.
    glm::vec3 m_right = { }; ///< The local right vector.
    glm::vec3 m_front = { }; ///< The viewing direction.

    float m_yaw = -90.0f; ///< The yaw (left and right).
    float m_pitch = 0.0f; ///< The pitch (down and up).
    float m_sensitivity = 0.06f; ///< The mouse sensitivity.
    float m_fov = 45.0f; ///< The field of view.
  };
}

#endif // CAMERA_BASE_HPP