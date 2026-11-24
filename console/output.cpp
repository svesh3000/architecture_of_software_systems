#include "output.hpp"
#include <iomanip>
#include <iostream>

namespace
{
  constexpr int sourceColumnWidth = 9;
  constexpr int countColumnWidth = 11;
  constexpr int probabilityColumnWidth = 12;
  constexpr int timeColumnWidth = 12;
  constexpr int varianceColumnWidth = 12;

  void printTableSeparator()
  {
    std::cout << std::string(sourceColumnWidth + countColumnWidth * 3 + probabilityColumnWidth + timeColumnWidth * 3
            + varianceColumnWidth * 2 + 9,
        '-') << '\n';
  }
}

void queuing_system::printSimulationSummary(const Simulator & simulator)
{
  std::cout << "\n===== SIMULATION SUMMARY =====\n";
  std::cout << std::fixed << std::setprecision(5);

  std::cout << "Simulation time: " << simulator.getCurrentTime() << '\n';
  std::cout << "Generated requests: " << simulator.getGeneratedCount() << '\n';
  std::cout << "Served requests: " << simulator.getServedCount() << '\n';
  std::cout << "Rejected requests: " << simulator.getRejectedCount() << '\n';
  std::cout << "Requests in service: " << simulator.getInServiceCount() << '\n';

  const double rejectionProbability = simulator.getGeneratedCount() == 0
      ? 0.0
      : static_cast< double >(simulator.getRejectedCount()) / simulator.getGeneratedCount();

  std::cout << "Rejection probability: " << rejectionProbability << '\n';

  printSourceStatistics(simulator);
  printDeviceStatistics(simulator);
}

void queuing_system::printAutomaticSimulationSummary(const Simulator & simulator)
{
  std::cout << "\n===== SIMULATION SUMMARY =====\n";
  std::cout << "Simulation time:             " << simulator.getCurrentTime() << '\n';
  std::cout << "Generated requests:          " << simulator.getGeneratedCount() << '\n';
  std::cout << "Served requests:             " << simulator.getServedCount() << '\n';
  std::cout << "Rejected requests:           " << simulator.getRejectedCount() << '\n';
  std::cout << "Requests in service:         " << simulator.getInServiceCount() << '\n';

  const size_t generatedCount = simulator.getGeneratedCount();
  const size_t rejectedCount = simulator.getRejectedCount();

  const double rejectionProbability = generatedCount > 0 ? static_cast< double >(rejectedCount) / generatedCount : 0.0;

  size_t completedCount = 0;
  double totalWaitingTime = 0.0;
  double totalWaitingTimeSquared = 0.0;
  double totalServiceTime = 0.0;
  double totalServiceTimeSquared = 0.0;

  for (const auto & statistics : simulator.getSourceStatistics())
  {
    const size_t sourceCompletedCount = statistics.getCompletedCount();
    const double averageWaitingTime = statistics.getAverageWaitingTime();
    const double averageServiceTime = statistics.getAverageServiceTime();
    const double waitingTimeVariance = statistics.getWaitingTimeVariance();
    const double serviceTimeVariance = statistics.getServiceTimeVariance();

    completedCount += sourceCompletedCount;
    totalWaitingTime += averageWaitingTime * sourceCompletedCount;
    totalWaitingTimeSquared += (waitingTimeVariance + averageWaitingTime * averageWaitingTime) * sourceCompletedCount;
    totalServiceTime += averageServiceTime * sourceCompletedCount;
    totalServiceTimeSquared += (serviceTimeVariance + averageServiceTime * averageServiceTime) * sourceCompletedCount;
  }

  const double averageWaitingTime = completedCount > 0 ? totalWaitingTime / completedCount : 0.0;
  const double averageServiceTime = completedCount > 0 ? totalServiceTime / completedCount : 0.0;
  const double averageTimeInSystem = averageWaitingTime + averageServiceTime;
  const double waitingTimeVariance = completedCount > 0
      ? std::max(0.0, totalWaitingTimeSquared / completedCount - averageWaitingTime * averageWaitingTime)
      : 0.0;

  const double serviceTimeVariance = completedCount > 0
      ? std::max(0.0, totalServiceTimeSquared / completedCount - averageServiceTime * averageServiceTime)
      : 0.0;

  std::cout << "\n===== AGGREGATE STATISTICS =====\n";
  std::cout << "Rejection probability:       " << rejectionProbability << '\n';
  std::cout << "Average waiting time:        " << averageWaitingTime << '\n';
  std::cout << "Average service time:        " << averageServiceTime << '\n';
  std::cout << "Average time in system:      " << averageTimeInSystem << '\n';
  std::cout << "Waiting time variance:       " << waitingTimeVariance << '\n';
  std::cout << "Service time variance:       " << serviceTimeVariance << '\n';

  const auto & devices = simulator.getDevices();
  if (!devices.empty())
  {
    double totalUtilization = 0.0;
    double minUtilization = simulator.getDeviceUtilization(0);
    double maxUtilization = minUtilization;

    for (size_t i = 0; i < devices.size(); ++i)
    {
      const double utilization = simulator.getDeviceUtilization(i);

      totalUtilization += utilization;
      minUtilization = std::min(minUtilization, utilization);
      maxUtilization = std::max(maxUtilization, utilization);
    }

    const double averageUtilization = totalUtilization / devices.size();

    std::cout << "\n===== DEVICE STATISTICS =====\n";
    std::cout << "Average device utilization:  " << averageUtilization << '\n';
    std::cout << "Minimum device utilization:  " << minUtilization << '\n';
    std::cout << "Maximum device utilization:  " << maxUtilization << '\n';
  }
}

