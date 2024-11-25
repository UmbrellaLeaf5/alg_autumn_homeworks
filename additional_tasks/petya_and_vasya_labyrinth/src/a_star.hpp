#pragma once

#include <functional>
#include <limits>
#include <queue>
#include <set>

#include "graph/graph.hpp"

/**
 * @brief Вычисляет кратчайший путь между двумя вершинами с помощью A*.
 * @tparam vert_t: тип вершины в графе
 * @tparam weight_t: тип веса в графе
 * @param start: начальная вершина.
 * @param goal: конечная вершина.
 * @param graph: граф, для которого необходимо вычислить кратчайшие пути.
 * @param heuristic_range: функция эвристической оценки расстояния от
 * произвольной вершины до конечной вершины. Должна принимать две вершины в
 * качестве аргументов и возвращать вес (оценку расстояния).
 * @throw `std::invalid_argument("AStar: there is no such start vertice in
 * graph.")`.
 * @throw `std::invalid_argument("AStar: there is no such goal vertice in
 * graph.")`.
 * @return `weight_t`: стоимость кратчайшего пути от `start` до `goal`
 * (если до вершины нет пути, то значение будет равно
 * `std::numeric_limits<weight_t>::max()`).
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
weight_t AStar(
    const vert_t& start, const vert_t& goal,
    const Graph<vert_t, weight_t>& graph,
    std::function<weight_t(const vert_t&, const vert_t&)> heuristic_range) {
  if (!graph.ContainsVert(start))
    throw std::invalid_argument(
        "AStar: there is no such start vertice in graph.");

  if (!graph.ContainsVert(goal))
    throw std::invalid_argument(
        "AStar: there is no such goal vertice in graph.");

  std::priority_queue<std::pair<weight_t, vert_t>,
                      std::vector<std::pair<weight_t, vert_t>>,
                      std::greater<std::pair<weight_t, vert_t>>>
      /**
       * @brief приоритетная очередь для вершин, которые нужно посетить
       * @details пары (стоимость, вершина), упорядоченные по стоимости.
       */
      visited_verts;

  // @brief стоимости пути от стартовой вершины до каждой вершины
  std::unordered_map<vert_t, weight_t> cost_from_start;

  // @brief суммы стоимостей пути от стартовой вершины и эвристики до конечной
  std::unordered_map<vert_t, weight_t> range_plus_cost;

  for (const auto& v : graph.Verts())
    cost_from_start[v] = std::numeric_limits<weight_t>::max();

  // начальные значения
  cost_from_start[start] = 0;
  range_plus_cost[start] =
      cost_from_start[start] + heuristic_range(start, goal);
  visited_verts.push({range_plus_cost[start], start});

  while (!visited_verts.empty()) {
    // посещенная вершина с минимальным значением range_plus_cost
    const vert_t current = visited_verts.top().second;
    visited_verts.pop();

    // достигнута конечная вершина, уже нашли путь до нужной вершины
    if (current == goal) return cost_from_start[goal];

    const auto neighbors = graph.GetAdjList()[current];
    for (const auto& neighbor : neighbors) {
      // @brief предполагаемая стоимость пути до соседа
      weight_t tentative_score =
          cost_from_start[current] +
          ((graph.IsWeighted()) ? graph.GetWeightOfEdge({current, neighbor})
                                : 1);

      // предполагаемая стоимость меньше текущей,
      // обновляем стоимость и добавляем соседа в очередь
      if (tentative_score < cost_from_start[neighbor]) {
        cost_from_start[neighbor] = tentative_score;

        // range_plus_cost для новой вершины (другого соседа)
        range_plus_cost[neighbor] =
            cost_from_start[neighbor] + heuristic_range(neighbor, goal);
        visited_verts.push({range_plus_cost[neighbor], neighbor});
      }
    }
  }

  // путь не был найден
  return std::numeric_limits<weight_t>::max();
}