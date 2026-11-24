#ifndef INPUT_HPP
#define INPUT_HPP
#include <cstddef>
#include <string>

namespace queuing_system
{
  struct SimulationParameters
  {
    size_t numSources;
    size_t numDevices;
    size_t bufferCapacity;

    double minArrival;
    double maxArrival;
    double meanServiceTime;
  };

  SimulationParameters readSimulationParameters();

  size_t readSize(const std::string & prompt);
  double readDouble(const std::string & prompt);
  int readMode();
}

#endif
