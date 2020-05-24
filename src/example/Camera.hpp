#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

namespace KEY
{
  extern bool w;
  extern bool a;
  extern bool s;
  extern bool d;
  extern bool c;
  extern bool space;
}

class Camera
{
public:
  Camera(int width, int height);

  void processKeyboard();
  void processMouse(float xoffset, float yoffset);

  void update();

  inline glm::mat4 getViewMatrix() { return glm::lookAt(m_position, m_position + m_front, m_worldUp); }
  inline glm::mat4 getProjectionMatrix() { return glm::perspective(glm::radians(m_fov), m_width / m_height, 0.1f, 100.0f); }

  void setScreenDimensions(int width, int height);

private:
  glm::fvec3 m_worldUp;
  glm::fvec3 m_up;
  glm::fvec3 m_right;

  // viewport dimensions
  float m_width;
  float m_height;

  void updateVectors();

public:
  glm::fvec3 m_position;
  glm::fvec3 m_front;

  float m_yaw;
  float m_pitch;
  float m_sensitivity;
  float m_fov;
};


#endif // CAMERA_HPPs