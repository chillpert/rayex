#ifndef BASE_COMPONENT_HPP
#define BASE_COMPONENT_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  class BaseComponent
  {
  protected:
    BaseComponent(const char* componentName);

    // Used to ensure the respective component has been initialized.
    void assertInitialized(const char* message);

    // Used to ensure the respective component has not been initialized yet.
    void assertNotInitialized(const char* message);

    // Ensures that a component is only deleted if it is initialized at that point of time.
    void assertDestruction();

    void initializationCallback();

  private:
    bool m_initialized;
    const char* m_componentName;
  };
}

#endif // BASE_COMPONENT_HPP