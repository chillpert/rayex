#include "api/Api.hpp"

namespace RX
{
  Api::~Api() { }

  bool Api::initialize(std::shared_ptr<Window> window)
  {
    m_window = window;

    return true;
  }
}