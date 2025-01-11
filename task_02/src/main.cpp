#include <iostream>

#include "find_bridges.hpp"
#include "find_cut_vertices.hpp"

int main() {
  // Пример использования:
  // 1. Задаем граф
  Graph<int, int> network_graph;

  // Добавляем вершины
  network_graph.AddVert(0);
  network_graph.AddVert(1);
  network_graph.AddVert(2);
  network_graph.AddVert(3);
  network_graph.AddVert(4);
  network_graph.AddVert(5);

  // Добавляем ребра
  network_graph.AddEdge({0, 1});
  network_graph.AddEdge({1, 2});
  network_graph.AddEdge({2, 0});
  network_graph.AddEdge({1, 3});
  network_graph.AddEdge({3, 4});
  network_graph.AddEdge({4, 5});
  network_graph.AddEdge({3, 5});

  // Пример графа с 2 компонентами связности
  //    network_graph.AddVert(6);
  //    network_graph.AddVert(7);
  //    network_graph.AddEdge({6,7});

  // 2. Находим мосты
  std::vector<std::pair<int, int>> bridges = FindBridges(network_graph);

  // 3. Находим точки сочленения
  std::vector<int> cut_vertices = FindCutVertices(network_graph);

  // 4. Выводим результаты
  std::cout << "Bridges (wires that, if damaged, will disconnect the network):"
            << std::endl;

  if (bridges.empty())
    std::cout << "No bridges were found in the network." << std::endl;
  else
    for (const auto& bridge : bridges)
      std::cout << "(" << bridge.first << ", " << bridge.second << ")"
                << std::endl;

  std::cout << "\nCut Vertices (routers that, if out of order, will disconnect "
               "the network):"
            << std::endl;
  if (cut_vertices.empty())
    std::cout << "No cut vertices were found in the network." << std::endl;
  else
    for (const auto& cut_vertice : cut_vertices)
      std::cout << cut_vertice << std::endl;

  return 0;
}
