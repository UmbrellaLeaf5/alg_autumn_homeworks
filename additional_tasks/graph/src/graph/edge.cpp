#include "graph.hpp"

template class Graph<std::string, long>;
template class Graph<std::string, double>;

template class Graph<short, long>;
template class Graph<int, long>;
template class Graph<size_t, long>;

template class Graph<short, double>;
template class Graph<int, double>;
template class Graph<size_t, double>;

template <typename vert_t, typename weight_t>
Graph<vert_t, weight_t>::Edge::Edge(vert_t start_vert, vert_t end_vert,
                                    weight_t weight)
    : start_vert_{start_vert}, end_vert_{end_vert}, weight_{weight} {
  if (weight <= 0)
    throw std::invalid_argument("Edge: weight must be greater than zero.");
}

template <typename vert_t, typename weight_t>
Graph<vert_t, weight_t>::Edge::Edge(std::pair<vert_t, vert_t> edge_pair)
    : start_vert_{edge_pair.first}, end_vert_{edge_pair.second} {}

template <typename vert_t, typename weight_t>
Graph<vert_t, weight_t>::Edge::Edge(
    std::tuple<vert_t, vert_t, weight_t> edge_tuple)
    : start_vert_{StartVertFromTuple(edge_tuple)},
      end_vert_{EndVertFromTuple(edge_tuple)},
      weight_{WeightFromTuple(edge_tuple)} {}

template <typename vert_t, typename weight_t>
weight_t Graph<vert_t, weight_t>::Edge::Weight() const {
  if (!IsWeighted())
    throw std::logic_error("Edge: " + Name() + " is not weighted.");

  return weight_;
}

template <typename vert_t, typename weight_t>
auto Graph<vert_t, weight_t>::Edge::operator<=>(const Edge& rhs) const {
  if (!(IsWeighted() && rhs.IsWeighted()))
    throw std::invalid_argument("Edge: unweighted edges are not comparable.");

  return weight_ <=> rhs.Weight();
}
