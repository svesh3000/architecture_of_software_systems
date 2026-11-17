#include "producer.hpp"

queuing_system::Producer::Producer(
    size_t id, std::mt19937 & generator, double minTime, double maxTime, double startTime):
  id(id),
  gen(generator),
  dist(minTime, maxTime),
  nextArrivalTime(startTime + dist(gen))
{}

queuing_system::Bid queuing_system::Producer::generateBid()
{
  Bid newBid;
  newBid.id = ++bidCounter;
  newBid.sourceId = id;
  newBid.arrivalTime = nextArrivalTime;

  nextArrivalTime += dist(gen);

  return newBid;
}

double queuing_system::Producer::getNextArrivalTime() const noexcept
{
  return nextArrivalTime;
}
