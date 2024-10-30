#pragma once

#include <algorithm>
#include <stack>
#include <stdexcept>

#include "graph/graph.hpp"

namespace {
template <typename vert_t>
void StronglyConnectedComponentsStep(
    const vert_t& v, size_t& curr_index, std::stack<vert_t>& verts_stack,
    std::unordered_map<vert_t, size_t>& indexes,
    std::unordered_map<vert_t, size_t>& low_links,
    std::unordered_map<vert_t, bool>& is_on_stack,
    std::unordered_map<vert_t, std::vector<vert_t>>& adj_list,
    std::vector<std::vector<vert_t>>& strongly_connected_components) {
  // в curr_index храним количество ранее обработанных вершин,
  // indexes[v] - это "время входа" в вершину v
  indexes[v] = curr_index;
  low_links[v] = curr_index;
  curr_index++;

  verts_stack.push(v);

  // is_on_stack нужно, чтобы проверять принадлежность вершины стеку за O(1)
  is_on_stack[v] = true;

  // перебираем рёбра, исходящие из v
  for (auto& w : adj_list[v]) {
    if (indexes[w] == 0) {
      // вершина w ранее не посещалась; запускаемся из неё рекурсивно
      StronglyConnectedComponentsStep(w, curr_index, verts_stack, indexes,
                                      low_links, is_on_stack, adj_list,
                                      strongly_connected_components);

      low_links[v] = std::min(low_links[v], low_links[w]);
    } else if (is_on_stack[w])
      // вершина w находится в стеке, значит, принадлежит той же компоненте
      // сильной связности, что и v

      // если w не в стеке, значит, ребро (v, w) ведёт в ранее обработанную
      // компоненту сильной связности и должна быть проигнорирована
      low_links[v] = std::min(low_links[v], low_links[w]);
  }

  // вершина v - корень текущей компоненты сильной связности,
  // все вершины в стеке от v и выше образуют эту компоненту
  if (low_links[v] == indexes[v]) {
    vert_t u{};
    std::vector<vert_t> strongly_connected_component{};

    do {
      u = verts_stack.top();
      verts_stack.pop();

      is_on_stack[u] = false;
      strongly_connected_component.push_back(u);
    } while (u != v);

    strongly_connected_components.push_back(strongly_connected_component);
  }
}

}  // namespace

/**
 * @brief Поиск компонент сильной связности в ориентированного графа по
 * алгоритму Тарьяна
 * @tparam vert_t: тип вершин
 * @tparam weight_t: тип весов
 * @param graph: исходный граф
 * @throw std::invalid_argument("StronglyConnectedComponents: graph is not
 * directed.");
 * @return std::vector<std::vector<vert_t>>: компоненты сильной связности
 */
template <AllowedVertType vert_t, AllowedWeightType weight_t>
std::vector<std::vector<vert_t>> StronglyConnectedComponents(
    Graph<vert_t, weight_t> graph) {
  if (!graph.IsDirected())
    throw std::invalid_argument(
        "StronglyConnectedComponents: graph is not directed.");

  std::vector<std::vector<vert_t>> strongly_connected_component{};

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

  for (const auto& v : graph.Verts())
    if (indexes[v] == 0)
      StronglyConnectedComponentsStep(v, curr_index, verts_stack, indexes,
                                      low_links, is_on_stack, adj_list,
                                      strongly_connected_component);

  return strongly_connected_component;
}
