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
    const std::deque< Bid > & getQueue() const noexcept;

    bool push(const Bid & bid, Bid & rejectedBid);
    Bid extract();
    const Bid & front() const;

  private:
    size_t capacity;
    std::deque< Bid > queue;
    size_t rejected_count = 0;
  };
}

#endif
