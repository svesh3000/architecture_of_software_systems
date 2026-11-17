#include "buffer.hpp"
#include <cassert>

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

void queuing_system::Buffer::push(const Bid & bid)
{
  if (isFull())
  {
    queue.pop_back();
    rejected_count++;
  }
  queue.push_back(bid);
}

void queuing_system::Buffer::pop() noexcept
{
  assert(!queue.empty());
  queue.pop_front();
}

const queuing_system::Bid & queuing_system::Buffer::front() const
{
  assert(!queue.empty());
  return queue.front();
}
