#include "step_mode.hpp"
#include <iostream>
#include <string>
#include "output.hpp"

void queuing_system::runStepMode(Simulator & simulator, double arrivalEndTime)
{
  std::cout << "\n===== STEP-BY-STEP SIMULATION =====\n";

  printSimulationState(simulator, arrivalEndTime);

  std::string command;
  while (true)
  {
    std::cout << "\nPress Enter to continue or q to quit: ";

    if (!std::getline(std::cin, command))
    {
      break;
    }

    if (command == "q" || command == "Q")
    {
      break;
    }

    if (!command.empty())
    {
      std::cout << "Unknown command. Press Enter to continue or q to quit.\n";
      continue;
    }

    if (!simulator.step(arrivalEndTime))
    {
      std::cout << "\nSimulation completed.\n";
      break;
    }

    printSimulationState(simulator, arrivalEndTime);
  }

  printSimulationSummary(simulator);
}
