#include <iostream>

#include "find_bridges.hpp"

int main() {
  Graph<int, long> g;
  g.AddEdge(0, 1);
  g.AddEdge(0, 2);
  g.AddEdge(1, 2);
  g.AddEdge(3, 2);

  g.MakeUndirected();

  std::cout << g << std::endl;
  std::cout << "Bridges: " << std::endl;
  std::cout << FindBridges(g) << std::endl;

  return 0;
}
