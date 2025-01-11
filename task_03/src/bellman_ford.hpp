#pragma once

#include "graph.hpp"

/**
 * @brief Вычисляет кратчайшие пути от заданной начальной вершины до всех других
 * вершин в графе (в котором не должно быть отрицательных весовых циклов) с
 * помощью алгоритма Беллмана-Форда.
 * @tparam vert_t: тип вершины в графе.
 * @tparam weight_t: тип веса в графе.
 * @param graph: граф, для которого необходимо вычислить кратчайшие пути.
 * @param start: начальная вершина, от которой вычисляются расстояния.
 * @throw std::invalid_argument("BellmanFord: there is no such start vertice in
 * graph.").
 * @throw `throw std::runtime_error("BellmanFord: graph has negative cycle.")`.
 * @return `std::unordered_map<vert_t, weight_t>`: словарь, где ключ - вершина,
 * а значение - кратчайшее расстояние от start до этой вершины (если до вершины
 * нет пути, то значение будет равно `std::numeric_limits<weight_t>::max()`).
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline std::unordered_map<vert_t, weight_t> BellmanFord(
    const Graph<vert_t, weight_t>& graph, const vert_t& start) {
  if (!graph.ContainsVert(start))
    throw std::invalid_argument(
        "BellmanFord: there is no such start vertice in graph.");

  if (graph.Verts().empty()) return {};

  std::unordered_map<vert_t, weight_t> distances;

  // инициализация расстояний до бесконечности
  for (const auto& vert : graph.Verts())
    distances[vert] = std::numeric_limits<weight_t>::max();

  distances[start] = 0;

  // if d[v] > d[u] + ω(u,v)
  auto Condition = [&distances](vert_t u, vert_t v, weight_t weight) {
    return distances[u] != std::numeric_limits<weight_t>::max() &&
           distances[u] + weight < distances[v];
  };

  // релаксация ребер |V| - 1 раз
  for (size_t i = 0; i < graph.VertsAmount() - 1; i++)
    for (const auto& edge : graph.Edges()) {
      auto u = StartVertFromTuple(edge);
      auto v = EndVertFromTuple(edge);
      weight_t weight = graph.IsWeighted() ? WeightFromTuple(edge) : 1;

      if (Condition(u, v, weight)) distances[v] = distances[u] + weight;

      if (!graph.IsDirected() && Condition(v, u, weight))
        distances[u] = distances[v] + weight;
    }

  // проверка отрицательных циклов
  for (const auto& edge : graph.Edges()) {
    auto u = StartVertFromTuple(edge);
    auto v = EndVertFromTuple(edge);
    weight_t weight = graph.IsWeighted() ? WeightFromTuple(edge) : 1;

    if (Condition(u, v, weight))
      throw std::runtime_error("BellmanFord: graph has negative cycle.");

    if (!graph.IsDirected() && Condition(v, u, weight))
      throw std::runtime_error("BellmanFord: graph has negative cycle.");
  }

  return distances;
}