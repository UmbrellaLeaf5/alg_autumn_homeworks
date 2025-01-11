#include <gtest/gtest.h>

#include "johnson.hpp"

#define LONG_INF std::numeric_limits<long>::max()
#define INF std::numeric_limits<int>::max()

namespace {

std::unordered_map<int, std::unordered_map<int, int>>
ConvertVectorVectorToUnorderedMap(const std::vector<std::vector<int>>& vec) {
  std::unordered_map<int, std::unordered_map<int, int>> result_map;

  for (size_t i = 0; i < vec.size(); i++) {
    const std::vector<int>& inner_vec = vec[i];
    std::unordered_map<int, int> inner_map;

    for (size_t j = 0; j < inner_vec.size(); j++) inner_map[j] = inner_vec[j];

    result_map[i] = inner_map;
  }

  return result_map;
}

}  // namespace

TEST(BellmanFordTest, BasicTest) {
  Graph<int, int> graph = Graph<int, int>::GraphWeighted({{0, 1, 10},
                                                          {0, 2, 5},
                                                          {1, 2, 2},
                                                          {1, 3, 1},
                                                          {2, 1, 3},
                                                          {2, 3, 9},
                                                          {2, 4, 2},
                                                          {3, 4, 4},
                                                          {4, 0, 7},
                                                          {4, 3, 6}});

  auto distances = BellmanFord(graph, 0);

  std::unordered_map<int, int> expected = {
      {0, 0}, {1, 8}, {2, 5}, {3, 9}, {4, 7}};

  ASSERT_EQ(distances, expected);
}

TEST(BellmanFordTest, Graph) {
  Graph<std::string, int> graph =
      Graph<std::string, int>::GraphWeighted({{"A", "B", 5},
                                              {"A", "C", 2},
                                              {"B", "D", 4},
                                              {"C", "B", 1},
                                              {"C", "D", 6}});

  auto distances = BellmanFord(graph, std::string("A"));
  std::unordered_map<std::string, int> expected = {
      {"A", 0}, {"B", 3}, {"C", 2}, {"D", 7}};
  ASSERT_EQ(distances, expected);
}

TEST(BellmanFordTest, DisconnectedGraph) {
  Graph<int, int> graph =
      Graph<int, int>::GraphWeighted({{0, 1, 1}, {2, 3, 1}});

  auto distances = BellmanFord(graph, 0);
  std::unordered_map<int, int> expected = {
      {0, 0},
      {1, 1},
      {2, std::numeric_limits<int>::max()},
      {3, std::numeric_limits<int>::max()}};

  ASSERT_EQ(distances, expected);

  distances = BellmanFord(graph, 2);

  expected = {{0, std::numeric_limits<int>::max()},
              {1, std::numeric_limits<int>::max()},
              {2, 0},
              {3, 1}};
  ASSERT_EQ(distances, expected);
}

TEST(BellmanFordTest, NegativeWeights) {
  Graph<int, int> graph = Graph<int, int>::GraphWeighted({{0, 1, -1},
                                                          {0, 2, 4},
                                                          {1, 2, 3},
                                                          {1, 3, 2},
                                                          {1, 4, 2},
                                                          {3, 1, 1},
                                                          {4, 3, -3}});

  auto distances = BellmanFord(graph, 0);
  std::unordered_map<int, int> expected = {
      {0, 0}, {1, -1}, {2, 2}, {3, -2}, {4, 1}};

  ASSERT_EQ(distances, expected);
}

TEST(BellmanFordTest, NegativeCycle) {
  Graph<int, int> graph =
      Graph<int, int>::GraphWeighted({{0, 1, 1}, {1, 2, -3}, {2, 0, -1}});

  ASSERT_THROW(BellmanFord(graph, 0), std::runtime_error);
}

TEST(BellmanFordTest, EmptyGraph) {
  Graph<int, int> graph;
  graph.AddVert(0);

  auto distances = BellmanFord(graph, 0);

  std::unordered_map<int, int> expected = {{0, 0}};
  ASSERT_EQ(distances, expected);
}

