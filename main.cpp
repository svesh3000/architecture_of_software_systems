#include <chrono>
#include <iostream>
#include <random>

int main()
{
  std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
  size_t n = 0, m = 0;
  size_t K = 0, KMIN = 0;
  double TAY = 0.0, TAY2 = 0.0;
  std::uniform_real_distribution< double > iz2_dist(TAY, TAY2);
  double mean_service = 0.0;
  double lambda = 1.0 / mean_service;
  std::exponential_distribution< double > pz1_dist(lambda);

  std::cout << "Input n (sources), m (devices), KMIN: ";
  std::cin >> n >> m >> KMIN;

  std::cout << "Input TAY2 (max interval): ";
  std::cin >> TAY2;

  std::cout << "Input mean service time: ";
  std::cin >> mean_service;

  while (!std::cin)
  {
    while (TAY < TAY2)
    {
      // генерация заявки
      // блок обработки результатов одной реализации
      double DTAY = iz2_dist(gen);
      TAY += DTAY;
    }
  }
  // вывод таблицы результатов
  return 0;
}
