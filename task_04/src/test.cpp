#include <gtest/gtest.h>

#include "dijkstra.hpp"

#define LONG_INF std::numeric_limits<long>::max()

TEST(DijkstraTest, SimpleDAG_A) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = Dijkstra(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 5}, {"C", 3}, {"D", 7}, {"E", 4}, {"F", 6}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, SimpleDAG_B) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = Dijkstra(graph, std::string("B"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 9}, {"E", LONG_INF}, {"B", 0},
      {"D", 2}, {"C", LONG_INF}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, SimpleDAG_C) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = Dijkstra(graph, std::string("C"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 3}, {"E", 1}, {"B", LONG_INF}, {"D", 4}, {"C", 0}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, SimpleDAG_D) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = Dijkstra(graph, std::string("D"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 7}, {"E", LONG_INF}, {"B", LONG_INF},
      {"D", 0}, {"C", LONG_INF}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, SimpleDAG_E) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = Dijkstra(graph, std::string("E"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 2},        {"E", 0},        {"B", LONG_INF},
      {"D", LONG_INF}, {"C", LONG_INF}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, SimpleDAG_F) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  auto distances = Dijkstra(graph, std::string("F"));

  std::unordered_map<std::string, long> expected_distances = {
      {"F", 0},        {"E", LONG_INF}, {"B", LONG_INF},
      {"D", LONG_INF}, {"C", LONG_INF}, {"A", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, UnweightedDAG) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B"});
  graph.AddEdge({"A", "C"});
  graph.AddEdge({"B", "D"});
  graph.AddEdge({"C", "D"});
  graph.AddEdge({"C", "E"});
  graph.AddEdge({"D", "F"});
  graph.AddEdge({"E", "F"});

  auto distances = Dijkstra(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 1}, {"C", 1}, {"D", 2}, {"E", 2}, {"F", 3}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, NonDirectedGraph) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B"});
  graph.AddEdge({"A", "C"});
  graph.AddEdge({"B", "D"});
  graph.AddEdge({"C", "D"});

  graph.MakeUndirected();

  ASSERT_NO_THROW(Dijkstra(graph, std::string("A")));

  auto distances = Dijkstra(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"B", 1}, {"D", 2}, {"C", 1}, {"A", 0}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, NonExistingStartVertex) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B"});
  graph.AddEdge({"A", "C"});
  graph.AddEdge({"B", "D"});
  graph.AddEdge({"C", "D"});

  graph.MakeUndirected();

  ASSERT_THROW(Dijkstra(graph, std::string("E")), std::invalid_argument);
}

