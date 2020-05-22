#ifndef BASE_COMPONENT_HPP
#define BASE_COMPONENT_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  // TODO: add macros so that in a release build these checks won't get executed.
  class BaseComponent
  {
  protected:
    BaseComponent(const char* componentName);

    // Used to ensure the respective component has been initialized.
    void assertInitialized(const char* message) const;

    // Used to ensure the respective component has not been initialized yet.
    void assertNotInitialized(const char* message) const;

    // Ensures that a component is only deleted if it is initialized at that point of time.
    void assertDestruction();

    // Needs to be called right after the initialization finished.
    void initializationCallback();

  private:
    bool m_initialized;
    // The name of the inherited class used to print more detailed error messages.
    const char* m_componentName;
  };
}

#endif // BASE_COMPONENT_HPP