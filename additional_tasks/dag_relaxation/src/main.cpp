#include "dag_relaxation.hpp"

int main() {
  Graph<std::string, long> graph;
  // graph.AddEdge("A", "B", 5);
  // graph.AddEdge("A", "C", 3);
  // graph.AddEdge("B", "D", 2);
  // graph.AddEdge("C", "D", 4);
  // graph.AddEdge("C", "E", 1);
  // graph.AddEdge("D", "F", 7);
  // graph.AddEdge("E", "F", 2);

  graph.AddEdge("A", "B");
  graph.AddEdge("A", "C");
  graph.AddEdge("B", "D");
  graph.AddEdge("C", "D");
  graph.AddEdge("C", "E");
  graph.AddEdge("D", "F");
  graph.AddEdge("E", "F");
  graph.MakeDirected();

  std::string start = "A";

  auto distances = DAGRelaxation(graph, start);

  std::cout << "кратчайшие пути от вершины '" << start << "':" << std::endl;
  for (const auto& vert : distances) {
    std::cout << "до вершины '" << vert.first << "': "
              << (vert.second == std::numeric_limits<long>::max()
                      ? "inf"
                      : std::to_string(vert.second))
              << std::endl;
  }

  return 0;
}
