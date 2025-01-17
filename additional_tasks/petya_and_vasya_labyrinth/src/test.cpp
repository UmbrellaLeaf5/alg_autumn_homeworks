#include <gtest/gtest.h>

#include "dag_relaxation.hpp"
#include "petya_and_vasya_labyrinth.hpp"

#define LONG_INF std::numeric_limits<long>::max()

namespace {

template <typename Key, typename Value>
inline bool operator==(const std::unordered_map<Key, Value>& lhs,
                       const std::unordered_map<Key, Value>& rhs) {
  if (lhs.size() != rhs.size()) return false;

  for (const auto& [key, value] : lhs)
    if (rhs.find(key) == rhs.end() || rhs.at(key) != value) return false;

  // else
  return true;
}

template <typename Key, typename Value>
inline bool operator!=(const std::unordered_map<Key, Value>& lhs,
                       const std::unordered_map<Key, Value>& rhs) {
  return !(lhs == rhs);
}

}  // namespace

TEST(DAGRelaxationTest, SimpleDAG_A) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = DAGRelaxation(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 5}, {"C", 3}, {"D", 7}, {"E", 4}, {"F", 6}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DAGRelaxationTest, SimpleDAG_B) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = DAGRelaxation(graph, std::string("B"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 9}, {"E", LONG_INF}, {"B", 0},
      {"D", 2}, {"C", LONG_INF}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DAGRelaxationTest, SimpleDAG_C) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = DAGRelaxation(graph, std::string("C"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 3}, {"E", 1}, {"B", LONG_INF}, {"D", 4}, {"C", 0}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DAGRelaxationTest, SimpleDAG_D) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = DAGRelaxation(graph, std::string("D"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 7}, {"E", LONG_INF}, {"B", LONG_INF},
      {"D", 0}, {"C", LONG_INF}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DAGRelaxationTest, SimpleDAG_E) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = DAGRelaxation(graph, std::string("E"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 2},        {"E", 0},        {"B", LONG_INF},
      {"D", LONG_INF}, {"C", LONG_INF}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DAGRelaxationTest, SimpleDAG_F) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = DAGRelaxation(graph, std::string("F"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 0},        {"E", LONG_INF}, {"B", LONG_INF},
      {"D", LONG_INF}, {"C", LONG_INF}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DAGRelaxationTest, UnweightedDAG) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B"});
  graph.AddEdge({"A", "C"});
  graph.AddEdge({"B", "D"});
  graph.AddEdge({"C", "D"});
  graph.AddEdge({"C", "E"});
  graph.AddEdge({"D", "F"});
  graph.AddEdge({"E", "F"});

  auto distances = DAGRelaxation(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 1}, {"C", 1}, {"D", 2}, {"E", 2}, {"F", 3}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DAGRelaxationTest, NonDirectedGraph) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B"});
  graph.AddEdge({"A", "C"});
  graph.AddEdge({"B", "D"});
  graph.AddEdge({"C", "D"});

  graph.MakeUndirected();

  ASSERT_THROW(DAGRelaxation(graph, std::string("A")), std::invalid_argument);
}

TEST(DAGRelaxationTest, NonExistingStartVertex) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B"});
  graph.AddEdge({"A", "C"});
  graph.AddEdge({"B", "D"});
  graph.AddEdge({"C", "D"});

  graph.MakeUndirected();

  ASSERT_THROW(DAGRelaxation(graph, std::string("E")), std::invalid_argument);
}

TEST(DAGRelaxationTest, DAGWithNoIncomingEdges) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  graph.AddEdge({"G", "F", 3});

  auto distances = DAGRelaxation(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 5}, {"C", 3},       {"D", 7},
      {"E", 4}, {"F", 6}, {"G", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DAGRelaxationTest, OneVertDAG) {
  Graph<std::string, long> graph;
  graph.AddVert("A");

  auto distances = DAGRelaxation(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {{"A", 0}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DAGRelaxationTest, TwoVertsDAG) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});

  auto distances = DAGRelaxation(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {{"A", 0},
                                                              {"B", 5}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DAGRelaxationTest, MultiplePathsToOneVertex) {
  Graph<std::string, long> graph;

  graph.AddEdge({"A", "B", 1});
  graph.AddEdge({"A", "C", 4});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 1});

  auto distances = DAGRelaxation(graph, std::string("A"));
  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 1}, {"C", 4}, {"D", 3}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DAGRelaxationTest, IntVerticesAndWeights) {
  Graph<int, long> graph;

  graph.AddEdge({1, 2, 5});
  graph.AddEdge({1, 3, 2});
  graph.AddEdge({2, 3, 1});

  auto distances = DAGRelaxation(graph, 1);
  std::unordered_map<int, long> expected_distances = {{1, 0}, {2, 5}, {3, 2}};

  ASSERT_EQ(distances, expected_distances);
}

