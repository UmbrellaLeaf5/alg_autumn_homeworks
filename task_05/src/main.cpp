#include <iostream>

#include "dec_tree.hpp"
#include "rmq.hpp"

int main() {
  // Пример использования RMQ
  std::vector<int> arr = {5, -1, 8, 1, 9, 4};
  RMQ<int> rmq(arr);

  std::cout << "RMQ(1, 4): " << rmq.Query(1, 4) << std::endl;
  std::cout << "RMQ(0, 2): " << rmq.Query(0, 2) << std::endl;
  std::cout << "RMQ(2, 5): " << rmq.Query(2, 5) << std::endl;
  std::cout << "RMQ(0, 5): " << rmq.Query(0, 5) << std::endl;

  std::cout << std::endl;

  // Пример использования RMQ с другими типами
  std::vector<double> arr_double = {5.5, 2.2, 8.8, 1.1, 9.9, 4.4};
  RMQ<double> rmq_double(arr_double);

  std::cout << "RMQ(1, 4): " << rmq_double.Query(1, 4) << std::endl;
  std::cout << "RMQ(0, 2): " << rmq_double.Query(0, 2) << std::endl;

  // std::vector<ssize_t> priorities = {17, 0, 36, 16, 23, 15, 42, 18, 20};
  // std::cout << ImplicitTreap<ssize_t, ssize_t>(priorities) << std::endl;

  return 0;
}