TEST(BellmanFordTest, NonExistentStartVert) {
  Graph<int, int> graph = Graph<int, int>::GraphWeighted({{0, 1, 1}});
  ASSERT_THROW(BellmanFord(graph, 2), std::invalid_argument);
}

TEST(BellmanFordTest, SimpleDAG_A) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = BellmanFord(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 5}, {"C", 3}, {"D", 7}, {"E", 4}, {"F", 6}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, SimpleDAG_B) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = BellmanFord(graph, std::string("B"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 9}, {"E", LONG_INF}, {"B", 0},
      {"D", 2}, {"C", LONG_INF}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, SimpleDAG_C) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = BellmanFord(graph, std::string("C"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 3}, {"E", 1}, {"B", LONG_INF}, {"D", 4}, {"C", 0}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, SimpleDAG_D) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = BellmanFord(graph, std::string("D"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 7}, {"E", LONG_INF}, {"B", LONG_INF},
      {"D", 0}, {"C", LONG_INF}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, SimpleDAG_E) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = BellmanFord(graph, std::string("E"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 2},        {"E", 0},        {"B", LONG_INF},
      {"D", LONG_INF}, {"C", LONG_INF}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, SimpleDAG_F) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = BellmanFord(graph, std::string("F"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 0},        {"E", LONG_INF}, {"B", LONG_INF},
      {"D", LONG_INF}, {"C", LONG_INF}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, UnweightedDAG) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B"});
  graph.AddEdge({"A", "C"});
  graph.AddEdge({"B", "D"});
  graph.AddEdge({"C", "D"});
  graph.AddEdge({"C", "E"});
  graph.AddEdge({"D", "F"});
  graph.AddEdge({"E", "F"});

  auto distances = BellmanFord(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 1}, {"C", 1}, {"D", 2}, {"E", 2}, {"F", 3}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, NonDirectedGraph) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B"});
  graph.AddEdge({"A", "C"});
  graph.AddEdge({"B", "D"});
  graph.AddEdge({"C", "D"});

  graph.MakeUndirected();

  ASSERT_NO_THROW(BellmanFord(graph, std::string("A")));

  auto distances = BellmanFord(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"B", 1}, {"D", 2}, {"C", 1}, {"A", 0}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, NonExistingStartVertex) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B"});
  graph.AddEdge({"A", "C"});
  graph.AddEdge({"B", "D"});
  graph.AddEdge({"C", "D"});

  graph.MakeUndirected();

  ASSERT_THROW(BellmanFord(graph, std::string("E")), std::invalid_argument);
}

