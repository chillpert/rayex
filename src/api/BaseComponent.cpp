#include "BaseComponent.hpp"

namespace RX
{
  const char* messagePrefix = "Wrong Execution Order: ";

  BaseComponent::BaseComponent(const char* componentName) :
    m_initialized(false), 
    m_componentName(componentName) { }

  // Used to ensure the respective component has been initialized.
  void BaseComponent::assertInitialized(const char* message)
  {
    if (!m_initialized)
    {
      char result[100];
      strcpy(result, messagePrefix);
      strcat(result, m_componentName);
      strcat(result, ": ");
      strcat(result, message);
      RX_ERROR(result);
    }
  }

  // Used to ensure the respective component has not been initialized yet.
  void BaseComponent::assertNotInitialized(const char* message)
  {
    if (m_initialized)
    {
      char result[100];
      strcpy(result, messagePrefix);
      strcat(result, m_componentName);
      strcat(result, ": ");
      strcat(result, message);
      RX_ERROR(result);
    }
  }

  // Ensures that a component is only deleted if it is initialized at that point of time.
  void BaseComponent::assertDestruction()
  {
    if (!m_initialized)
    {
      char result[100];
      strcpy(result, messagePrefix);
      strcat(result, m_componentName);
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
      strcpy(result, messagePrefix);
      strcat(result, m_componentName);
      RX_ERROR(result);
    }

    m_initialized = true;
  }
}