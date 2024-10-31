#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "graph/graph.hpp"

namespace {

template <AllowedVertType vert_t>
inline void FindCutVerticesStep(
    const vert_t& vert, const vert_t& parent_vert, size_t& curr_index,
    std::unordered_map<vert_t, size_t>& indexes,
    std::unordered_map<vert_t, size_t>& low_links,
    std::unordered_map<vert_t, bool>& visited,
    std::unordered_map<vert_t, std::vector<vert_t>>& adj_list,
    std::vector<vert_t>& cut_verts) {
  // в curr_index храним количество ранее обработанных вершин,
  // indexes[v] - это "время входа" в вершину v
  visited[vert] = true;
  indexes[vert] = low_links[vert] = curr_index++;

  size_t count = 0;

  // перебираем рёбра, исходящие из vert
  for (const auto& u_vert : adj_list[vert])
    if (!visited[u_vert]) {
      // вершина u_vert ранее не посещалась; запускаемся из неё рекурсивно
      FindCutVerticesStep(u_vert, vert, curr_index, indexes, low_links, visited,
                          adj_list, cut_verts);

      count++;

      low_links[vert] = std::min(low_links[vert], low_links[u_vert]);

      if (vert != parent_vert && low_links[u_vert] >= indexes[vert])
        cut_verts.push_back(vert);

    } else if (u_vert !=
               parent_vert)  // если предок vert уже посещен - игнорируем
      low_links[vert] = std::min(low_links[vert], low_links[u_vert]);

  if (vert == parent_vert && count >= 2) cut_verts.push_back(vert);
}

}  // namespace

/**
 * @brief Поиск точек сочленения в графе (используя элементы алгоритма Тарьяна)
 * @tparam vert_t: тип вершин
 * @tparam weight_t: тип весов
 * @param graph: исходный граф
 * @return std::vector<vert_t>: точки сочленения в графе
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline std::vector<vert_t> FindCutVertices(
    const Graph<vert_t, weight_t>& graph) {
  if (graph.Verts().empty()) return {};

  std::vector<vert_t> cut_verts;

  size_t curr_index = 0;

  std::unordered_map<vert_t, std::vector<vert_t>> adj_list = graph.GetAdjList();

  std::unordered_map<vert_t, size_t> indexes;
  std::unordered_map<vert_t, size_t> low_links;
  std::unordered_map<vert_t, bool> visited;

  for (const auto& vert : graph.Verts()) {
    indexes[vert] = low_links[vert] = 0;
    visited[vert] = false;
  }

  for (const auto& vert : graph.Verts())
    if (!visited[vert])
      FindCutVerticesStep(vert, vert, curr_index, indexes, low_links, visited,
                          adj_list, cut_verts);

  std::reverse(cut_verts.begin(), cut_verts.end());

  return cut_verts;
}