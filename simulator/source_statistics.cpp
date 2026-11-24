#include "source_statistics.hpp"
#include <algorithm>

namespace
{
  double calculateVariance(double sum, double sumSquared, size_t count) noexcept
  {
    if (count == 0)
    {
      return 0.0;
    }

    const double average = sum / count;
    const double averageSquared = sumSquared / count;

    return std::max(0.0, averageSquared - average * average);
  }
}

void queuing_system::SourceStatistics::recordGeneratedBid() noexcept
{
  ++generatedCount;
}

void queuing_system::SourceStatistics::recordRejectedBid() noexcept
{
  ++rejectedCount;
}

void queuing_system::SourceStatistics::recordCompletedBid(double waitingTime, double serviceTime) noexcept
{
  ++completedCount;

  totalWaitingTime += waitingTime;
  totalWaitingTimeSquared += waitingTime * waitingTime;

  totalServiceTime += serviceTime;
  totalServiceTimeSquared += serviceTime * serviceTime;
}

size_t queuing_system::SourceStatistics::getGeneratedCount() const noexcept
{
  return generatedCount;
}

size_t queuing_system::SourceStatistics::getRejectedCount() const noexcept
{
  return rejectedCount;
}

size_t queuing_system::SourceStatistics::getCompletedCount() const noexcept
{
  return completedCount;
}

double queuing_system::SourceStatistics::getRejectionProbability() const noexcept
{
  if (generatedCount == 0)
  {
    return 0.0;
  }

  return static_cast< double >(rejectedCount) / generatedCount;
}

double queuing_system::SourceStatistics::getAverageWaitingTime() const noexcept
{
  if (completedCount == 0)
  {
    return 0.0;
  }

  return totalWaitingTime / completedCount;
}

double queuing_system::SourceStatistics::getAverageServiceTime() const noexcept
{
  if (completedCount == 0)
  {
    return 0.0;
  }

  return totalServiceTime / completedCount;
}

double queuing_system::SourceStatistics::getAverageTimeInSystem() const noexcept
{
  return getAverageWaitingTime() + getAverageServiceTime();
}

double queuing_system::SourceStatistics::getWaitingTimeVariance() const noexcept
{
  return calculateVariance(totalWaitingTime, totalWaitingTimeSquared, completedCount);
}

double queuing_system::SourceStatistics::getServiceTimeVariance() const noexcept
{
  return calculateVariance(totalServiceTime, totalServiceTimeSquared, completedCount);
}
