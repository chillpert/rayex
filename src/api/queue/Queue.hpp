#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "pch/stdafx.hpp"

namespace RX
{
  typedef enum QueueCapability
  {
    GRAPHICS = 0x01,
    PRESENT = 0x02,
    TRANSFER = 0x04
  } QueueCapability;

  class Queue
  {
  public:
    Queue(uint32_t index, int capability, float priority) :
      m_queue(nullptr), m_index(index), m_capbility(capability), m_priority(priority) { }

    inline vk::Queue& getQueue() { return m_queue; }
    inline uint32_t getIndex() { return m_index; }
    inline float getPriority() { return m_priority; }
    int getCapability() { return m_capbility; }

    friend std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Queue> queue);

  private:
    vk::Queue m_queue;
    uint32_t m_index;
    float m_priority;
    int m_capbility;
  };
}

#endif // QUEUE_HPP