TEST(BellmanFordTest, DAGWithNoIncomingEdges) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  graph.AddEdge({"G", "F", 3});

  auto distances = BellmanFord(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 5}, {"C", 3},       {"D", 7},
      {"E", 4}, {"F", 6}, {"G", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, OneVertDAG) {
  Graph<std::string, long> graph;
  graph.AddVert("A");

  auto distances = BellmanFord(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {{"A", 0}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, TwoVertsDAG) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});

  auto distances = BellmanFord(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {{"A", 0},
                                                              {"B", 5}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, MultiplePathsToOneVertex) {
  Graph<std::string, long> graph;

  graph.AddEdge({"A", "B", 1});
  graph.AddEdge({"A", "C", 4});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 1});

  auto distances = BellmanFord(graph, std::string("A"));
  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 1}, {"C", 4}, {"D", 3}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, IntVerticesAndWeights) {
  Graph<int, long> graph;

  graph.AddEdge({1, 2, 5});
  graph.AddEdge({1, 3, 2});
  graph.AddEdge({2, 3, 1});

  auto distances = BellmanFord(graph, 1);
  std::unordered_map<int, long> expected_distances = {{1, 0}, {2, 5}, {3, 2}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, CharVerticesAndDoubleWeights) {
  Graph<std::string, double> graph;

  graph.AddEdge({"A", "B", 3.5});
  graph.AddEdge({"A", "C", 1.5});
  graph.AddEdge({"B", "C", 2.5});

  auto distances = BellmanFord(graph, std::string("A"));

  std::unordered_map<std::string, double> expected_distances = {
      {"A", 0.0}, {"B", 3.5}, {"C", 1.5}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, StringVerticesAndLongWeights) {
  Graph<std::string, long> graph;

  graph.AddEdge({"A", "B", 10});
  graph.AddEdge({"A", "C", 5});
  graph.AddEdge({"B", "C", 2});

  auto distances = BellmanFord(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 10}, {"C", 5}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(BellmanFordTest, SingleVertexGraph) {
  Graph<int, int> graph;
  graph.AddVert(1);
  auto distances = BellmanFord(graph, 1);
  EXPECT_EQ(distances.size(), 1);
  EXPECT_EQ(distances[1], 0);
}

TEST(BellmanFordTest, SimpleUnweightedGraph) {
  Graph<int, int> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddEdge({1, 2});
  graph.AddEdge({2, 3});
  auto distances = BellmanFord(graph, 1);
  EXPECT_EQ(distances.size(), 3);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 1);
  EXPECT_EQ(distances[3], 2);
}

TEST(BellmanFordTest, SimpleWeightedGraph) {
  Graph<int, int> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddEdge({1, 2, 4});
  graph.AddEdge({2, 3, 2});
  auto distances = BellmanFord(graph, 1);
  EXPECT_EQ(distances.size(), 3);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 4);
  EXPECT_EQ(distances[3], 6);
}

TEST(BellmanFordTest, GraphWithDisconnectedVertices) {
  Graph<int, int> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddEdge({1, 2, 4});

  auto distances = BellmanFord(graph, 1);
  EXPECT_EQ(distances.size(), 3);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 4);
  EXPECT_EQ(distances[3], std::numeric_limits<int>::max());
}

TEST(BellmanFordTest, NegativeWeightException) {
  Graph<int, int> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddEdge({1, 2, -1});
  EXPECT_NO_THROW(BellmanFord(graph, 1));
}

TEST(BellmanFordTest, StartVertexNotFoundException) {
  Graph<int, int> graph;
  graph.AddVert(1);
  EXPECT_THROW(BellmanFord(graph, 2), std::invalid_argument);
}

TEST(BellmanFordTest, MoreComplexGraph) {
  Graph<int, int> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddVert(4);
  graph.AddVert(5);
  graph.AddEdge({1, 2, 4});
  graph.AddEdge({1, 3, 2});
  graph.AddEdge({2, 3, 1});
  graph.AddEdge({3, 4, 5});
  graph.AddEdge({4, 5, 3});

  auto distances = BellmanFord(graph, 1);
  EXPECT_EQ(distances.size(), 5);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 4);
  EXPECT_EQ(distances[3], 2);
  EXPECT_EQ(distances[4], 7);
  EXPECT_EQ(distances[5], 10);
}

TEST(BellmanFordTest, GraphWithCycle) {
  Graph<int, int> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddEdge({1, 2, 2});
  graph.AddEdge({2, 3, 3});
  graph.AddEdge({3, 1, 1});
  auto distances = BellmanFord(graph, 1);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 2);
  EXPECT_EQ(distances[3], 5);
}

TEST(BellmanFordTest, MultipleShortestPaths) {
  Graph<int, int> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddEdge({1, 2, 4});
  graph.AddEdge({1, 3, 4});
  auto distances = BellmanFord(graph, 1);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 4);
  EXPECT_EQ(distances[3], 4);
}

TEST(BellmanFordTest, LargerGraph) {
  Graph<int, int> graph;

  graph.AddEdge({1, 2, 2});
  graph.AddEdge({1, 3, 5});
  graph.AddEdge({2, 4, 4});
  graph.AddEdge({2, 5, 1});
  graph.AddEdge({3, 6, 3});
  graph.AddEdge({3, 7, 2});
  graph.AddEdge({4, 8, 6});
  graph.AddEdge({5, 9, 3});
  graph.AddEdge({7, 10, 4});
  graph.AddEdge({10, 9, 1});
  graph.AddEdge({8, 6, 2});
  graph.AddEdge({9, 4, 9});

  graph.MakeUndirected();

  auto distances = BellmanFord(graph, 1);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 2);
  EXPECT_EQ(distances[9], 6);
  EXPECT_EQ(distances[10], 7);
}

