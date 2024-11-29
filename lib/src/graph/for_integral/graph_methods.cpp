#include "../graph.hpp"

GRAPH_TEMPLATE_CONSTRUCT_FOR_INTEGRAL;

template <AllowedVertType vert_t, AllowedWeightType weight_t>
std::vector<std::vector<vert_t>>
Graph<vert_t, weight_t>::GetAdjListWithoutKeys() const {
  std::vector<std::vector<vert_t>> adj_list(
      *std::max_element(Verts().begin(), Verts().end()) + 1);

  for (const auto& edge : edges_) {
    adj_list[edge.StartVert()].push_back(edge.EndVert());
    if (!IsDirected()) adj_list[edge.EndVert()].push_back(edge.StartVert());
  }

  return adj_list;
}

template <AllowedVertType vert_t, AllowedWeightType weight_t>
std::vector<std::vector<weight_t>> Graph<vert_t, weight_t>::GetAdjMatrix()
    const {
  std::vector<std::vector<weight_t>> adj_matrix(
      VertsAmount(), std::vector<weight_t>(VertsAmount(), 0));

  for (const auto& edge : edges_)
    if (edge.IsWeighted()) {
      adj_matrix[edge.StartVert()][edge.EndVert()] = edge.Weight();
      if (!IsDirected())
        adj_matrix[edge.EndVert()][edge.StartVert()] = edge.Weight();
    } else {
      adj_matrix[edge.StartVert()][edge.EndVert()] = 1;
      if (!IsDirected()) adj_matrix[edge.EndVert()][edge.StartVert()] = 1;
    }

  return adj_matrix;
}

template <AllowedVertType vert_t, AllowedWeightType weight_t>
std::pair<vert_t, vert_t> Graph<vert_t, weight_t>::ParseEdgeString(
    const std::string& edge_str) {
  const vert_t pos = edge_str.find("->");

  if (size_t(pos) == std::string::npos)
    throw std::invalid_argument("EdgeString: invalid edge string format: " +
                                edge_str);

  try {
    vert_t start_vert = std::stoul(edge_str.substr(0, pos));
    vert_t end_vert = std::stoul(edge_str.substr(pos + 2));

    return {start_vert, end_vert};
  }

  catch (...) {
    throw std::invalid_argument(
        "EdgeString: invalid edge string format "
        "(vertices should be numbers): " +
        edge_str);
  }
}

template <AllowedVertType vert_t, AllowedWeightType weight_t>
void Graph<vert_t, weight_t>::RemoveVert(vert_t vert) {
  if (!Contains(Verts(), vert))
    throw std::invalid_argument("RemoveVert: there is no such vert in graph: " +
                                std::to_string(vert));

  verts_.erase(std::remove(verts_.begin(), verts_.end(), vert), verts_.end());

  edges_.erase(std::remove_if(edges_.begin(), edges_.end(),
                              [vert](const Edge& edge) {
                                return edge.StartVert() == vert ||
                                       edge.EndVert() == vert;
                              }),
               edges_.end());
}