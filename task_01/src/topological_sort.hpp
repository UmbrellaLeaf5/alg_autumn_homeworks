#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>

#include "graph/graph.hpp"

template <typename vert_t, typename weight_t>
static void TopologicalSortStepDFS(const Graph<vert_t, weight_t>& graph,
                                   vert_t u_vert,
                                   std::unordered_map<vert_t, bool>& visited,
                                   std::vector<vert_t>& ans) {
  visited[u_vert] = true;

  for (size_t i = 0; i < graph.GetAdjList()[u_vert].size(); i++) {
    auto vert = graph.GetAdjList()[u_vert][i];
    if (!visited[vert]) TopologicalSortStepDFS(graph, vert, visited, ans);
  }

  ans.push_back(u_vert);
}

/**
 * @brief Производит топологическую сортировку вершин на основе обхода в глубину
 * (DFS)
 * @tparam vert_t: тип вершины в графе
 * @tparam weight_t: тип веса в графе
 * @param graph: сортируемый граф
 * @return std::vector<vert_t>: список отсортированных вершин
 */
template <typename vert_t, typename weight_t>
std::vector<vert_t> TopologicalSort(const Graph<vert_t, weight_t>& graph) {
  std::unordered_map<vert_t, bool> visited;

  for (const auto& vert : graph.Verts()) visited[vert] = false;

  std::vector<vert_t> ans;

  for (const auto& vert : graph.Verts())
    if (!visited[vert]) TopologicalSortStepDFS(graph, vert, visited, ans);

  std::reverse(ans.begin(), ans.end());

  return ans;
}
