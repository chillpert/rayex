#ifndef GUI_HPP
#define GUI_HPP

namespace RX
{
  class Gui
  {
  public:
    ~Gui();

    void initialize();
    void begin();
    void render();
    void end();

    void destroy();
    void recreate();
  };
}

#endif // GUI_HPP