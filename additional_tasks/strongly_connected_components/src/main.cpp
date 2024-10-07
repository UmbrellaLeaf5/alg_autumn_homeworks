#include <tarjan_algorithm.hpp>

int main() {
  std::unordered_map<std::string, std::vector<std::string>> adj_list_dict = {
      // clang-format off
      {"A", {"B"}},
      {"B", {"C", "D"}},
      {"C", {"A", "D"}},
      {"D", {"E"}},
      {"E", {"D"}}, 
      {"F", {"E", "G"}}, 
      {"G", {"F", "H"}}, 
      {"H", {"E", "G"}}
      // clang-format on
  };

  auto graph = Graph<std::string, long>::GraphFromAdjList(adj_list_dict);

  std::cout << StronglyConnectedComponents(graph) << std::endl;
  return 0;
}
