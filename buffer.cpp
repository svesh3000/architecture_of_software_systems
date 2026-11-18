#include "buffer.hpp"
#include <cassert>
#include <utility>

queuing_system::Buffer::Buffer(size_t k):
  capacity(k)
{}

bool queuing_system::Buffer::isFull() const noexcept
{
  return queue.size() >= capacity;
}
bool queuing_system::Buffer::isEmpty() const noexcept
{
  return queue.empty();
}
size_t queuing_system::Buffer::size() const noexcept
{
  return queue.size();
}
size_t queuing_system::Buffer::getRejectedCount() const noexcept
{
  return rejected_count;
}

const std::deque< queuing_system::Bid > & queuing_system::Buffer::getQueue() const noexcept
{
  return queue;
}

bool queuing_system::Buffer::push(const Bid & bid, Bid & rejectedBid)
{
  if (isFull())
  {
    rejectedBid = queue.back();
    queue.pop_back();
    ++rejected_count;
    queue.push_back(bid);
    return true;
  }

  queue.push_back(bid);
  return false;
}

queuing_system::Bid queuing_system::Buffer::extract()
{
  assert(!queue.empty());
  Bid b = std::move(queue.front());
  queue.pop_front();
  return b;
}

const queuing_system::Bid & queuing_system::Buffer::front() const
{
  assert(!queue.empty());
  return queue.front();
}