TEST(BellmanFordTest, FloatingPointWeights) {
  Graph<int, float> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddEdge({1, 2, 2.5f});
  auto distances = BellmanFord(graph, 1);
  EXPECT_FLOAT_EQ(distances[1], 0.0f);
  EXPECT_FLOAT_EQ(distances[2], 2.5f);
}

TEST(JohnsonTest, Simple_Test_1) {
  Graph<char, int> graph;

  auto result = Johnson(graph);
  std::unordered_map<char, std::unordered_map<char, int>> expected;

  ASSERT_EQ(result, expected);
}

TEST(JohnsonTest, Simple_Test_2) {
  Graph<char, int> graph;

  graph.AddVert('A');

  auto result = Johnson(graph);
  std::unordered_map<char, std::unordered_map<char, int>> expected;

  expected['A']['A'] = 0;

  ASSERT_EQ(result, expected);
}

TEST(JohnsonTest, Simple_Test_3) {
  Graph<char, int> graph;

  graph.AddVert('A');
  graph.AddVert('B');

  auto result = Johnson(graph);
  std::unordered_map<char, std::unordered_map<char, int>> expected;

  expected['A']['A'] = 0;
  expected['A']['B'] = INF;
  expected['B']['A'] = INF;
  expected['B']['B'] = 0;

  ASSERT_EQ(result, expected);
}

TEST(JohnsonTest, Simple_Test_4) {
  Graph<char, int> graph;

  graph.AddVert('A');
  graph.AddVert('B');

  graph.AddEdge({'A', 'B', -1});
  graph.AddEdge({'B', 'A', 2});

  auto result = Johnson(graph);
  std::unordered_map<char, std::unordered_map<char, int>> expected;

  expected['A']['A'] = 0;
  expected['A']['B'] = -1;
  expected['B']['A'] = 2;
  expected['B']['B'] = 0;

  ASSERT_EQ(result, expected);
}

TEST(JohnsonTest, Simple_Test_5) {
  Graph<char, int> graph;

  graph.AddVert('A');
  graph.AddVert('B');

  graph.AddEdge({'A', 'B', -2});
  graph.AddEdge({'B', 'A', 1});

  ASSERT_THROW(Johnson(graph), std::runtime_error);
}

TEST(JohnsonTest, Test_1) {
  Graph<int, int> graph;

  graph.AddVert(0);
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);

  graph.AddEdge({0, 1, 1});
  graph.AddEdge({1, 2, 2});
  graph.AddEdge({2, 3, 1});
  graph.AddEdge({0, 2, 4});

  auto result = Johnson(graph);
  std::unordered_map<int, std::unordered_map<int, int>> expected;

  expected[0][0] = 0;
  expected[0][1] = 1;
  expected[0][2] = 3;
  expected[0][3] = 4;

  expected[1][0] = INF;
  expected[1][1] = 0;
  expected[1][2] = 2;
  expected[1][3] = 3;

  expected[2][0] = INF;
  expected[2][1] = INF;
  expected[2][2] = 0;
  expected[2][3] = 1;

  expected[3][0] = INF;
  expected[3][1] = INF;
  expected[3][2] = INF;
  expected[3][3] = 0;

  ASSERT_EQ(result, expected);
}

