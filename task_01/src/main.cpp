#include <iostream>

#include "topological_sort.hpp"

int main() {
  // ------------------------ Graph with size_t -------------------------

  std::cout << std::endl
            << "------------------------------"
               "      Graph with size_t       "
               "------------------------------"
            << std::endl
            << std::endl;

  auto graph = Graph<size_t, long>::GraphWeighted({{0, 1, 1},
                                                   {0, 2, 2},
                                                   {1, 3, 3},
                                                   {1, 4, 4},
                                                   {2, 5, 5},
                                                   {3, 5, 6},
                                                   {4, 5, 7}});

  std::cout << graph << std::endl << std::endl;
  graph.PrintAdjList();

  std::cout << std::endl << "ANSWER: ";

  auto sorted_verts = TopologicalSort(graph);
  std::cout << sorted_verts << std::endl << std::endl;

  // ------------------------ Graph with string -------------------------

  std::cout << std::endl
            << "------------------------------"
               "      Graph with string       "
               "------------------------------"
            << std::endl
            << std::endl;

  auto graph_strs = Graph<std::string, long>::GraphWeighted({{"A", "B", 1},
                                                             {"A", "C", 2},
                                                             {"B", "D", 3},
                                                             {"B", "E", 4},
                                                             {"C", "F", 5},
                                                             {"D", "F", 6},
                                                             {"E", "F", 7}});

  std::cout << graph_strs << std::endl << std::endl;
  graph_strs.PrintAdjList();

  std::cout << std::endl << "ANSWER: ";

  auto sorted_verts_strs = TopologicalSort(graph_strs);
  std::cout << sorted_verts_strs << std::endl << std::endl;

  return 0;
}
