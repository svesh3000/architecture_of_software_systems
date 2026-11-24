#ifndef PRODUCER_HPP
#define PRODUCER_HPP
#include <cstddef>
#include <random>
#include "bid.hpp"

namespace queuing_system
{
  class Producer
  {
  public:
    Producer(size_t id, std::mt19937 & gen, double minTime, double maxTime, double startTime);

    Bid generateBid();

    double getNextArrivalTime() const noexcept;
    size_t getId() const noexcept;

  private:
    size_t id;
    size_t bidCounter = 0;

    std::mt19937 & gen;
    std::uniform_real_distribution< double > dist;

    double nextArrivalTime = 0.0;
  };
}

#endif
