#include "simulator.hpp"
#include <algorithm>
#include <chrono>
#include <limits>
#include "dispatchers.hpp"

queuing_system::Simulator::Simulator(size_t numSources, size_t numDevices, size_t bufferCapacity, double minArrival,
    double maxArrival, double meanServiceTime):
  buffer(bufferCapacity),
  gen(std::chrono::steady_clock::now().time_since_epoch().count()),
  pz1_dist(1.0 / meanServiceTime),
  lastDeviceIndex(numDevices - 1)
{
  for (size_t i = 0; i < numDevices; ++i)
  {
    devices.emplace_back(i + 1);
  }

  for (size_t i = 0; i < numSources; ++i)
  {
    producers.emplace_back(i + 1, gen, minArrival, maxArrival, 0.0);
  }
}

void queuing_system::Simulator::step()
{
  double nextArrival = std::numeric_limits< double >::infinity();
  for (const auto & p : producers)
  {
    nextArrival = std::min(nextArrival, p.getNextArrivalTime());
  }

  double nextDeparture = std::numeric_limits< double >::infinity();
  for (const auto & d : devices)
  {
    if (d.isBusy())
    {
      nextDeparture = std::min(nextDeparture, d.getReleaseTime());
    }
  }

  double nextEventTime = std::min(nextArrival, nextDeparture);

  if (nextEventTime == std::numeric_limits< double >::infinity())
  {
    return;
  }

  currentTime = nextEventTime;

  for (auto & p : producers)
  {
    if (p.getNextArrivalTime() == currentTime)
    {
      handleArrival(p.generateBid(), devices, buffer, lastDeviceIndex, currentTime, gen, pz1_dist);
    }
  }

  for (auto & d : devices)
  {
    if (d.isBusy() && d.getReleaseTime() == currentTime)
    {
      servedCount++;
      handleDeparture(d, buffer, currentTime, gen, pz1_dist);
    }
  }
}

void queuing_system::Simulator::run(double endTime)
{
  while (currentTime < endTime)
  {
    step();
  }
}

double queuing_system::Simulator::getCurrentTime() const noexcept
{
  return currentTime;
}

size_t queuing_system::Simulator::getServedCount() const noexcept
{
  return servedCount;
}

size_t queuing_system::Simulator::getRejectedCount() const noexcept
{
  return buffer.getRejectedCount();
}

const queuing_system::Buffer & queuing_system::Simulator::getBuffer() const noexcept
{
  return buffer;
}

const std::vector< queuing_system::Device > & queuing_system::Simulator::getDevices() const noexcept
{
  return devices;
}
