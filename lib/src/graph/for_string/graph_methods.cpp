#include "../graph.hpp"

template class Graph<std::string, long>;
template class Graph<std::string, double>;

template <AllowedVertType vert_t, AllowedWeightType weight_t>
void Graph<vert_t, weight_t>::RemoveVert(vert_t vert) {
  if (!Contains(Verts(), vert))
    throw std::invalid_argument("RemoveVert: there is no such vert in graph: " +
                                vert);

  verts_.erase(std::remove(verts_.begin(), verts_.end(), vert), verts_.end());

  edges_.erase(std::remove_if(edges_.begin(), edges_.end(),
                              [vert](const Edge& edge) {
                                return edge.StartVert() == vert ||
                                       edge.EndVert() == vert;
                              }),
               edges_.end());
}

template <AllowedVertType vert_t, AllowedWeightType weight_t>
std::pair<vert_t, vert_t> Graph<vert_t, weight_t>::ParseEdgeString(
    const std::string& edge_str) {
  const size_t pos = edge_str.find("->");

  if (pos == std::string::npos)
    throw std::invalid_argument("EdgeString: invalid edge string format: " +
                                edge_str);

  vert_t start_vert = edge_str.substr(0, pos);
  vert_t end_vert = edge_str.substr(pos + 2);

  return {start_vert, end_vert};
}

// DELETED methods

template <AllowedVertType vert_t, AllowedWeightType weight_t>
std::vector<std::vector<vert_t>>
Graph<vert_t, weight_t>::GetAdjListWithoutKeys() const {
  throw std::logic_error(
      "GetAdjListWithoutKeys: this method is deleted for std::string.");
}

template <AllowedVertType vert_t, AllowedWeightType weight_t>
std::vector<std::vector<weight_t>> Graph<vert_t, weight_t>::GetAdjMatrix()
    const {
  throw std::logic_error(
      "GetAdjMatrix: this method is deleted for std::string.");
}
