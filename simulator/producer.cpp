#include "producer.hpp"

namespace queuing_system
{
  Producer::Producer(size_t id, std::mt19937 & gen, double minTime, double maxTime, double startTime):
    id(id),
    gen(gen),
    dist(minTime, maxTime),
    nextArrivalTime(startTime)
  {}

  Bid Producer::generateBid()
  {
    Bid bid;

    bid.id = ++bidCounter;
    bid.sourceId = id;
    bid.arrivalTime = nextArrivalTime;

    nextArrivalTime += dist(gen);

    return bid;
  }

  double Producer::getNextArrivalTime() const noexcept
  {
    return nextArrivalTime;
  }

  size_t Producer::getId() const noexcept
  {
    return id;
  }
}
