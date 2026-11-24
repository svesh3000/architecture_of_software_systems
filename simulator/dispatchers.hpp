#ifndef DISPATCHERS_HPP
#define DISPATCHERS_HPP
#include <cstddef>
#include <random>
#include <vector>
#include "buffer.hpp"
#include "device.hpp"

namespace queuing_system
{
  Device * selectDevice(std::vector< Device > & devices, size_t & lastIndex);

  bool handleArrival(const Bid & newBid, std::vector< Device > & devices, Buffer & buffer, size_t & lastIndex,
      double currentTime, std::mt19937 & gen, std::exponential_distribution< double > & pz1_dist, Bid & rejectedBid);

  void handleDeparture(Device & device, Buffer & buffer, double currentTime, std::mt19937 & gen,
      std::exponential_distribution< double > & pz1_dist);
}
#endif
