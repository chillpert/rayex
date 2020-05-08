#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  namespace KEY
  {
    bool w = false;
    bool a = false;
    bool s = false;
    bool d = false;
    bool c = false;
    bool space = false;
  }

  class Camera
  {
  public:
    RX_API Camera(float width, float height);
  
    RX_API void processKeyboard();
    RX_API void processMouse(float xoffset, float yoffset);

    RX_API void update();

    RX_API inline glm::mat4 getViewMatrix() { return glm::lookAt(m_position, m_position + m_front, m_worldUp); }
    RX_API inline glm::mat4 getProjectionMatrix() { return glm::perspective(glm::radians(m_fov), m_width / m_height, 0.1f, 100.0f); }

    RX_API void setScreenDimensions(int width, int height);

  private:
    glm::fvec3 m_worldUp;
    glm::fvec3 m_up;
    glm::fvec3 m_right;

    // viewport dimensions
    float m_width;
    float m_height;

    RX_API void updateVectors();
  
  public:
    glm::fvec3 m_position;
    glm::fvec3 m_front;

    float m_yaw;
    float m_pitch;
    float m_sensitivity;
    float m_fov;
  };
}

#endif // CAMERA_HPPs