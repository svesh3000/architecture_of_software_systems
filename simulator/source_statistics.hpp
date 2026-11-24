#ifndef SOURCE_STATISTICS_HPP
#define SOURCE_STATISTICS_HPP
#include <cstddef>

namespace queuing_system
{
  class SourceStatistics
  {
  public:
    void recordGeneratedBid() noexcept;
    void recordRejectedBid() noexcept;
    void recordCompletedBid(double waitingTime, double serviceTime) noexcept;

    size_t getGeneratedCount() const noexcept;
    size_t getRejectedCount() const noexcept;
    size_t getCompletedCount() const noexcept;

    double getRejectionProbability() const noexcept;
    double getAverageWaitingTime() const noexcept;
    double getAverageServiceTime() const noexcept;
    double getAverageTimeInSystem() const noexcept;
    double getWaitingTimeVariance() const noexcept;
    double getServiceTimeVariance() const noexcept;

  private:
    size_t generatedCount = 0;
    size_t rejectedCount = 0;
    size_t completedCount = 0;

    double totalWaitingTime = 0.0;
    double totalWaitingTimeSquared = 0.0;

    double totalServiceTime = 0.0;
    double totalServiceTimeSquared = 0.0;
  };
}

#endif
