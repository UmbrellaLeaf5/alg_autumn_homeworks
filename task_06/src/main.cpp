#include "lca.hpp"

int main() {
  // Пример использования с графом из строк
  Graph<std::string, int> graph = Graph<std::string, int>::GraphFromStrs(
      {"a->b", "a->c", "b->d", "b->e", "c->f", "c->g"});
  LCA<std::string, int> lca_str(graph, "a");
  std::cout << "LCA(d, e): " << lca_str.Ancestor("d", "e")
            << std::endl;  // LCA(d, e): b
  std::cout << "LCA(d, f): " << lca_str.Ancestor("d", "f")
            << std::endl;  // LCA(d, f): a
  std::cout << "LCA(g, e): " << lca_str.Ancestor("g", "e")
            << std::endl;  // LCA(g, e): a

  std::cout << std::endl;

  // Пример использования с графом из чисел
  Graph<int, int> graph_int =
      Graph<int, int>::GraphFromAdjList({{1, 2}, {0, 3, 4}, {0, 5, 6}});
  LCA<int, int> lca_int(graph_int, 0);
  std::cout << "LCA(3, 4): " << lca_int.Ancestor(3, 4)
            << std::endl;  // LCA(3, 4): 1
  std::cout << "LCA(3, 6): " << lca_int.Ancestor(3, 6)
            << std::endl;  // LCA(3, 6): 0
  std::cout << "LCA(5, 6): " << lca_int.Ancestor(5, 6)
            << std::endl;  // LCA(5, 6): 2

  std::cout << "LCA(5, 6): " << CalculateLCA(graph_int, 0, 5, 6)
            << std::endl;  // LCA(5, 6): 2

  std::cout << std::endl;

  Graph<int, int> undirected_graph =
      Graph<int, int>::GraphFromAdjList({{1, 2}, {0, 3, 4}, {0, 5, 6}});
  undirected_graph.MakeUndirected();
  LCA<int, int> lca_undirected_graph(undirected_graph, 0);
  std::cout << "LCA(3, 4): " << lca_undirected_graph.Ancestor(3, 4)
            << std::endl;  // LCA(3, 4): 1
  std::cout << "LCA(3, 6): " << lca_undirected_graph.Ancestor(3, 6)
            << std::endl;  // LCA(3, 6): 0
  std::cout << "LCA(5, 6): " << lca_undirected_graph.Ancestor(5, 6)
            << std::endl;  // LCA(5, 6): 2

  return 0;
}
