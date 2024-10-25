#pragma once

#include <limits>

#include "topological_sort.hpp"

/**
 * @brief Вычисляет кратчайшие пути от заданной начальной вершины до всех других
 * вершин в направленном ациклическом графе (DAG) с помощью релаксации.
 * @tparam vert_t: тип вершины в графе
 * @tparam weight_t: тип веса в графе
 * @param graph: граф, для которого необходимо вычислить кратчайшие пути.
 * @param start_vert: начальная вершина, от которой вычисляются расстояния.
 * @throw std::invalid_argument("DAGRelaxation: there is no such start
 * vertice.")
 * @throw std::invalid_argument("DAGRelaxation: graph is not directed.");
 * @return std::unordered_map<vert_t, weight_t>: словарь, где ключ - вершина, а
 * значение - кратчайшее расстояние от start_vert до этой вершины
 * (если до вершины нет пути, то значение будет равно
 * std::numeric_limits<weight_t>::max())
 */
template <typename vert_t, typename weight_t>
std::unordered_map<vert_t, weight_t> DAGRelaxation(
    const Graph<vert_t, weight_t>& graph, vert_t start_vert) {
  if (!graph.ContainsVert(start_vert))
    throw std::invalid_argument(
        "DAGRelaxation: there is no such start vertice in graph.");

  if (!graph.IsDirected())
    throw std::invalid_argument("DAGRelaxation: graph is not directed.");

  /// @brief хеш-таблица расстояний от start_vert до каждой вершины
  std::unordered_map<vert_t, weight_t> dists;

  // инициализация расстояний от start_vert до каждой вершины бесконечностями
  for (const auto& vert : graph.Verts())
    dists[vert] = std::numeric_limits<weight_t>::max();

  // расстояние от начальной вершины до самой себя равно 0
  dists[start_vert] = 0;

  std::vector<vert_t> sorted_verts = TopologicalSort(graph);

  for (const auto& u_vert : sorted_verts)
    for (size_t i = 0; i < graph.GetAdjList()[u_vert].size(); i++) {
      auto vert = graph.GetAdjList()[u_vert][i];

      // (нас не интересует бесконечное расстояние от start до u_vert)
      if (dists[u_vert] == std::numeric_limits<weight_t>::max()) continue;

      // если граф не взвешен, то задаем расстояние единицей
      weight_t u_v_dist = 1;
      if (graph.IsWeighted()) u_v_dist = graph.GetWeightOfEdge({u_vert, vert});

      // релаксируем ребро, если текущее расстояние до vert больше, чем
      // расстояние до u_vert + расстояние между вершинами (вес их ребра)
      if (dists[vert] > dists[u_vert] + u_v_dist)
        dists[vert] = dists[u_vert] + u_v_dist;
    }

  return dists;
}