#pragma once

#include "bellman_ford.hpp"
#include "dijkstra.hpp"

/**
 * @brief Вычисляет кратчайшие расстояния между всеми парами вершин в графе с
 * использованием алгоритма Джонсона.
 * @tparam vert_t: тип вершины в графе.
 * @tparam weight_t: тип веса в графе.
 * @param graph: граф, для которого необходимо вычислить кратчайшие пути.
 * @throw `throw std::runtime_error("Johnson: graph has negative cycle.")`.
 * @return `std::unordered_map<vert_t, std::unordered_map<vert_t, weight_t>>`:
 *  `std::unordered_map`, где:
 *      - внешний ключ - это начальная вершина.
 *      - значение - это вложенный `std::unordered_map`, где:
 *          - внутренний ключ - это конечная вершина.
 *          - значение - это кратчайшее расстояние между начальной и конечной
 *            вершинами. Если пути нет, то значение будет равно
 * `std::numeric_limits<weight_t>::max()`.
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline std::unordered_map<vert_t, std::unordered_map<vert_t, weight_t>> Johnson(
    const Graph<vert_t, weight_t>& graph) {
  // 1. Создание расширенного графа G'
  vert_t source_vert;

  if constexpr (std::is_same_v<vert_t, std::string>)
    source_vert = "___source_vert___";
  else {
    source_vert = -1;
    for (const auto& vert : graph.Verts())
      if (vert <= source_vert) source_vert = vert - 1;
  }

  Graph<vert_t, weight_t> graph_prime = graph;
  graph_prime.AddVert(source_vert);

  // добавляем ребра с весом 0 от source_vert ко всем остальным вершинам
  for (const auto& vert : graph.Verts())
    graph_prime.AddEdge({source_vert, vert, static_cast<weight_t>(0)},
                        /* ignore warning = */ true);

  // 2.  Запуск алгоритма Беллмана-Форда для обнаружения отрицательных циклов и
  // поиска потенциалов φ(v)
  std::unordered_map<vert_t, weight_t> potential;
  try {
    potential = BellmanFord(graph_prime, source_vert);
  } catch (const std::runtime_error& e) {
    throw std::runtime_error("Johnson: " +
                             ReplacedString(e.what(), "BellmanFord: "));
  }

  // 3. Перевзвешивание ребер с использованием φ(v)
  Graph<vert_t, weight_t> reweighted_graph = graph;

  for (auto& edge : reweighted_graph.Edges()) {
    auto u = StartVertFromTuple(edge);
    auto v = EndVertFromTuple(edge);
    weight_t new_weight = WeightFromTuple(edge) + potential[u] - potential[v];

    reweighted_graph.SetEdgeWeight({u, v}, new_weight);
  }

  // 4. Запуск алгоритма Дейкстры для каждой вершины для получения
  // перевзвешенных расстояний
  std::unordered_map<vert_t, std::unordered_map<vert_t, weight_t>> answer;

  for (const auto& u : graph.Verts()) {
    std::unordered_map<vert_t, weight_t> dijkstra_dists =
        Dijkstra(reweighted_graph, u);
    answer.emplace(u, std::unordered_map<vert_t, weight_t>());
    // answer[u] = std::unordered_map<vert_t, weight_t>();

    // 5. Корректировка расстояний
    for (const auto& v : graph.Verts()) {
      answer[u][v] = std::numeric_limits<weight_t>::max();

      if (dijkstra_dists[v] != std::numeric_limits<weight_t>::max())
        answer[u][v] = dijkstra_dists[v] + potential[v] - potential[u];
    }
  }

  return answer;
}