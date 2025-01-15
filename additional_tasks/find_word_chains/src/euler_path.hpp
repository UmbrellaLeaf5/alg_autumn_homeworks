#pragma once

#include "graph.hpp"

namespace {

/**
 * @brief
 *
 * @tparam vert_t
 * @tparam weight_t
 * @param v
 * @param graph
 * @param visited
 */

/**
 * @brief Вспомогательная рекурсивная функция для обхода графа в глубину (DFS).
 *
 * @details Проверяет связность графа, начиная с вершины `v`.  Эта функция
 * используется внутри `HasEulerPath` для проверки связности после проверки
 * количества вершин с нечётной степенью.
 *
 * @tparam vert_t: тип вершин графа.
 * @tparam weight_t: тип весов рёбер графа.
 * @param v: начальная вершина для DFS.
 * @param graph: граф, в котором выполняется обход.
 * @param visited: посещенные вершины.
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline void HasEulerPathStep(const vert_t& v,
                             const Graph<vert_t, weight_t>& graph,
                             std::unordered_map<vert_t, bool>& visited) {
  visited[v] = true;

  // получаем соседнюю вершину (откуда идёт ребро), если соседняя вершина ещё не
  // посещена, рекурсивно вызываем DFS для неё.

  for (const auto& edge_tuple : graph.Edges()) {
    if (StartVertFromTuple(edge_tuple) == v) {
      vert_t neighbor = EndVertFromTuple(edge_tuple);

      if (!visited[neighbor]) HasEulerPathStep(neighbor, graph, visited);
    }

    if (EndVertFromTuple(edge_tuple) == v && !graph.IsDirected()) {
      vert_t neighbor = StartVertFromTuple(edge_tuple);

      if (!visited[neighbor]) HasEulerPathStep(neighbor, graph, visited);
    }
  }
}

/**
 * @brief Вычисляет исходящую степень вершины в графе.
 *
 * @tparam vert_t: тип вершин графа.
 * @tparam weight_t: тип весов рёбер графа.
 * @param graph: граф, в котором вычисляется исходящая степень.
 * @param v: вершина, для которой вычисляется исходящая степень.
 *
 * @return size_t: исходящая степень вершины (количество исходящих ребер).
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline size_t OutDeg(const Graph<vert_t, weight_t>& graph, const vert_t& v) {
  return graph.GetAdjList()[v].size();
}

/**
 * @brief Вычисляет входящую степень вершины в графе.
 *
 * @tparam vert_t: тип вершин графа.
 * @tparam weight_t: тип весов рёбер графа.
 * @param graph: граф, в котором вычисляется входящая степень.
 * @param v: вершина, для которой вычисляется входящая степень.
 *
 * @return size_t: входящая степень вершины (количество входящих ребер).
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline size_t InDeg(const Graph<vert_t, weight_t>& graph, const vert_t& v) {
  // для неориентированных графов входящая степень равна исходящей
  if (!graph.IsDirected()) return OutDeg(graph, v);

  size_t res = 0;

  auto adj_list = graph.GetAdjList();

  for (const auto& u : graph.Verts())
    if (Contains(adj_list[u], v)) res++;

  return res;
}

}  // namespace

/**
 * @brief Проверяет, существует ли в графе эйлеров путь.
 *
 * @details Эйлеров путь — это путь в графе, который проходит через каждое ребро
 * ровно один раз. Функция сначала проверяет необходимое, но недостаточное
 * условие существования эйлерова пути: количество вершин с нечетной степенью
 * должно быть не больше двух. Затем она проверяет связность графа с помощью
 * DFS.
 *
 * @tparam vert_t: тип вершин графа.
 * @tparam weight_t: тип весов рёбер графа.
 * @param graph: граф, для которого проверяется наличие эйлерова пути.
 *
 * @return `true`, если эйлеров путь существует
 * @return `false` в противном случае.
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline bool HasEulerPath(const Graph<vert_t, weight_t>& graph) {
  // считаем количество вершин с нечетной степенью
  size_t odd_vert_count = 0;

  for (const auto& v : graph.Verts())
    if (OutDeg(graph, v) != InDeg(graph, v))
      odd_vert_count++;

    else if (!graph.IsDirected() && OutDeg(graph, v) % 2 != 0)
      odd_vert_count++;

  // если вершин с нечетной степенью больше двух, то эйлерова пути нет
  if (odd_vert_count > 2) return false;

  std::unordered_map<vert_t, bool> visited;
  for (const auto& vert : graph.Verts()) visited[vert] = false;

  std::unordered_map<vert_t, size_t> degree;
  for (const auto& vert : graph.Verts())
    degree[vert] = graph.IsDirected() ? InDeg(graph, vert) + OutDeg(graph, vert)
                                      : OutDeg(graph, vert);

  // находим первую вершину с ненулевой степенью и начинаем DFS
  for (const auto& v : graph.Verts())
    if (degree[v] > 0) {
      HasEulerPathStep(v, graph, visited);
      break;
    }

  // проверяем, все ли вершины с ненулевой степенью были посещены.
  for (const auto& v : graph.Verts())
    if (degree[v] > 0 && !visited[v]) return false;

  // все вершины с ненулевой степенью были посещены, эйлеров путь существует
  return true;
}