TEST(JohnsonTest, Test_2) {
  Graph<char, int> graph;

  graph.AddVert('A');
  graph.AddVert('B');
  graph.AddVert('C');
  graph.AddVert('D');

  graph.AddEdge({'A', 'B', -2});
  graph.AddEdge({'A', 'D', 5});
  graph.AddEdge({'A', 'C', 7});
  graph.AddEdge({'B', 'C', 8});
  graph.AddEdge({'B', 'D', 6});
  graph.AddEdge({'C', 'B', 3});
  graph.AddEdge({'C', 'D', -4});
  graph.AddEdge({'D', 'A', -1});

  auto result = Johnson(graph);
  std::unordered_map<char, std::unordered_map<char, int>> expected;

  expected['A']['A'] = 0;
  expected['A']['B'] = -2;
  expected['A']['C'] = 6;
  expected['A']['D'] = 2;

  expected['B']['A'] = 3;
  expected['B']['B'] = 0;
  expected['B']['C'] = 8;
  expected['B']['D'] = 4;

  expected['C']['A'] = -5;
  expected['C']['B'] = -7;
  expected['C']['C'] = 0;
  expected['C']['D'] = -4;

  expected['D']['A'] = -1;
  expected['D']['B'] = -3;
  expected['D']['C'] = 5;
  expected['D']['D'] = 0;

  ASSERT_EQ(result, expected);
}

TEST(JohnsonTest, Test_3) {
  Graph<int, int> graph;

  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddVert(4);
  graph.AddVert(5);

  graph.AddEdge({1, 2, -4});
  graph.AddEdge({2, 3, 5});
  graph.AddEdge({3, 1, 2});
  graph.AddEdge({1, 4, 1});
  graph.AddEdge({1, 5, -2});
  graph.AddEdge({4, 5, 3});

  auto result = Johnson(graph);
  std::unordered_map<int, std::unordered_map<int, int>> expected;

  expected[1][1] = 0;
  expected[1][2] = -4;
  expected[1][3] = 1;
  expected[1][4] = 1;
  expected[1][5] = -2;

  expected[2][1] = 7;
  expected[2][2] = 0;
  expected[2][3] = 5;
  expected[2][4] = 8;
  expected[2][5] = 5;

  expected[3][1] = 2;
  expected[3][2] = -2;
  expected[3][3] = 0;
  expected[3][4] = 3;
  expected[3][5] = 0;

  expected[4][1] = INF;
  expected[4][2] = INF;
  expected[4][3] = INF;
  expected[4][4] = 0;
  expected[4][5] = 3;

  expected[5][1] = INF;
  expected[5][2] = INF;
  expected[5][3] = INF;
  expected[5][4] = INF;
  expected[5][5] = 0;

  ASSERT_EQ(result, expected);
}

TEST(JohnsonTest, Test_4) {
  Graph<char, int> graph;

  graph.AddVert('A');
  graph.AddVert('B');
  graph.AddVert('C');
  graph.AddVert('D');
  graph.AddVert('E');
  graph.AddVert('F');
  graph.AddVert('G');

  graph.AddEdge({'A', 'B', 5});
  graph.AddEdge({'A', 'C', 2});
  graph.AddEdge({'B', 'C', 1});
  graph.AddEdge({'B', 'D', 3});
  graph.AddEdge({'C', 'E', -2});
  graph.AddEdge({'C', 'F', 4});
  graph.AddEdge({'D', 'E', 2});
  graph.AddEdge({'D', 'G', 6});
  graph.AddEdge({'E', 'F', 3});
  graph.AddEdge({'E', 'G', -4});
  graph.AddEdge({'F', 'G', 1});
  graph.AddEdge({'F', 'C', -7});
  graph.AddEdge({'G', 'A', 8});

  ASSERT_THROW(Johnson(graph), std::runtime_error);
}

