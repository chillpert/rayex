#ifndef BASE_COMPONENT_HPP
#define BASE_COMPONENT_HPP

#include "pch/stdafx.hpp"

#ifdef RX_DEBUG
  #define RX_ASSERT_INITIALIZED(message) assertInitialized(message)
  #define RX_ASSERT_NOT_INITIALIZED(message) assertNotInitialized(message)
  #define RX_ASSERT_DESTRUCTION assertDestruction();
  #define RX_INITIALIZATION_CALLBACK initializationCallback();
#else
  #define RX_ASSERT_INITIALIZED(message)
  #define RX_ASSERT_NOT_INITIALIZED(message)
  #define RX_ASSERT_DESTRUCTION
  #define RX_INITIALIZATION_CALLBACK
#endif

namespace RX
{
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