#include "tarjan_algorithm.hpp"

void Solution(std::istream& is = std::cin, std::ostream& os = std::cout) {
  size_t experiments_amount;
  is >> experiments_amount;

  Graph<std::string, long> graph;
  graph.MakeDirected();

  for (size_t i = 0; i < experiments_amount; i++) {
    std::string u, v;
    is >> u >> v;

    graph.AddEdge(u, v);
    graph.AddVert(u);
    graph.AddVert(v);
  }

  for (const auto& component : StronglyConnectedComponents(graph))
    if (component.size() != 1 && !component.empty())
      for (const auto& elem : component) os << elem << " ";

  os << std::endl;
}