TEST(JohnsonTest, Test_5) {
  Graph<std::string, int> graph;

  graph.AddVert("A");
  graph.AddVert("B");
  graph.AddVert("C");
  graph.AddVert("D");
  graph.AddVert("E");
  graph.AddVert("F");

  graph.AddEdge({"A", "B", 2});
  graph.AddEdge({"A", "C", 1});
  graph.AddEdge({"A", "F", 8});
  graph.AddEdge({"B", "D", 4});
  graph.AddEdge({"B", "E", -3});
  graph.AddEdge({"C", "B", 5});
  graph.AddEdge({"C", "E", 5});
  graph.AddEdge({"C", "F", 6});
  graph.AddEdge({"D", "A", -1});
  graph.AddEdge({"D", "F", -3});
  graph.AddEdge({"E", "C", -2});
  graph.AddEdge({"E", "F", -2});
  graph.AddEdge({"E", "D", 3});

  auto result = Johnson(graph);
  std::unordered_map<std::string, std::unordered_map<std::string, int>>
      expected;

  expected["A"]["A"] = 0;
  expected["A"]["B"] = 2;
  expected["A"]["C"] = -3;
  expected["A"]["D"] = 2;
  expected["A"]["E"] = -1;
  expected["A"]["F"] = -3;

  expected["B"]["A"] = -1;
  expected["B"]["B"] = 0;
  expected["B"]["C"] = -5;
  expected["B"]["D"] = 0;
  expected["B"]["E"] = -3;
  expected["B"]["F"] = -5;

  expected["C"]["A"] = 4;
  expected["C"]["B"] = 5;
  expected["C"]["C"] = 0;
  expected["C"]["D"] = 5;
  expected["C"]["E"] = 2;
  expected["C"]["F"] = 0;

  expected["D"]["A"] = -1;
  expected["D"]["B"] = 1;
  expected["D"]["C"] = -4;
  expected["D"]["D"] = 0;
  expected["D"]["E"] = -2;
  expected["D"]["F"] = -4;

  expected["E"]["A"] = 2;
  expected["E"]["B"] = 3;
  expected["E"]["C"] = -2;
  expected["E"]["D"] = 3;
  expected["E"]["E"] = 0;
  expected["E"]["F"] = -2;

  expected["F"]["A"] = INF;
  expected["F"]["B"] = INF;
  expected["F"]["C"] = INF;
  expected["F"]["D"] = INF;
  expected["F"]["E"] = INF;
  expected["F"]["F"] = 0;

  ASSERT_EQ(result, expected);
}

TEST(JohnsonTest, SimpleGraph) {
  Graph<int, int> graph;

  graph.AddEdge({0, 1, 1});
  graph.AddEdge({1, 2, 2});
  graph.AddEdge({2, 3, 1});
  graph.AddEdge({0, 2, 4});

  std::vector<std::vector<int>> expected_vec = {
      {0, 1, 3, 4}, {INF, 0, 2, 3}, {INF, INF, 0, 1}, {INF, INF, INF, 0}};

  auto result = Johnson(graph);
  EXPECT_EQ(result, ConvertVectorVectorToUnorderedMap(expected_vec));
}

TEST(JohnsonTest, GraphWithNegativeWeights) {
  Graph<int, int> graph;

  graph.AddEdge({0, 1, -2});
  graph.AddEdge({1, 2, -3});
  graph.AddEdge({2, 3, 1});
  graph.AddEdge({0, 2, 4});

  std::vector<std::vector<int>> expected_vec = {
      {0, -2, -5, -4}, {INF, 0, -3, -2}, {INF, INF, 0, 1}, {INF, INF, INF, 0}};

  auto result = Johnson(graph);
  EXPECT_EQ(result, ConvertVectorVectorToUnorderedMap(expected_vec));
}

TEST(JohnsonTest, NegativeCycle) {
  Graph<int, int> graph;

  graph.AddEdge({0, 1, 1});
  graph.AddEdge({1, 2, 2});
  graph.AddEdge({2, 0, -4});
  graph.AddEdge({1, 3, 5});

  EXPECT_THROW(Johnson(graph), std::runtime_error);
}

TEST(JohnsonTest, DisconnectedGraph) {
  Graph<int, int> graph;

  graph.AddEdge({0, 1, 1});
  graph.AddEdge({1, 2, 2});

  graph.AddVert(3);

  std::vector<std::vector<int>> expected_vec = {
      {0, 1, 3, INF}, {INF, 0, 2, INF}, {INF, INF, 0, INF}, {INF, INF, INF, 0}};

  auto result = Johnson(graph);
  EXPECT_EQ(result, ConvertVectorVectorToUnorderedMap(expected_vec));
}

TEST(JohnsonTest, EmptyGraph) {
  Graph<int, int> graph;
  // No edges or vertices

  auto result = Johnson(graph);
  EXPECT_TRUE(result.empty());
}