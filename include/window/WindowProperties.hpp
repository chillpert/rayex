#ifndef WINDOWPROPERTIES_HPP
#define WINDOWPROPERTIES_HPP

namespace RX
{
  class WindowProperties
  {
  public:


    float time;
    float deltaTime;
    
    int m_width = 500;
    int m_height = 400;

    const char* m_title = "My application";
  };
}

#endif // WINDOWPROPERTIES_HPP