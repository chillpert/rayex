#include "Camera.hpp"
#include "window/Time.hpp"

namespace KEY
{
  bool w = false;
  bool a = false;
  bool s = false;
  bool d = false;
  bool c = false;
  bool space = false;
}

Camera::Camera(float width, float height) :
  m_worldUp{ 0.0f, 1.0f, 0.0f },
  m_width(width),
  m_height(height),
  m_position{ 0.0f, 0.0f, 3.0f },
  m_front{ 0.0f, 0.0f, -1.0f },
  m_yaw(-90.0f),
  m_pitch(0.0f),
  m_sensitivity(0.06f),
  m_fov(45.0f),
  m_firstMovement(true),
  m_mouseX(0.0f),
  m_mouseY(0.0f)
{
  updateVectors();
}

void Camera::update()
{
  processKeyboard();
}

void Camera::updateVectors()
{
  glm::vec3 t_front;
  t_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  t_front.y = sin(glm::radians(m_pitch));
  t_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
  m_front = glm::normalize(t_front);
  m_right = glm::normalize(glm::cross(m_front, m_worldUp));
  m_up    = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::setScreenDimensions(int width, int height)
{
  m_width = static_cast<float>(width);
  m_height = static_cast<float>(height);
}

void Camera::processKeyboard()
{
  static float speed = 1.0f;

  float finalSpeed = speed * RX::Time::getDeltaTime();
  if (KEY::w)
    m_position += m_front * finalSpeed;

  if (KEY::s)
    m_position -= m_front * finalSpeed;

  if (KEY::a)
    m_position -= m_right * finalSpeed; 

  if (KEY::d)
    m_position += m_right * finalSpeed;

  if (KEY::c)
    m_position.y -= finalSpeed / 2.0f;

  if (KEY::space)
    m_position.y += finalSpeed / 2.0f;
}

void Camera::processMouse(float xoffset, float yoffset)
{
  xoffset *= m_sensitivity;
  yoffset *= m_sensitivity;

  m_yaw   += xoffset;
  m_pitch += yoffset;

  if (m_yaw > 360.0f)
    m_yaw = fmod(m_yaw, 360.0f);

  if (m_yaw < 0.0f)
    m_yaw = 360.0f + fmod(m_yaw, 360.0f);

  if (m_pitch > 89.0f)
    m_pitch = 89.0f;

  if (m_pitch < -89.0f)
    m_pitch = -89.0f;

  updateVectors();
}