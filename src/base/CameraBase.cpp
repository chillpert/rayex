#include "CameraBase.hpp"

namespace RENDERER_NAMESPACE
{
  CameraBase::CameraBase( int width, int height, const glm::vec3& position ) :
    m_width( width ),
    m_height( height ),
    m_position( position )
  {
    updateVectors( );
    updateViewMatrix( );
  }

  void CameraBase::setPosition( const glm::vec3& position )
  {
    m_position = position;

    updateViewMatrix( );
  }

  void CameraBase::setSize( int width, int height )
  {
    m_width = width;
    m_height = height;

    updateProjectionMatrix( );
  }

  void CameraBase::setFov( float fov )
  {
    m_fov = fov;

    updateProjectionMatrix( );
  }

  void CameraBase::setSensitivity( float sensitivity )
  {
    m_sensitivity = sensitivity;
  }

  void CameraBase::updateViewMatrix( )
  {
    m_view = glm::lookAt( m_position, m_position + m_front, m_worldUp );

    m_viewInverse = glm::inverse( m_view );
  }

  void CameraBase::updateProjectionMatrix( )
  {
    m_projection = glm::perspective( glm::radians( m_fov ), static_cast<float>( m_width ) / static_cast<float>( m_height ), 0.1f, 100.0f );
    m_projection[1, 1] *= -1;

    m_projectionInverse = glm::inverse( m_projection );
  }

  void CameraBase::processMouse( float xOffset, float yOffset )
  {
    m_updateView = true;

    xOffset *= m_sensitivity;
    yOffset *= m_sensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    if ( m_yaw > 360.0f )
      m_yaw = fmod( m_yaw, 360.0f );

    if ( m_yaw < 0.0f )
      m_yaw = 360.0f + fmod( m_yaw, 360.0f );

    if ( m_pitch > 89.0f )
      m_pitch = 89.0f;

    if ( m_pitch < -89.0f )
      m_pitch = -89.0f;

    updateVectors( );
  }

  void CameraBase::updateVectors( )
  {
    glm::vec3 t_front;
    t_front.x = cos( glm::radians( m_yaw ) ) * cos( glm::radians( m_pitch ) );
    t_front.y = sin( glm::radians( m_pitch ) );
    t_front.z = sin( glm::radians( m_yaw ) ) * cos( glm::radians( m_pitch ) );

    m_front = glm::normalize( t_front );
    m_right = glm::normalize( glm::cross( m_front, m_worldUp ) );
    m_up = glm::normalize( glm::cross( m_right, m_front ) );
  }
}