#include "api/Api.hpp"

namespace RX
{
  Api::~Api() { }

  void Api::initialize(std::shared_ptr<Window> window)
  {
    m_window = window;
  }

  Api::Type Api::s_type;
}