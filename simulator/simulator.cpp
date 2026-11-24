#include "simulator.hpp"
#include <limits>
#include <random>
#include <stdexcept>
#include "dispatchers.hpp"

namespace
{
  size_t validateDeviceCount(size_t numDevices)
  {
    if (numDevices == 0)
    {
      throw std::invalid_argument("The number of devices must be greater than zero.");
    }
    return numDevices - 1;
  }

  double validateMeanServiceTime(double meanServiceTime)
  {
    if (meanServiceTime <= 0.0)
    {
      throw std::invalid_argument("The mean service time must be greater than zero.");
    }
    return meanServiceTime;
  }
}

queuing_system::Simulator::Simulator(size_t numSources, size_t numDevices, size_t bufferCapacity, double minArrival,
    double maxArrival, double meanServiceTime):
  buffer(bufferCapacity),
  gen(std::random_device{}()),
  pz1_dist(1.0 / validateMeanServiceTime(meanServiceTime)),
  lastDeviceIndex(validateDeviceCount(numDevices))
{
  if (numSources == 0)
  {
    throw std::invalid_argument("The number of sources must be greater than zero.");
  }

  if (minArrival >= maxArrival)
  {
    throw std::invalid_argument("The minimum arrival time must not exceed the maximum arrival time.");
  }

  producers.reserve(numSources);
  for (size_t i = 0; i < numSources; ++i)
  {
    producers.emplace_back(i + 1, gen, minArrival, maxArrival, 0.0);
  }

  devices.reserve(numDevices);
  for (size_t i = 0; i < numDevices; ++i)
  {
    devices.emplace_back(i + 1);
  }

  sourceStatistics.resize(numSources);
}

bool queuing_system::Simulator::step(double arrivalEndTime)
{
  return step(arrivalEndTime, std::numeric_limits< size_t >::max());
}

bool queuing_system::Simulator::step(double arrivalEndTime, size_t maxGeneratedCount)
{
  double nextArrivalTime = std::numeric_limits< double >::max();
  double nextDepartureTime = std::numeric_limits< double >::max();
  Producer * nextProducer = nullptr;
  Device * nextDevice = nullptr;

  if (generatedCount < maxGeneratedCount)
  {
    for (auto & producer : producers)
    {
      const double arrivalTime = producer.getNextArrivalTime();
      if (arrivalTime < nextArrivalTime)
      {
        nextArrivalTime = arrivalTime;
        nextProducer = &producer;
      }
    }

    if (nextArrivalTime > arrivalEndTime)
    {
      nextProducer = nullptr;
    }
  }

  for (auto & device : devices)
  {
    if (device.isBusy() && device.getReleaseTime() < nextDepartureTime)
    {
      nextDepartureTime = device.getReleaseTime();
      nextDevice = &device;
    }
  }

  if (nextProducer == nullptr && nextDevice == nullptr)
  {
    return false;
  }

  if (nextProducer != nullptr && nextArrivalTime <= nextDepartureTime)
  {
    currentTime = nextArrivalTime;
    const Bid newBid = nextProducer->generateBid();
    ++generatedCount;
    sourceStatistics.at(newBid.sourceId - 1).recordGeneratedBid();

    Bid rejectedBid;
    const bool wasBidRejected
        = handleArrival(newBid, devices, buffer, lastDeviceIndex, currentTime, gen, pz1_dist, rejectedBid);

    if (wasBidRejected)
    {
      sourceStatistics.at(rejectedBid.sourceId - 1).recordRejectedBid();
    }
  }
  else
  {
    currentTime = nextDepartureTime;
    const Bid completedBid = nextDevice->getCurrentBid();
    const double serviceTime = nextDevice->getReleaseTime() - nextDevice->getServiceStartTime();
    const double waitingTime = nextDevice->getServiceStartTime() - completedBid.arrivalTime;
    sourceStatistics.at(completedBid.sourceId - 1).recordCompletedBid(waitingTime, serviceTime);
    ++servedCount;
    handleDeparture(*nextDevice, buffer, currentTime, gen, pz1_dist);
  }

  return true;
}

void queuing_system::Simulator::run(double arrivalEndTime)
{
  while (step(arrivalEndTime))
  {
  }
}

void queuing_system::Simulator::runForRequests(size_t targetGeneratedCount)
{
  while (step(std::numeric_limits< double >::max(), targetGeneratedCount))
  {
  }
}

double queuing_system::Simulator::getCurrentTime() const noexcept
{
  return currentTime;
}

size_t queuing_system::Simulator::getGeneratedCount() const noexcept
{
  return generatedCount;
}

size_t queuing_system::Simulator::getServedCount() const noexcept
{
  return servedCount;
}

size_t queuing_system::Simulator::getRejectedCount() const noexcept
{
  return buffer.getRejectedCount();
}

size_t queuing_system::Simulator::getInServiceCount() const noexcept
{
  size_t count = 0;

  for (const auto & device : devices)
  {
    if (device.isBusy())
    {
      ++count;
    }
  }

  return count;
}

const queuing_system::Buffer & queuing_system::Simulator::getBuffer() const noexcept
{
  return buffer;
}

const std::vector< queuing_system::Device > & queuing_system::Simulator::getDevices() const noexcept
{
  return devices;
}

const std::vector< queuing_system::Producer > & queuing_system::Simulator::getProducers() const noexcept
{
  return producers;
}

const std::vector< queuing_system::SourceStatistics > & queuing_system::Simulator::getSourceStatistics() const noexcept
{
  return sourceStatistics;
}

double queuing_system::Simulator::getDeviceUtilization(size_t deviceIndex) const noexcept
{
  if (deviceIndex >= devices.size() || currentTime <= 0.0)
  {
    return 0.0;
  }

  const Device & device = devices[deviceIndex];
  double totalBusyTime = device.getTotalBusyTime();

  if (device.isBusy())
  {
    totalBusyTime += currentTime - device.getServiceStartTime();
  }

  return totalBusyTime / currentTime;
}
