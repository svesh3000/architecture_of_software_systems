#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP
#include <cstddef>
#include <random>
#include <vector>
#include "buffer.hpp"
#include "device.hpp"
#include "producer.hpp"
#include "source_statistics.hpp"

namespace queuing_system
{
  class Simulator
  {
  public:
    Simulator(size_t numSources, size_t numDevices, size_t bufferCapacity, double minArrival, double maxArrival,
        double meanServiceTime);

    bool step(double arrivalEndTime);
    bool step(double arrivalEndTime, size_t maxGeneratedCount);

    void run(double arrivalEndTime);
    void runForRequests(size_t targetGeneratedCount);

    double getCurrentTime() const noexcept;

    size_t getGeneratedCount() const noexcept;
    size_t getServedCount() const noexcept;
    size_t getRejectedCount() const noexcept;
    size_t getInServiceCount() const noexcept;

    const Buffer & getBuffer() const noexcept;
    const std::vector< Device > & getDevices() const noexcept;
    const std::vector< Producer > & getProducers() const noexcept;
    const std::vector< SourceStatistics > & getSourceStatistics() const noexcept;

    double getDeviceUtilization(size_t deviceIndex) const noexcept;

  private:
    Buffer buffer;

    std::mt19937 gen;
    std::exponential_distribution< double > pz1_dist;

    size_t lastDeviceIndex;

    std::vector< Producer > producers;
    std::vector< Device > devices;
    std::vector< SourceStatistics > sourceStatistics;

    double currentTime = 0.0;

    size_t generatedCount = 0;
    size_t servedCount = 0;
  };
}

#endif
