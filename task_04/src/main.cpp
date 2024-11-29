#include <iostream>

#include "dijkstra.hpp"

int main() {
  Graph<int, double> graph;

  graph.AddEdge(0, 1, 4);
  graph.AddEdge(0, 2, 1);
  graph.AddEdge(1, 3, 1);
  graph.AddEdge(2, 1, 2);
  graph.AddEdge(2, 3, 5);
  graph.AddEdge(3, 4, 3);

  auto distances = Dijkstra(graph, 0);

  std::cout << "Res with 0:" << std::endl;

  for (const auto& pair : distances)
    std::cout << "To " << pair.first << ": " << pair.second << std::endl;

  Graph<std::string, double> graph_str;

  graph_str.AddEdge("A", "B", 4.0);
  graph_str.AddEdge("A", "C", 2.0);
  graph_str.AddEdge("B", "C", 1.0);

  auto distances_str = Dijkstra(graph_str, std::string("A"));
  std::cout << std::endl << "Res with A:" << std::endl;

  for (const auto& pair : distances_str)
    std::cout << "To " << pair.first << ": " << pair.second << std::endl;

  return 0;
}