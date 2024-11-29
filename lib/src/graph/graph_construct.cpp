#include "graph.hpp"

GRAPH_TEMPLATE_CONSTRUCT_FOR_STRING;
GRAPH_TEMPLATE_CONSTRUCT_FOR_INTEGRAL;

template <AllowedVertType vert_t, AllowedWeightType weight_t>
Graph<vert_t, weight_t> Graph<vert_t, weight_t>::GraphNonWeighted(
    const std::vector<std::pair<vert_t, vert_t>>& edges_pairs) {
  if (edges_pairs.empty()) return Graph();

  std::vector<Edge> edges{};
  edges.reserve(edges_pairs.size());

  for (const auto& edge : edges_pairs) {
    edges.push_back(Edge(edge.first, edge.second));
  }

  return Graph(edges);
}

template <AllowedVertType vert_t, AllowedWeightType weight_t>
Graph<vert_t, weight_t> Graph<vert_t, weight_t>::GraphWeighted(
    const std::vector<std::pair<vert_t, vert_t>>& edges_pairs,
    const std::vector<weight_t>& weights) {
  if (edges_pairs.empty() && weights.empty()) return Graph();

  std::vector<Edge> edges{};
  edges.reserve(edges_pairs.size());

  if (edges_pairs.size() != weights.size())
    throw std::invalid_argument(
        "GraphWeighted: the sizes of the edges and weights vectors do not "
        "match.");

  for (size_t i = 0; i < weights.size(); i++) {
    auto edge = Edge(edges_pairs[i].first, edges_pairs[i].second, weights[i]);
    edges.push_back(edge);
  }

  return Graph(edges);
}

template <AllowedVertType vert_t, AllowedWeightType weight_t>
Graph<vert_t, weight_t> Graph<vert_t, weight_t>::GraphWeighted(
    const std::vector<std::tuple<vert_t, vert_t, weight_t>>& edges_tuples) {
  if (edges_tuples.empty()) return Graph();

  std::vector<Edge> edges;

  for (const auto& [start, end, weight] : edges_tuples)
    edges.emplace_back(start, end, weight);

  return Graph(edges);
}

template <AllowedVertType vert_t, AllowedWeightType weight_t>
Graph<vert_t, weight_t> Graph<vert_t, weight_t>::GraphFromMap(
    const std::unordered_map<std::string, weight_t>& edges_dict) {
  if (edges_dict.empty()) return Graph();

  std::vector<Graph<vert_t, weight_t>::Edge> edges;

  for (const auto& [edge_str, weight] : edges_dict) {
    vert_t start_vert, end_vert;
    std::tie(start_vert, end_vert) = ParseEdgeString(edge_str);

    edges.emplace_back(start_vert, end_vert, weight);
  }

  return Graph(edges);
}

template <AllowedVertType vert_t, AllowedWeightType weight_t>
Graph<vert_t, weight_t> Graph<vert_t, weight_t>::GraphFromStrs(
    const std::vector<std::string>& edges_strs) {
  if (edges_strs.empty()) return Graph();

  std::vector<Graph<vert_t, weight_t>::Edge> edges;

  for (const auto& edge_str : edges_strs) {
    vert_t start_vert, end_vert;
    std::tie(start_vert, end_vert) = ParseEdgeString(edge_str);

    edges.emplace_back(start_vert, end_vert);
  }

  return Graph(edges);
}

template <AllowedVertType vert_t, AllowedWeightType weight_t>
Graph<vert_t, weight_t> Graph<vert_t, weight_t>::GraphFromAdjList(
    const std::unordered_map<vert_t, std::vector<vert_t>>& adj_list_dict) {
  if (adj_list_dict.empty()) return Graph();

  std::vector<Edge> edges{};

  for (const auto& vert_str_pair : adj_list_dict) {
    auto vert = vert_str_pair.first;
    for (const auto& vert_2 : vert_str_pair.second)
      edges.push_back(Edge(vert, vert_2));
  }

  return Graph(edges);
}
