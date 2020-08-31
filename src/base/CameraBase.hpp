#ifndef CAMERA_BASE_HPP
#define CAMERA_BASE_HPP

#include "pch/stdafx.hpp"

namespace rx
{
  class CameraBase
  {
  public:
    CameraBase( const glm::ivec2& screenSize, const glm::vec3& position = glm::vec3( 0.0f ) ) :
      m_screenSize( screenSize ),
      m_position( position ),
      m_view( glm::mat4( 1.0f ) ),
      m_projection( glm::mat4( 1.0f ) )
    { }

    virtual ~CameraBase( ) = default;
    virtual void update( ) { };

    inline const glm::vec3& getPosition( ) const { return m_position; }

    void setPosition( const glm::vec3& position )
    {
      m_position = position;
    }

    void setScreenSize( const glm::ivec2& screenSize )
    {
      m_screenSize = screenSize;
    }

    virtual inline const glm::mat4& getViewMatrix( ) const { return m_view; }
    virtual inline const glm::mat4& getProjectionMatrix( ) const { return m_projection; }
    
  protected:
    glm::ivec2 m_screenSize;
    glm::vec3 m_position;

    glm::mat4 m_view;
    glm::mat4 m_projection;
  };
}

#endif // CAMERA_BASE_HPP