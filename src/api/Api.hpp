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
    virtual ~Api();

    virtual void initialize(std::shared_ptr<Window> window) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void clean() = 0;
  
    enum Type { VULKAN };
     
    inline static Type getType() { return s_type; }

  protected:
    static Type s_type;
    std::shared_ptr<Window> m_window;
  };
}

#endif // API_HPP