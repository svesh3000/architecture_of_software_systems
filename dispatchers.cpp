#include "dispatchers.hpp"
#include "device.hpp"

queuing_system::Device * queuing_system::selectDevice(std::vector< Device > & devices, size_t & lastIndex)
{
  size_t n = devices.size();
  for (size_t i = 0; i < n; ++i)
  {
    size_t idx = (lastIndex + 1 + i) % n;
    if (!devices[idx].isBusy())
    {
      lastIndex = idx;
      return &devices[idx];
    }
  }
  return nullptr;
}

bool queuing_system::handleArrival(const Bid & newBid, std::vector< Device > & devices, Buffer & buffer,
    size_t & lastIndex, double currentTime, std::mt19937 & gen, std::exponential_distribution< double > & pz1_dist,
    Bid & rejectedBid)
{
  Device * freeDevice = selectDevice(devices, lastIndex);
  if (freeDevice != nullptr)
  {
    const double serviceTime = pz1_dist(gen);
    freeDevice->putOnService(newBid, currentTime, serviceTime);
    return false;
  }

  return buffer.push(newBid, rejectedBid);
}

void queuing_system::handleDeparture(Device & device, Buffer & buffer, double currentTime, std::mt19937 & gen,
    std::exponential_distribution< double > & pz1_dist)
{
  device.freeDevice();
  if (!buffer.isEmpty())
  {
    double serviceTime = pz1_dist(gen);
    device.putOnService(buffer.extract(), currentTime, serviceTime);
  }
}

