#include <iostream>

#include "graph.hpp"

template <typename V, typename W>
void PrintGraph(const Graph<V, W>& graph) {
  std::cout << std::endl << graph << std::endl << std::endl;
  graph.PrintAdjList();
  std::cout << std::endl << graph.GetAdjList() << std::endl;
}

int main() {
  // ------------------------- Graph from pairs -------------------------

  std::cout << std::endl
            << "------------------------------"
               "       Graph from pairs       "
               "------------------------------"
            << std::endl
            << std::endl;

  const std::vector<std::pair<size_t, size_t>> edges_pairs = {
      {0, 1}, {1, 2}, {2, 0}};

  auto graph = Graph<size_t, long>::GraphNonWeighted(edges_pairs);

  std::cout << "Pairs: " << edges_pairs << std::endl;

  PrintGraph(graph);

  // ------------------- Graph from pairs and weights -------------------

  std::cout << std::endl
            << "------------------------------"
               " Graph from pairs and weights "
               "------------------------------"
            << std::endl
            << std::endl;

  const std::vector<long> weights = {1, 2, 3};

  graph = Graph<size_t, long>::GraphWeighted(edges_pairs, weights);

  std::cout << "Pairs: " << edges_pairs << std::endl
            << "Weights: " << weights << std::endl;

  PrintGraph(graph);

  // ------------------------- Graph from tuples ------------------------

  std::cout << std::endl
            << "------------------------------"
               "      Graph from tuples       "
               "------------------------------"
            << std::endl
            << std::endl;

  const std::vector<std::tuple<size_t, size_t, long>> edges_tuples = {
      {0, 1, 5}, {1, 2, 10}, {2, 0, 3}};

  graph = Graph<size_t, long>::GraphWeighted(edges_tuples);

  std::cout << "Tuples: " << edges_tuples << std::endl;

  PrintGraph(graph);

  // ------------------------ Graph from strings ------------------------

  std::cout << std::endl
            << "------------------------------"
               "      Graph from strings      "
               "------------------------------"
            << std::endl
            << std::endl;

  const std::vector<std::string> edges_strs = {"0->1", "2->1", "3->2", "1->3"};

  graph = Graph<size_t, long>::GraphFromStrs(edges_strs);

  std::cout << "Strings: " << edges_strs << std::endl;

  PrintGraph(graph);

  // -------------------------- Graph from dict -------------------------

  std::cout << std::endl
            << "------------------------------"
               "       Graph from dict        "
               "------------------------------"
            << std::endl
            << std::endl;

  const std::unordered_map<std::string, long> edges_dict = {
      {"0->1", 5}, {"2->1", 1}, {"3->2", 2}, {"1->3", 3}};

  graph = Graph<size_t, long>::GraphFromMap(edges_dict);

  std::cout << "Dicts: " << edges_dict << std::endl;

  PrintGraph(graph);

  // ----------------------- Graph from AdjMatrix -----------------------

  std::cout << std::endl
            << "------------------------------"
               "     Graph from AdjMatrix     "
               "------------------------------"
            << std::endl
            << std::endl;

  std::vector<std::vector<long>> adj_matrix = {{0, 1, 0}, {1, 0, 1}, {0, 1, 0}};

  graph = Graph<size_t, long>::GraphFromAdjMatrix(adj_matrix);

  std::cout << "AdjMatrix: " << adj_matrix << std::endl;

  PrintGraph(graph);

  std::cout << std::endl << "WEIGHTED VERSION:" << std::endl << std::endl;

  adj_matrix = {{0, 1, 0}, {1, 0, 5}, {0, 7, 0}};
  graph = Graph<size_t, long>::GraphFromAdjMatrix(adj_matrix, true);

  std::cout << "AdjMatrix: " << adj_matrix << std::endl;

  PrintGraph(graph);

  // ------------------------ Graph from AdjList ------------------------

  std::cout << std::endl
            << "------------------------------"
               "      Graph from AdjList      "
               "------------------------------"
            << std::endl
            << std::endl;

  const std::vector<std::vector<size_t>> adj_list = {{1}, {0, 2}, {1}};

  graph = Graph<size_t, long>::GraphFromAdjList(adj_list);

  std::cout << "AdjList: " << adj_list << std::endl;

  PrintGraph(graph);

  // ------------------- Graph from AdjList with keys -------------------

  std::cout << std::endl
            << "------------------------------"
               " Graph from AdjList with keys "
               "------------------------------"
            << std::endl
            << std::endl;

  const std::unordered_map<size_t, std::vector<size_t>> adj_list_dict = {
      {0, {1}}, {1, {0, 2}}, {2, {1}}};

  graph = Graph<size_t, long>::GraphFromAdjList(adj_list_dict);

  std::cout << "AdjList: " << adj_list_dict << std::endl;

  PrintGraph(graph);

  std::cout << std::endl << "STRING VERSION:" << std::endl << std::endl;

  std::unordered_map<std::string, std::vector<std::string>> adj_list_dict_2 = {
      {"A", {"B"}}, {"B", {"A", "C"}}, {"C", {"B"}}};

  auto graph_2 = Graph<std::string, long>::GraphFromAdjList(adj_list_dict_2);

  std::cout << "AdjList: " << adj_list_dict_2 << std::endl;

  PrintGraph(graph_2);

  return 0;
}