TEST(DijkstraTest, DAGWithNoIncomingEdges) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});
  graph.AddEdge({"A", "C", 3});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 4});
  graph.AddEdge({"C", "E", 1});
  graph.AddEdge({"D", "F", 7});
  graph.AddEdge({"E", "F", 2});

  graph.AddEdge({"G", "F", 3});

  auto distances = Dijkstra(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 5}, {"C", 3},       {"D", 7},
      {"E", 4}, {"F", 6}, {"G", LONG_INF}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, OneVertDAG) {
  Graph<std::string, long> graph;
  graph.AddVert("A");

  auto distances = Dijkstra(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {{"A", 0}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, TwoVertsDAG) {
  Graph<std::string, long> graph;
  graph.AddEdge({"A", "B", 5});

  auto distances = Dijkstra(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {{"A", 0},
                                                              {"B", 5}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, MultiplePathsToOneVertex) {
  Graph<std::string, long> graph;

  graph.AddEdge({"A", "B", 1});
  graph.AddEdge({"A", "C", 4});
  graph.AddEdge({"B", "D", 2});
  graph.AddEdge({"C", "D", 1});

  auto distances = Dijkstra(graph, std::string("A"));
  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 1}, {"C", 4}, {"D", 3}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, IntVerticesAndWeights) {
  Graph<int, long> graph;

  graph.AddEdge({1, 2, 5});
  graph.AddEdge({1, 3, 2});
  graph.AddEdge({2, 3, 1});

  auto distances = Dijkstra(graph, 1);
  std::unordered_map<int, long> expected_distances = {{1, 0}, {2, 5}, {3, 2}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, CharVerticesAndDoubleWeights) {
  Graph<std::string, double> graph;

  graph.AddEdge({"A", "B", 3.5});
  graph.AddEdge({"A", "C", 1.5});
  graph.AddEdge({"B", "C", 2.5});

  auto distances = Dijkstra(graph, std::string("A"));

  std::unordered_map<std::string, double> expected_distances = {
      {"A", 0.0}, {"B", 3.5}, {"C", 1.5}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, StringVerticesAndLongWeights) {
  Graph<std::string, long> graph;

  graph.AddEdge({"A", "B", 10});
  graph.AddEdge({"A", "C", 5});
  graph.AddEdge({"B", "C", 2});

  auto distances = Dijkstra(graph, std::string("A"));

  std::unordered_map<std::string, long> expected_distances = {
      {"A", 0}, {"B", 10}, {"C", 5}};

  ASSERT_EQ(distances, expected_distances);
}

TEST(DijkstraTest, EmptyGraph) {
  Graph<int, int> graph;
  ASSERT_THROW(Dijkstra(graph, 1), std::invalid_argument);
}

TEST(DijkstraTest, SingleVertexGraph) {
  Graph<int, int> graph;
  graph.AddVert(1);
  auto distances = Dijkstra(graph, 1);
  EXPECT_EQ(distances.size(), 1);
  EXPECT_EQ(distances[1], 0);
}

TEST(DijkstraTest, SimpleUnweightedGraph) {
  Graph<int, int> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddEdge({1, 2});
  graph.AddEdge({2, 3});
  auto distances = Dijkstra(graph, 1);
  EXPECT_EQ(distances.size(), 3);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 1);
  EXPECT_EQ(distances[3], 2);
}

TEST(DijkstraTest, SimpleWeightedGraph) {
  Graph<int, int> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddEdge({1, 2, 4});
  graph.AddEdge({2, 3, 2});
  auto distances = Dijkstra(graph, 1);
  EXPECT_EQ(distances.size(), 3);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 4);
  EXPECT_EQ(distances[3], 6);
}

TEST(DijkstraTest, GraphWithDisconnectedVertices) {
  Graph<int, int> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddEdge({1, 2, 4});

  auto distances = Dijkstra(graph, 1);
  EXPECT_EQ(distances.size(), 3);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 4);
  EXPECT_EQ(distances[3], std::numeric_limits<int>::max());
}

TEST(DijkstraTest, NegativeWeightException) {
  Graph<int, int> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddEdge({1, 2, -1});
  EXPECT_THROW(Dijkstra(graph, 1), std::logic_error);
}

TEST(DijkstraTest, StartVertexNotFoundException) {
  Graph<int, int> graph;
  graph.AddVert(1);
  EXPECT_THROW(Dijkstra(graph, 2), std::invalid_argument);
}

TEST(DijkstraTest, MoreComplexGraph) {
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

  auto distances = Dijkstra(graph, 1);
  EXPECT_EQ(distances.size(), 5);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 4);
  EXPECT_EQ(distances[3], 2);
  EXPECT_EQ(distances[4], 7);
  EXPECT_EQ(distances[5], 10);
}

TEST(DijkstraTest, GraphWithCycle) {
  Graph<int, int> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddEdge({1, 2, 2});
  graph.AddEdge({2, 3, 3});
  graph.AddEdge({3, 1, 1});
  auto distances = Dijkstra(graph, 1);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 2);
  EXPECT_EQ(distances[3], 5);
}

TEST(DijkstraTest, MultipleShortestPaths) {
  Graph<int, int> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddEdge({1, 2, 4});
  graph.AddEdge({1, 3, 4});
  auto distances = Dijkstra(graph, 1);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 4);
  EXPECT_EQ(distances[3], 4);
}

TEST(DijkstraTest, LargerGraph) {
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

  auto distances = Dijkstra(graph, 1);
  EXPECT_EQ(distances[1], 0);
  EXPECT_EQ(distances[2], 2);
  EXPECT_EQ(distances[9], 6);
  EXPECT_EQ(distances[10], 7);
}

TEST(DijkstraTest, FloatingPointWeights) {
  Graph<int, float> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddEdge({1, 2, 2.5f});
  auto distances = Dijkstra(graph, 1);
  EXPECT_FLOAT_EQ(distances[1], 0.0f);
  EXPECT_FLOAT_EQ(distances[2], 2.5f);
}
