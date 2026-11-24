#ifndef AUTOMATIC_MODE_HPP
#define AUTOMATIC_MODE_HPP
#include <cstddef>

namespace queuing_system
{
  void runAutomaticMode(size_t numSources, size_t numDevices, size_t bufferCapacity, double minArrival,
      double maxArrival, double meanServiceTime);
}

#endif
