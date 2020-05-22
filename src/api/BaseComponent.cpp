#include "BaseComponent.hpp"

namespace RX
{
  const char* messagePrefix = "Wrong Execution Order: ";

  BaseComponent::BaseComponent(const char* componentName) :
    m_initialized(false), 
    m_componentName(componentName) { }

  // Used to ensure the respective component has been initialized.
  void BaseComponent::assertInitialized(const char* message) const
  {
    if (!m_initialized)
    {
      char result[100];
      strcpy_s(result, messagePrefix);
      strcat_s(result, m_componentName);
      strcat_s(result, ": ");
      strcat_s(result, message);
      RX_ERROR(result);
    }
  }

  // Used to ensure the respective component has not been initialized yet.
  void BaseComponent::assertNotInitialized(const char* message) const
  {
    if (m_initialized)
    {
      char result[100];
      strcpy_s(result, messagePrefix);
      strcat_s(result, m_componentName);
      strcat_s(result, ": ");
      strcat_s(result, message);
      RX_ERROR(result);
    }
  }

  // Ensures that a component is only deleted if it is initialized at that point of time.
  void BaseComponent::assertDestruction()
  {
    if (!m_initialized)
    {
      char result[100];
      strcpy_s(result, messagePrefix);
      strcat_s(result, m_componentName);
      RX_ERROR(result);
    }

    m_initialized = false;
    VK_LOG("Destroyed " << m_componentName);
  }

  void BaseComponent::initializationCallback()
  {
    if (m_initialized)
    {
      char result[100];
      strcpy_s(result, messagePrefix);
      strcat_s(result, m_componentName);
      RX_ERROR(result);
    }

    m_initialized = true;
    VK_LOG("Created " << m_componentName);
  }
}