void queuing_system::printSourceStatistics(const Simulator & simulator)
{
  const auto & statistics = simulator.getSourceStatistics();

  std::cout << "\n===== SOURCE STATISTICS =====\n";
  std::cout << std::fixed << std::setprecision(5);

  std::cout << std::left << std::setw(sourceColumnWidth) << "Source" << std::right << std::setw(countColumnWidth)
            << "Generated" << std::setw(countColumnWidth) << "Rejected" << std::setw(countColumnWidth) << "Completed"
            << std::setw(probabilityColumnWidth) << "P(reject)" << std::setw(timeColumnWidth) << "Avg wait"
            << std::setw(timeColumnWidth) << "Avg service" << std::setw(timeColumnWidth) << "Avg system"
            << std::setw(varianceColumnWidth) << "Var wait" << std::setw(varianceColumnWidth) << "Var service" << '\n';

  printTableSeparator();

  for (size_t i = 0; i < statistics.size(); ++i)
  {
    const auto & source = statistics[i];

    std::cout << std::left << std::setw(sourceColumnWidth) << i + 1 << std::right << std::setw(countColumnWidth)
              << source.getGeneratedCount() << std::setw(countColumnWidth) << source.getRejectedCount()
              << std::setw(countColumnWidth) << source.getCompletedCount() << std::setw(probabilityColumnWidth)
              << source.getRejectionProbability() << std::setw(timeColumnWidth) << source.getAverageWaitingTime()
              << std::setw(timeColumnWidth) << source.getAverageServiceTime() << std::setw(timeColumnWidth)
              << source.getAverageTimeInSystem() << std::setw(varianceColumnWidth) << source.getWaitingTimeVariance()
              << std::setw(varianceColumnWidth) << source.getServiceTimeVariance() << '\n';
  }

  printTableSeparator();
}

void queuing_system::printDeviceStatistics(const Simulator & simulator)
{
  const auto & devices = simulator.getDevices();

  std::cout << "\n===== DEVICE STATISTICS =====\n";
  std::cout << std::left << std::setw(12) << "Device" << std::right << std::setw(15) << "Utilization" << '\n';

  std::cout << std::string(27, '-') << '\n';

  for (size_t i = 0; i < devices.size(); ++i)
  {
    std::cout << std::left << std::setw(12) << devices[i].getId() << std::right << std::fixed << std::setprecision(5)
              << std::setw(15) << simulator.getDeviceUtilization(i) << '\n';
  }

  std::cout << std::string(27, '-') << '\n';
}

void queuing_system::printSimulationState(const Simulator & simulator)
{
  std::cout << "\n===== CURRENT SIMULATION STATE =====\n";
  std::cout << std::fixed << std::setprecision(5);

  std::cout << "Current time: " << simulator.getCurrentTime() << '\n';
  std::cout << "Generated: " << simulator.getGeneratedCount() << '\n';
  std::cout << "Served: " << simulator.getServedCount() << '\n';
  std::cout << "Rejected: " << simulator.getRejectedCount() << '\n';

  std::cout << "\nDEVICES:\n";

  for (const auto & device : simulator.getDevices())
  {
    std::cout << "Device " << device.getId() << ": ";

    if (device.isBusy())
    {
      const auto & bid = device.getCurrentBid();

      std::cout << "Busy"
                << ", request " << bid.id << ", source " << bid.sourceId << ", release time "
                << device.getReleaseTime();
    }
    else
    {
      std::cout << "Free";
    }

    std::cout << '\n';
  }

  const auto & buffer = simulator.getBuffer();

  std::cout << "\nBUFFER:\n";
  std::cout << "Size: " << buffer.size() << '\n';

  for (const auto & bid : buffer.getQueue())
  {
    std::cout << "Request " << bid.id << ", source " << bid.sourceId << ", arrival time " << bid.arrivalTime << '\n';
  }
}
