#pragma once

#include "graph.hpp"

namespace {

template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline void TopologicalSortStep(vert_t u_vert,
                                std::unordered_map<vert_t, bool>& visited,
                                Graph<vert_t, weight_t>& graph,
                                std::vector<vert_t>& topological_order) {
  if (visited[u_vert])
    throw std::invalid_argument("TopologicalSort: graph contains cycle.");

  visited[u_vert] = true;

  const auto neighbors = graph.GetAdjList()[u_vert];
  for (const auto& vert : neighbors)
    if (graph.ContainsVert(u_vert))
      TopologicalSortStep(vert, visited, graph, topological_order);

  topological_order.push_back(u_vert);
  graph.RemoveVert(u_vert);
}

}  // namespace

/**
 * @brief Производит топологическую сортировку вершин на основе обхода в глубину
 * (DFS).
 *
 * @tparam vert_t: тип вершины в графе.
 * @tparam weight_t: тип веса в графе.
 * @param graph: сортируемый граф.
 *
 * @throw std::invalid_argument("TopologicalSort: graph is not directed.").
 * @throw std::invalid_argument("TopologicalSort: graph contains cycle.").
 *
 * @return `std::vector<vert_t>`: список отсортированных вершин.
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline std::vector<vert_t> TopologicalSort(Graph<vert_t, weight_t> graph) {
  if (!graph.IsDirected())
    throw std::invalid_argument("TopologicalSort: graph is not directed.");

  if (graph.Verts().empty()) return {};

  std::vector<vert_t> topological_order;

  std::unordered_map<vert_t, bool> visited;
  for (const auto& vert : graph.Verts()) visited[vert] = false;

  while (graph.VertsAmount())
    TopologicalSortStep(graph.Verts()[0], visited, graph, topological_order);

  std::reverse(topological_order.begin(), topological_order.end());

  return topological_order;
}
