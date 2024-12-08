#pragma once

#include <algorithm>
#include <stack>
#include <stdexcept>

#include "graph.hpp"

namespace {

template <AllowedVertType vert_t>
inline void StronglyConnectedComponentsStep(
    const vert_t& vert, size_t& curr_index, std::stack<vert_t>& verts_stack,
    std::unordered_map<vert_t, size_t>& indexes,
    std::unordered_map<vert_t, size_t>& low_links,
    std::unordered_map<vert_t, bool>& is_on_stack,
    std::unordered_map<vert_t, std::vector<vert_t>>& adj_list,
    std::vector<std::vector<vert_t>>& strongly_connected_components) {
  // в curr_index храним количество ранее обработанных вершин,
  // indexes[vert] - это "время входа" в вершину vert
  indexes[vert] = low_links[vert] = curr_index++;

  verts_stack.push(vert);

  // is_on_stack нужно, чтобы проверять принадлежность вершины стеку за O(1)
  is_on_stack[vert] = true;

  // перебираем рёбра, исходящие из vert
  for (auto& u_vert : adj_list[vert]) {
    if (indexes[u_vert] == 0) {
      // вершина u_vert ранее не посещалась; запускаемся из неё рекурсивно
      StronglyConnectedComponentsStep(u_vert, curr_index, verts_stack, indexes,
                                      low_links, is_on_stack, adj_list,
                                      strongly_connected_components);

      low_links[vert] = std::min(low_links[vert], low_links[u_vert]);
    } else if (is_on_stack[u_vert])
      // вершина u_vert находится в стеке, значит, принадлежит той же компоненте
      // сильной связности, что и vert

      // если u_vert не в стеке, значит, ребро (vert, u_vert) ведёт в ранее
      // обработанную компоненту сильной связности и должна быть проигнорирована
      low_links[vert] = std::min(low_links[vert], low_links[u_vert]);
  }

  // вершина vert - корень текущей компоненты сильной связности,
  // все вершины в стеке от vert и выше образуют эту компоненту
  if (low_links[vert] == indexes[vert]) {
    vert_t u_vert;
    std::vector<vert_t> strongly_connected_component;

    do {
      u_vert = verts_stack.top();
      verts_stack.pop();

      is_on_stack[u_vert] = false;
      strongly_connected_component.push_back(u_vert);
    } while (u_vert != vert);

    strongly_connected_components.push_back(strongly_connected_component);
  }
}

}  // namespace

/**
 * @brief Поиск компонент сильной связности в ориентированного графа по
 * алгоритму Тарьяна.
 * @tparam vert_t: тип вершин
 * @tparam weight_t: тип весов
 * @param graph: исходный граф
 * @throw `std::invalid_argument("StronglyConnectedComponents: graph is not
 * directed.")`.
 * @return `std::vector<std::vector<vert_t>>`: компоненты сильной связности
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
std::vector<std::vector<vert_t>> StronglyConnectedComponents(
    const Graph<vert_t, weight_t>& graph) {
  if (!graph.IsDirected())
    throw std::invalid_argument(
        "StronglyConnectedComponents: graph is not directed.");

  if (graph.Verts().empty()) return {};

  std::vector<std::vector<vert_t>> strongly_connected_component;

  std::stack<vert_t> verts_stack;
  size_t curr_index = 0;

  std::unordered_map<vert_t, std::vector<vert_t>> adj_list = graph.GetAdjList();

  std::unordered_map<vert_t, size_t> indexes;
  std::unordered_map<vert_t, size_t> low_links;
  std::unordered_map<vert_t, bool> is_on_stack;

  for (const auto& vert : graph.Verts()) {
    indexes[vert] = low_links[vert] = 0;
    is_on_stack[vert] = false;
  }

  for (const auto& vert : graph.Verts())
    if (indexes[vert] == 0)
      StronglyConnectedComponentsStep(vert, curr_index, verts_stack, indexes,
                                      low_links, is_on_stack, adj_list,
                                      strongly_connected_component);

  return strongly_connected_component;
}
