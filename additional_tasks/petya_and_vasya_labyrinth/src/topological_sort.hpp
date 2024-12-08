#pragma once

#include "graph.hpp"

namespace {

template <AllowedVertType vert_t>
void TopologicalSortStep(
    const vert_t& u_vert, std::unordered_map<vert_t, bool>& visited,
    std::unordered_map<vert_t, std::vector<vert_t>>& adj_list,
    std::vector<vert_t>& topological_order) {
  visited[u_vert] = true;

  for (const auto& vert : adj_list[u_vert])
    if (!visited[vert])
      TopologicalSortStep(vert, visited, adj_list, topological_order);

  topological_order.push_back(u_vert);
}

}  // namespace

/**
 * @brief Производит топологическую сортировку вершин на основе обхода в глубину
 * (DFS)
 * @tparam vert_t: тип вершины в графе
 * @tparam weight_t: тип веса в графе
 * @param graph: сортируемый граф
 * @throw `std::invalid_argument("TopologicalSort: graph is not directed.")`.
 * @return `std::vector<vert_t>`: список отсортированных вершин
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
std::vector<vert_t> TopologicalSort(const Graph<vert_t, weight_t>& graph) {
  if (!graph.IsDirected())
    throw std::invalid_argument("TopologicalSort: graph is not directed.");
  if (graph.Verts().empty()) return {};

  std::vector<vert_t> topological_order;

  std::unordered_map<vert_t, bool> visited;
  for (const auto& vert : graph.Verts()) visited[vert] = false;

  std::unordered_map<vert_t, std::vector<vert_t>> adj_list = graph.GetAdjList();

  for (const auto& vert : graph.Verts())
    if (!visited[vert])
      TopologicalSortStep(vert, visited, adj_list, topological_order);

  std::reverse(topological_order.begin(), topological_order.end());

  return topological_order;
}
