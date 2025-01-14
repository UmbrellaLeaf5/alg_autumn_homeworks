#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "graph.hpp"

namespace {

template <AllowedVertType vert_t>
inline void FindBridgesStep(
    const vert_t& vert, const vert_t& parent_vert, size_t& curr_index,
    std::unordered_map<vert_t, size_t>& indexes,
    std::unordered_map<vert_t, size_t>& low_links,
    std::unordered_map<vert_t, bool>& visited,
    std::unordered_map<vert_t, std::vector<vert_t>>& adj_list,
    std::vector<std::pair<vert_t, vert_t>>& bridges) {
  // в curr_index храним количество ранее обработанных вершин,
  // indexes[v] - это "время входа" в вершину v
  visited[vert] = true;
  indexes[vert] = low_links[vert] = curr_index++;

  // перебираем рёбра, исходящие из vert
  for (const auto& u_vert : adj_list[vert])
    if (!visited[u_vert]) {
      // вершина u_vert ранее не посещалась; запускаемся из неё рекурсивно
      FindBridgesStep(u_vert, vert, curr_index, indexes, low_links, visited,
                      adj_list, bridges);

      low_links[vert] = std::min(low_links[vert], low_links[u_vert]);

      // если время входа в vert меньше, чем время низкой связи u_vert,
      // то ребро {u_vert->vert} - это мост
      if (low_links[u_vert] > indexes[vert]) bridges.push_back({vert, u_vert});
    } else if (u_vert !=
               parent_vert)  // если предок vert уже посещен - игнорируем
      low_links[vert] = std::min(low_links[vert], low_links[u_vert]);
}

}  // namespace

/**
 * @brief Поиск мостов в графе (используя элементы алгоритма Тарьяна).
 *
 * @tparam vert_t: тип вершин.
 * @tparam weight_t: тип весов.
 * @param graph: исходный граф.
 *
 * @return `std::vector<std::pair<vert_t, vert_t>>`: мосты в графе.
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
inline std::vector<std::pair<vert_t, vert_t>> FindBridges(
    const Graph<vert_t, weight_t>& graph) {
  if (graph.Verts().empty()) return {};

  std::vector<std::pair<vert_t, vert_t>> bridges;

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
      FindBridgesStep(vert, vert, curr_index, indexes, low_links, visited,
                      adj_list, bridges);

  std::reverse(bridges.begin(), bridges.end());

  return bridges;
}
