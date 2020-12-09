#ifndef CUSTOM_CAMERA_HPP
#define CUSTOM_CAMERA_HPP

#include "Keys.hpp"
#include "Rayex.hpp"

class CustomCamera : public rx::Camera
{
public:
  CustomCamera( int width, int height, const glm::vec3& position ) :
    rx::Camera( width, height, position )
  {
  }

  void processKeyboard( ) override
  {
    const float defaultSpeed  = 2.5F;
    static float currentSpeed = defaultSpeed;
    float finalSpeed          = currentSpeed * rx::Time::getDeltaTime( );

    if ( Key::eLeftShift )
    {
      currentSpeed = 10.0F;
    }
    else if ( Key::eLeftCtrl )
    {
      currentSpeed = 0.5F;
    }
    else
    {
      currentSpeed = defaultSpeed;
    }

    if ( Key::eW )
    {
      _position += _front * finalSpeed;
      _updateView = true;
    }

    if ( Key::eS )
    {
      _position -= _front * finalSpeed;
      _updateView = true;
    }

    if ( Key::eA )
    {
      _position -= _right * finalSpeed;
      _updateView = true;
    }

    if ( Key::eD )
    {
      _position += _right * finalSpeed;
      _updateView = true;
    }

    if ( Key::eC )
    {
      _position.y -= finalSpeed / 2.0F;
      _updateView = true;
    }

    if ( Key::eSpace )
    {
      _position.y += finalSpeed / 2.0F;
      _updateView = true;
    }
  }
};

#endif // CUSTOM_CAMERA_HPP
