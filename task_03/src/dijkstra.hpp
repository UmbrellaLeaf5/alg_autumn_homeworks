#pragma once

#include "graph.hpp"

/**
 * @brief Вычисляет кратчайшие пути от заданной начальной вершины до всех других
 * вершин в графе (вес в котором может быть только положительным, если есть) с
 * помощью алгоритма Дейкстры.
 * @tparam vert_t: тип вершины в графе.
 * @tparam weight_t: тип веса в графе.
 * @param graph: граф, для которого необходимо вычислить кратчайшие пути.
 * @param start: начальная вершина, от которой вычисляются расстояния.
 * @throw std::invalid_argument("Dijkstra: there is no such start vertice in
 * graph.").
 * @throw `throw std::logic_error("Dijkstra: graph contains negative weighted
 * edges.")`.
 * @return `std::unordered_map<vert_t, weight_t>`: словарь, где ключ - вершина,
 * а значение - кратчайшее расстояние от start до этой вершины (если до вершины
 * нет пути, то значение будет равно `std::numeric_limits<weight_t>::max()`).
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline std::unordered_map<vert_t, weight_t> Dijkstra(
    const Graph<vert_t, weight_t>& graph, const vert_t& start) {
  if (!graph.ContainsVert(start))
    throw std::invalid_argument(
        "Dijkstra: there is no such start vertice in graph.");

  if (graph.Verts().empty()) return {};

  std::unordered_map<vert_t, weight_t> distances;
  std::unordered_map<vert_t, bool> visited;

  // инициализация расстояний до бесконечности
  for (const auto& vert : graph.Verts()) {
    distances[vert] = std::numeric_limits<weight_t>::max();
    visited[vert] = false;
  }

  distances[start] = 0;

  for (size_t i = 0; i < graph.VertsAmount(); i++) {
    vert_t min_vert =
        graph.Verts()[0];  // находим вершину с минимальным расстоянием
    weight_t min_distance = std::numeric_limits<weight_t>::max();

    for (const auto& node : graph.Verts())
      if (!visited[node] && distances[node] < min_distance) {
        min_vert = node;
        min_distance = distances[node];
      }

    if (min_distance == std::numeric_limits<weight_t>::max())
      break;  // все достижимые вершины обработаны

    visited[min_vert] = true;

    // релаксация ребер
    const auto neighbors = graph.GetAdjList()[min_vert];
    for (const auto& neighbor : neighbors) {
      weight_t weight =
          graph.IsWeighted() ? graph.GetEdgeWeight({min_vert, neighbor}) : 1;

      if (weight < 0)
        throw std::logic_error(
            "Dijkstra: graph contains negative weighted edges.");

      if (distances[min_vert] + weight < distances[neighbor])
        distances[neighbor] = distances[min_vert] + weight;
    }
  }

  return distances;
}
