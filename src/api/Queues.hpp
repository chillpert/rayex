#ifndef QUEUES_HPP
#define QUEUES_HPP

#include "stdafx.hpp"

namespace rx
{
  class Queues
  {
  public:
    void init( );

    void retrieveHandles( );

    static bool isComplete( vk::PhysicalDevice physicalDevice );
    static bool hasDedicatedTransferQueueFamily( vk::PhysicalDevice physicalDevice );

  private:
    std::optional<uint32_t> m_graphicsFamilyIndex;
    std::optional<uint32_t> m_transferFamilyIndex;
  };
}

#endif // QUEUES_HPP