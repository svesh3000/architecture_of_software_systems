#include "device.hpp"
#include <cassert>

queuing_system::Device::Device(size_t deviceId):
  id(deviceId)
{}

void queuing_system::Device::putOnService(const queuing_system::Bid & bid, double currentTime, double time)
{
  busy = true;
  serviceStartTime = currentTime;
  releaseTime = currentTime + time;
  currentBid = bid;
}

void queuing_system::Device::freeDevice() noexcept
{
  totalBusyTime += releaseTime - serviceStartTime;
  busy = false;
  releaseTime = 0.0;
  serviceStartTime = 0.0;
  currentBid = Bid{};
}

size_t queuing_system::Device::getId() const noexcept
{
  return id;
}

bool queuing_system::Device::isBusy() const noexcept
{
  return busy;
}

double queuing_system::Device::getReleaseTime() const noexcept
{
  assert(busy && "Cannot get releaseTime of a free device!");
  return releaseTime;
}

const queuing_system::Bid & queuing_system::Device::getCurrentBid() const noexcept
{
  assert(busy && "Cannot get currentBid of a free device!");
  return currentBid;
}

double queuing_system::Device::getServiceStartTime() const noexcept
{
  return serviceStartTime;
}

double queuing_system::Device::getTotalBusyTime() const noexcept
{
  return totalBusyTime;
}
