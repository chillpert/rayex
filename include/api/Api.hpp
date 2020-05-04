#ifndef API_HPP
#define API_HPP

#include "window/Window.hpp"

#include <memory>
#include <iostream>

namespace RX
{
  class Api
  {
  public:
    RX_API virtual bool initialize() = 0;
    RX_API virtual void update() = 0;
    RX_API virtual void render() = 0;
    RX_API virtual void clean() = 0;
  };
}

#endif // API_HPP