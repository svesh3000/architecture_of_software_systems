#ifndef OUTPUT_HPP
#define OUTPUT_HPP
#include "../simulator/simulator.hpp"

namespace queuing_system
{
  void printSimulationSummary(const Simulator & simulator);
  void printAutomaticSimulationSummary(const Simulator & simulator);

  void printEventCalendar(const Simulator & simulator, double arrivalEndTime);
  void printSourceStatistics(const Simulator & simulator);
  void printDeviceStatistics(const Simulator & simulator);
  void printSimulationState(const Simulator & simulator, double arrivalEndTime);
}

#endif
