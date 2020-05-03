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
    virtual bool initialize() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
  };
}

#endif // API_HPP