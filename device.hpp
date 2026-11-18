#ifndef DEVICE_HPP
#define DEVICE_HPP
#include <cassert>
#include "bid.hpp"

namespace queuing_system
{
  class Device
  {
  public:
    explicit Device(size_t deviceId);

    void putOnService(const Bid & bid, double currentTime, double time);
    void freeDevice() noexcept;

    size_t getId() const noexcept;
    bool isBusy() const noexcept;
    double getReleaseTime() const noexcept;
    const Bid & getCurrentBid() const noexcept;
    double getServiceStartTime() const noexcept;
    double getTotalBusyTime() const noexcept;

  private:
    size_t id;
    bool busy = false;
    double releaseTime = 0.0;
    Bid currentBid;
    double serviceStartTime = 0.0;
    double totalBusyTime = 0.0;
  };
}

#endif
