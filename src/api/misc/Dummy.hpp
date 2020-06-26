#ifndef DUMMY_HPP
#define DUMMY_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  // Contains all data needed for the creation process.
  struct DummyInfo
  { 
  
  };

  class Dummy
  {
  public:
    // Make sure no body can use this class
    Dummy() = default;
    Dummy(DummyInfo& info)
    {
      init(info);
    }

    Dummy(DummyInfo&& info)
    {
      init(info);
    }

    // RAII destruction
    ~Dummy()
    {
      destroy();
    }

    inline uint32_t get() { return m_dummy; }

    void init(DummyInfo& info)
    {
      m_info = info;
    }

    void init(DummyInfo&& info)
    {
      init(info);
    }

    void destroy()
    {
      
    }

  private:
    uint32_t m_dummy;

    DummyInfo m_info;
  };
}

#endif // DUMMY_HPP