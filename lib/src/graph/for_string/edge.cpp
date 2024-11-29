#include "../graph.hpp"

GRAPH_TEMPLATE_CONSTRUCT_FOR_STRING;

template <AllowedVertType vert_t, AllowedWeightType weight_t>
const std::string& Graph<vert_t, weight_t>::Edge::Name() const {
  static std::string name;

  if (IsWeighted())
    name = "['" + StartVert() + "'->'" + EndVert() +
           "', w: " + std::to_string(Weight()) + "]";
  else
    name = "['" + StartVert() + "'->'" + EndVert() + "']";

  // example: "["Paris"->"Berlin"]"

  return name;
}
