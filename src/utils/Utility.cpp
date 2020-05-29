#include "Utility.hpp"

namespace RX
{
  namespace Utils
  {
    bool log = true;

    size_t createCounter = 0;
    size_t destroyCounter = 0;
    
    size_t allocateCounter = 0;
    size_t freeCounter = 0;
  }
}