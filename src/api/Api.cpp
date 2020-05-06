#include "api/Api.hpp"

namespace RX
{
  Api::Type Api::s_type;

  Api::~Api() { }

  void Api::initialize(std::shared_ptr<Window> window)
  {
    m_window = window;
  }
}