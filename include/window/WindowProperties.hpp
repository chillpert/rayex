#ifndef WINDOWPROPERTIES_HPP
#define WINDOWPROPERTIES_HPP

namespace Renderer
{
  class WindowProperties
  {
  public:
    const char* title = "My application";
    
    int width = 500;
    int height = 400;

    float time;
    float deltaTime;
  };
}

#endif // WINDOWPROPERTIES_HPP