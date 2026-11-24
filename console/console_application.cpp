#include "console_application.hpp"
#include <iostream>
#include <limits>
#include "../simulator/simulator.hpp"
#include "automatic_mode.hpp"
#include "input.hpp"
#include "step_mode.hpp"

int queuing_system::runConsoleApplication()
{
  try
  {
    const SimulationParameters parameters = readSimulationParameters();

    const int mode = readMode();
    if (mode == 1)
    {
      runAutomaticMode(parameters.numSources, parameters.numDevices, parameters.bufferCapacity, parameters.minArrival,
          parameters.maxArrival, parameters.meanServiceTime);
    }
    else
    {
      const double arrivalEndTime = readDouble("Arrival generation end time: ");
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

      Simulator simulator(parameters.numSources, parameters.numDevices, parameters.bufferCapacity,
          parameters.minArrival, parameters.maxArrival, parameters.meanServiceTime);

      runStepMode(simulator, arrivalEndTime);
    }
  }
  catch (const std::exception & exception)
  {
    std::cerr << "\nERROR: " << exception.what() << '\n';
    return 1;
  }

  return 0;
}
