#include "automatic_mode.hpp"
#include <cmath>
#include <iostream>
#include "../simulator/simulator.hpp"
#include "output.hpp"

namespace
{
  constexpr double confidenceCoefficient = 1.643;
  constexpr double relativeAccuracy = 0.1;

  constexpr size_t initialRequestCount = 100;
  constexpr size_t maxIterations = 30;
  constexpr size_t maxRequestCount = 10000000;

  double getTotalRejectionProbability(const queuing_system::Simulator & simulator)
  {
    if (simulator.getGeneratedCount() == 0)
    {
      return 0.0;
    }

    return static_cast< double >(simulator.getRejectedCount()) / simulator.getGeneratedCount();
  }

  size_t calculateNextRequestCount(double probability)
  {
    if (probability <= 0.0)
    {
      return 0;
    }

    const double result = confidenceCoefficient * confidenceCoefficient * (1.0 - probability)
        / (probability * relativeAccuracy * relativeAccuracy);

    if (!std::isfinite(result) || result >= static_cast< double >(maxRequestCount))
    {
      return maxRequestCount;
    }

    return static_cast< size_t >(std::ceil(result));
  }
}

void queuing_system::runAutomaticMode(size_t numSources, size_t numDevices, size_t bufferCapacity, double minArrival,
    double maxArrival, double meanServiceTime)
{
  size_t requestCount = initialRequestCount;

  double previousProbability = 0.0;
  bool hasPreviousProbability = false;

  std::cout << "\n===== AUTOMATIC SIMULATION =====\n";
  for (size_t i = 1; i <= maxIterations; ++i)
  {
    Simulator simulator(numSources, numDevices, bufferCapacity, minArrival, maxArrival, meanServiceTime);

    simulator.runForRequests(requestCount);
    const double currentProbability = getTotalRejectionProbability(simulator);

    std::cout << "\nIteration: " << i << '\n';
    std::cout << "Generated requests: " << simulator.getGeneratedCount() << '\n';
    std::cout << "Rejection probability: " << currentProbability << '\n';

    const bool converged = hasPreviousProbability
        && std::abs(currentProbability - previousProbability) < relativeAccuracy * previousProbability;

    if (converged)
    {
      std::cout << "\nRequired accuracy achieved.\n";
      printAutomaticSimulationSummary(simulator);
      return;
    }

    if (i == maxIterations || requestCount >= maxRequestCount)
    {
      std::cout << "\nMaximum number of iterations or requests reached.\n";
      std::cout << "Required accuracy was not achieved.\n";
      printAutomaticSimulationSummary(simulator);
      return;
    }

    size_t nextRequestCount = calculateNextRequestCount(currentProbability);
    if (nextRequestCount <= requestCount)
    {
      if (requestCount >= maxRequestCount)
      {
        std::cout << "\nMaximum request count reached.\n";
        printAutomaticSimulationSummary(simulator);
        return;
      }
      nextRequestCount = requestCount >= maxRequestCount / 2 ? maxRequestCount : requestCount * 2;
    }

    previousProbability = currentProbability;
    hasPreviousProbability = true;
    requestCount = nextRequestCount;
  }
}
