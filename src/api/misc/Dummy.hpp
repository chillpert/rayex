#ifndef DUMMY_HPP
#define DUMMY_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  // Contains all data needed for the creation process and possibly byproducts as well.
  struct DummyInfo { };

  class Dummy
  {
  public:
    ~Dummy()
    {
      destroy();
    }

    inline VkInstance get() { return m_dummy; }
    inline DummyInfo getInfo() { return m_info; }

    void initialize(DummyInfo& info)
    {
      m_info = info;

      // Use VK_CREATE macro even if you don't actually create a resource.
      VkResult res = VK_SUCCESS;
      VK_CREATE(res, "Dummy");
    }

    void destroy()
    {
      // Use the VK_DESTROY or VK_FREE macro
    }

  private:
    VkInstance m_dummy;
    DummyInfo m_info;

    // Make sure that non-initialized resources won't get destroyed via RAII.
    bool m_created = false;
  };
}

#endif // DUMMY_HPP