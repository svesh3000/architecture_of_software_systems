#include "input.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

size_t queuing_system::readSize(const std::string & prompt)
{
  while (true)
  {
    long long value = 0;
    std::cout << prompt;
    if (std::cin >> value && value >= 0)
    {
      return static_cast< size_t >(value);
    }

    if (std::cin.eof())
    {
      throw std::runtime_error("Input stream closed.");
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

    std::cout << "Invalid input. Please enter a non-negative integer.\n";
  }
}

double queuing_system::readDouble(const std::string & prompt)
{
  while (true)
  {
    double value = 0.0;
    std::cout << prompt;
    if (std::cin >> value && std::isfinite(value))
    {
      return value;
    }

    if (std::cin.eof())
    {
      throw std::runtime_error("Input stream closed.");
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

    std::cout << "Invalid input. Please enter a finite number.\n";
  }
}

int queuing_system::readMode()
{
  int mode = 0;

  while (true)
  {
    std::cout << "\nSelect mode:\n";
    std::cout << "1 - OR1: automatic simulation\n";
    std::cout << "2 - OD2: step-by-step simulation\n";
    std::cout << "Mode: ";

    if (!(std::cin >> mode))
    {
      if (std::cin.eof())
      {
        throw std::runtime_error("Input stream closed.");
      }

      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

      std::cerr << "Invalid input.\n";
      continue;
    }

    if (mode != 1 && mode != 2)
    {
      std::cerr << "Unknown simulation mode.\n";
      continue;
    }

    return mode;
  }
}

queuing_system::SimulationParameters queuing_system::readSimulationParameters()
{
  SimulationParameters parameters{};

  std::cout << "===== SIMULATION PARAMETERS =====\n";

  parameters.numSources = readSize("Number of sources: ");
  parameters.numDevices = readSize("Number of devices: ");
  parameters.bufferCapacity = readSize("Buffer capacity: ");

  parameters.minArrival = readDouble("Minimum arrival interval: ");
  parameters.maxArrival = readDouble("Maximum arrival interval: ");
  parameters.meanServiceTime = readDouble("Mean service time: ");

  return parameters;
}
