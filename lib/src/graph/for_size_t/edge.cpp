#include "../graph.hpp"

template class Graph<short, long>;
template class Graph<int, long>;
template class Graph<size_t, long>;

template class Graph<short, double>;
template class Graph<int, double>;
template class Graph<size_t, double>;

template <AllowedVertType vert_t, AllowedWeightType weight_t>
const std::string& Graph<vert_t, weight_t>::Edge::Name() const {
  static std::string name;

  if (IsWeighted())
    name = "[" + std::to_string(StartVert()) + "->" +
           std::to_string(EndVert()) + ", w: " + std::to_string(Weight()) + "]";
  else
    name = "[" + std::to_string(StartVert()) + "->" +
           std::to_string(EndVert()) + "]";

  // example: "[4->5]"

  return name;
}