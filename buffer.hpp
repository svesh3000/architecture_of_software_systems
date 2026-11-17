#ifndef BUFFER_HPP
#define BUFFER_HPP
#include <deque>
#include "bid.hpp"

namespace queuing_system
{
  class Buffer
  {
  public:
    explicit Buffer(size_t k);

    bool isFull() const noexcept;
    bool isEmpty() const noexcept;
    size_t size() const noexcept;
    size_t getRejectedCount() const noexcept;

    void push(const Bid & bid);
    void pop() noexcept;
    const Bid & front() const;

  private:
    size_t capacity;
    std::deque< Bid > queue;
    size_t rejected_count = 0;
  };
}

#endif
