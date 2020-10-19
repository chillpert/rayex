#ifndef CUSTOM_CAMERA_HPP
#define CUSTOM_CAMERA_HPP

#include "Keys.hpp"
#include "Rayex.hpp"

class CustomCamera : public rx::Camera
{
public:
  CustomCamera( int width, int height, const glm::vec3& position ) :
    rx::Camera( width, height, position ) {}

  void processKeyboard( ) override
  {
    const float defaultSpeed  = 2.5F;
    static float currentSpeed = defaultSpeed;
    float finalSpeed          = currentSpeed * rx::Time::getDeltaTime( );

    if ( Key::eLeftShift )
    {
      currentSpeed = 10.0F;
    }
    else
    {
      currentSpeed = defaultSpeed;
    }

    if ( Key::eW )
    {
      this->position += this->front * finalSpeed;
      this->updateView = true;
    }

    if ( Key::eS )
    {
      this->position -= this->front * finalSpeed;
      this->updateView = true;
    }

    if ( Key::eA )
    {
      this->position -= this->right * finalSpeed;
      this->updateView = true;
    }

    if ( Key::eD )
    {
      this->position += this->right * finalSpeed;
      this->updateView = true;
    }

    if ( Key::eC )
    {
      this->position.y -= finalSpeed / 2.0F;
      this->updateView = true;
    }

    if ( Key::eSpace )
    {
      this->position.y += finalSpeed / 2.0F;
      this->updateView = true;
    }
  }
};

#endif // CUSTOM_CAMERA_HPP
