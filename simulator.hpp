#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP
#include <cstddef>
#include <random>
#include <vector>
#include "buffer.hpp"
#include "device.hpp"
#include "producer.hpp"

namespace queuing_system
{
  class Simulator
  {
  public:
    Simulator(size_t numSources, size_t numDevices, size_t bufferCapacity, double minArrival, double maxArrival,
        double meanServiceTime);

    void step();
    void run(double endTime);

    double getCurrentTime() const noexcept;
    size_t getServedCount() const noexcept;
    size_t getRejectedCount() const noexcept;
    const Buffer & getBuffer() const noexcept;
    const std::vector< Device > & getDevices() const noexcept;

  private:
    std::vector< Producer > producers;
    std::vector< Device > devices;
    Buffer buffer;

    std::mt19937 gen;
    std::exponential_distribution< double > pz1_dist;
    double currentTime = 0.0;

    size_t lastDeviceIndex = 0;
    size_t servedCount = 0;
  };
}

#endif
