#ifndef BID_HPP
#define BID_HPP
#include <cstddef>

namespace queuing_system
{
  struct Bid
  {
    size_t id = 0;
    size_t sourceId = 0;
    double arrivalTime = 0.0;
  };
}
#endif
