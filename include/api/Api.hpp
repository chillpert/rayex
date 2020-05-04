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
    RX_API virtual ~Api();

    RX_API virtual bool initialize(std::shared_ptr<Window> window) = 0;
    RX_API virtual void update() = 0;
    RX_API virtual void render() = 0;
    RX_API virtual void clean() = 0;

  protected:
    std::shared_ptr<Window> m_window;
  };
}

#endif // API_HPP