// Тест с символами в качестве вершин и дробными значениями в качестве весов
TEST(DAGRelaxationTest, CharVerticesAndDoubleWeights) {
  Graph<std::string, double> graph;

  graph.AddEdge({"A", "B", 3.5});
  graph.AddEdge({"A", "C", 1.5});
  graph.AddEdge({"B", "C", 2.5});

  auto distances = DAGRelaxation(graph, std::string("A"));

  std::unordered_map<std::string, double> expected_distances = {
      {"A", 0.0}, {"B", 3.5}, {"C", 1.5}};

  ASSERT_EQ(distances, expected_distances);
}

// Тест со строками в качестве вершин и весами (long)
TEST(DAGRelaxationTest, StringVerticesAndLongWeights) {
  Graph<std::string, long> graph;

  graph.AddEdge({"A", "B", 10});
  graph.AddEdge({"A", "C", 5});
  graph.AddEdge({"B", "C", 2});

  auto distances = DAGRelaxation(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 10}, {"C", 5}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(ComplicatedBadSolutionTest, PetyaWinsSimple) {
  std::stringstream ss("P0#V");
  std::stringstream output;

  ComplicatedBadSolution(ss, output);
  EXPECT_EQ(output.str(), "Petya! with 1\n");
}

TEST(ComplicatedBadSolutionTest, VasyaWinsSimple) {
  std::stringstream ss("V0#P");
  std::stringstream output;

  ComplicatedBadSolution(ss, output);
  EXPECT_EQ(output.str(), "Vasya! with 1\n");
}

TEST(ComplicatedBadSolutionTest, DrawSimple) {
  std::stringstream ss("P0#V0");
  std::stringstream output;

  ComplicatedBadSolution(ss, output);
  EXPECT_EQ(output.str(), "Draw! with 1\n");
}

TEST(ComplicatedBadSolutionTest, ValeryWinsSimple) {
  std::stringstream ss("V#P\n");
  std::stringstream output;

  ComplicatedBadSolution(ss, output);
  EXPECT_EQ(output.str(), "Deadlock! Valery!\n");
}

TEST(ComplicatedGoodBadSolutionTest, PetyaWins) {
  std::stringstream ss;
  ss << "##V########\n"
     << "#000000000#\n"
     << "#0#####0#0#\n"
     << "#0#00000#0#\n"
     << "#####0###0#\n"
     << "P0#000#000#\n"
     << "#0###0#0#0#\n"
     << "#0#000#0#0#\n"
     << "#0#0###0#0#\n"
     << "#000#000#0#\n"
     << "#0###0#####\n\n";

  auto maze = ss.str();

  std::stringstream output;

  ComplicatedBadSolution(ss, output);
  EXPECT_EQ(output.str(), "Petya! with 6\n");

  ss << maze;

  std::stringstream output_12;

  GoodSolution(ss, output_12);
  EXPECT_EQ(output_12.str(), "Petya! with 6\n");

  std::stringstream ss_2;
  ss_2 << "#########V##\n"
       << "#000000#000#\n"
       << "#0####0#0#0#\n"
       << "#0#00#0#0#0#\n"
       << "00#0##0#0#0#\n"
       << "#0#0#00#0#0#\n"
       << "#0#0#0##0#0#\n"
       << "#0#00000000#\n"
       << "#0##########\n"
       << "#00000000000\n"
       << "##P#########\n\n";

  maze = ss_2.str();

  std::stringstream output_2;

  ComplicatedBadSolution(ss_2, output_2);
  EXPECT_EQ(output_2.str(), "Petya! with 8\n");

  ss_2 << maze;

  std::stringstream output_22;

  GoodSolution(ss_2, output_22);
  EXPECT_EQ(output_22.str(), "Petya! with 8\n");
}

TEST(ComplicatedGoodBadSolutionTest, VasyaWins) {
  std::stringstream ss;
  ss << "##V########\n"
     << "#000000000#\n"
     << "#0#####0#0#\n"
     << "#0#00000#0#\n"
     << "#####0###0#\n"
     << "P0#000#000#\n"
     << "#0###0#0#0#\n"
     << "#0#000#0#0#\n"
     << "#0#0###0#0#\n"
     << "#000#000#0#\n"
     << "#####0#####\n\n";

  auto maze = ss.str();

  std::stringstream output;

  ComplicatedBadSolution(ss, output);
  EXPECT_EQ(output.str(), "Vasya! with 21\n");

  ss << maze;

  std::stringstream output_12;

  GoodSolution(ss, output_12);
  EXPECT_EQ(output_12.str(), "Vasya! with 21\n");

  std::stringstream ss_2;
  ss_2 << "###V#########\n"
       << "#00000000#00#\n"
       << "#0####0####0#\n"
       << "00#0#0000000#\n"
       << "#0#0#######0#\n"
       << "#0#0#0000000#\n"
       << "#0#0#0###0###\n"
       << "#0#00000#000#\n"
       << "#0#########0#\n"
       << "#0000#000000#\n"
       << "####0##P#####\n\n";

  maze = ss_2.str();

  std::stringstream output_2;

  ComplicatedBadSolution(ss_2, output_2);
  EXPECT_EQ(output_2.str(), "Vasya! with 6\n");

  ss_2 << maze;

  std::stringstream output_22;

  GoodSolution(ss_2, output_22);
  EXPECT_EQ(output_22.str(), "Vasya! with 6\n");
}

TEST(ComplicatedBadSolutionTest, PetyaWinsHardDAGRelaxation) {
  std::stringstream ss;
  ss << "###P####\n"
     << "#000000#\n"
     << "#000000#\n"
     << "#000000#\n"
     << "#0000000\n"
     << "#000000#\n"
     << "#000000#\n"
     << "#000000#\n"
     << "#000000#\n"
     << "#000000#\n"
     << "####V###\n\n";

  std::stringstream output;

  ComplicatedBadSolution(ss, output);
  EXPECT_EQ(output.str(), "Petya! with 8\n");
}

TEST(ComplicatedGoodSolutionTest, PetyaWinsHardAStar) {
  std::stringstream ss;
  ss << "###P####\n"
     << "#000000#\n"
     << "#000000#\n"
     << "#000000#\n"
     << "#0000000\n"
     << "#000000#\n"
     << "#000000#\n"
     << "#000000#\n"
     << "#000000#\n"
     << "#000000#\n"
     << "####V###\n\n";

  std::stringstream output;

  GoodSolution(ss, output);
  EXPECT_EQ(output.str(), "Petya! with 8\n");
}

TEST(ComplicatedGoodBadSolutionTest, Draw) {
  std::stringstream ss;
  ss << "##V########\n"
     << "#000000000#\n"
     << "#0#####0#0#\n"
     << "#0#00000#0#\n"
     << "#####0###0#\n"
     << "P0#000#000#\n"
     << "#0###0#0#0#\n"
     << "#0#000#0#0#\n"
     << "#0#0###0#0#\n"
     << "#000#000#0#\n"
     << "###########\n\n";

  auto maze = ss.str();

  std::stringstream output;

  ComplicatedBadSolution(ss, output);
  EXPECT_EQ(output.str(), "Draw! with 25\n");

  ss << maze;

  std::stringstream output_12;

  GoodSolution(ss, output_12);
  EXPECT_EQ(output_12.str(), "Draw! with 25\n");
}

TEST(ComplicatedGoodBadSolutionTest, ValeryWins) {
  std::stringstream ss;
  ss << "##V########\n"
     << "#000000000#\n"
     << "#0#######0#\n"
     << "#0#00000#0#\n"
     << "#####0###0#\n"
     << "P0#000#000#\n"
     << "#0###0#0#0#\n"
     << "#0#000#0#0#\n"
     << "#0#0###0#0#\n"
     << "#000#000#0#\n"
     << "###########\n\n";

  auto maze = ss.str();

  std::stringstream output;

  ComplicatedBadSolution(ss, output);
  EXPECT_EQ(output.str(), "Deadlock! Valery!\n");

  ss << maze;

  std::stringstream output_12;

  GoodSolution(ss, output_12);
  EXPECT_EQ(output_12.str(), "Deadlock! Valery!\n");
}
