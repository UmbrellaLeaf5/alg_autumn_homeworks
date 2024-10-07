#include <gtest/gtest.h>

#include "tarjan_algorithm.hpp"

TEST(SCCSTA_Test, Simple) {
  std::unordered_map<std::string, std::vector<std::string>> adj_list_dict = {
      {"A", {"B"}}, {"B", {"C", "D"}}, {"C", {"A", "D"}}, {"D", {"E"}},
      {"E", {"D"}}, {"F", {"E", "G"}}, {"G", {"F", "H"}}, {"H", {"E", "G"}}};

  auto graph = Graph<std::string, long>::GraphFromAdjList(adj_list_dict);

  std::vector<std::vector<std::string>> answer = {
      {"D", "E"}, {"H", "G", "F"}, {"A", "C", "B"}};

  ASSERT_EQ(StronglyConnectedComponents(graph), answer);
}

TEST(SCCSTA_Test, EmptyGraph) {
  Graph<int, long> graph;
  auto components = StronglyConnectedComponents(graph);

  ASSERT_EQ(components.size(), 0);
}

TEST(SCCSTA_Test, SingleVertex) {
  Graph<int, long> graph;
  graph.AddVert(1);

  auto components = StronglyConnectedComponents(graph);

  ASSERT_EQ(components.size(), 1);

  ASSERT_EQ(components[0], std::vector<int>({1}));
}

TEST(SCCSTA_Test, TwoVerticesNoEdges) {
  Graph<int, long> graph;
  graph.AddVert(1);
  graph.AddVert(2);

  auto components = StronglyConnectedComponents(graph);
  ASSERT_EQ(components.size(), 2);

  ASSERT_EQ(components[0], std::vector<int>({1}));
  ASSERT_EQ(components[1], std::vector<int>({2}));
}

TEST(SCCSTA_Test, SimpleCycle) {
  Graph<int, long> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddEdge(1, 2);
  graph.AddEdge(2, 1);
  auto components = StronglyConnectedComponents(graph);
  ASSERT_EQ(components.size(), 1);
  ASSERT_EQ(components[0], std::vector<int>({1, 2}));
}

TEST(SCCSTA_Test, MultipleCycles) {
  Graph<int, long> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddVert(4);
  graph.AddEdge(1, 2);
  graph.AddEdge(2, 1);
  graph.AddEdge(3, 4);
  graph.AddEdge(4, 3);

  auto components = StronglyConnectedComponents(graph);

  ASSERT_EQ(components.size(), 2);

  std::sort(components[0].begin(), components[0].end());
  std::sort(components[1].begin(), components[1].end());

  ASSERT_EQ(components[0], std::vector<int>({1, 2}));
  ASSERT_EQ(components[1], std::vector<int>({3, 4}));
}

TEST(SCCSTA_Test, ComplexGraph) {
  Graph<int, long> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddVert(4);
  graph.AddVert(5);
  graph.AddVert(6);
  graph.AddEdge(1, 2);
  graph.AddEdge(2, 3);
  graph.AddEdge(3, 1);
  graph.AddEdge(4, 5);
  graph.AddEdge(5, 6);
  graph.AddEdge(6, 4);
  graph.AddEdge(2, 4);

  auto components = StronglyConnectedComponents(graph);

  ASSERT_EQ(components.size(), 2);

  std::sort(components.begin(), components.end());

  std::sort(components[0].begin(), components[0].end());
  std::sort(components[1].begin(), components[1].end());

  ASSERT_EQ(components[0], std::vector<int>({1, 2, 3}));
  ASSERT_EQ(components[1], std::vector<int>({4, 5, 6}));
}

TEST(SCCSTA_Test, UndirectedGraph) {
  Graph<int, long> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddEdge(1, 2);

  graph.MakeUndirected();

  ASSERT_THROW(StronglyConnectedComponents(graph), std::invalid_argument);
}

TEST(SCCSTA_Test, SelfLoop) {
  Graph<int, long> graph;
  graph.AddVert(1);
  graph.AddEdge(1, 1);

  auto components = StronglyConnectedComponents(graph);

  ASSERT_EQ(components.size(), 2);

  ASSERT_EQ(components[0], std::vector<int>({1}));
  ASSERT_EQ(components[1], std::vector<int>({1}));
}

TEST(SCCSTA_Test, MultipleSelfLoops) {
  Graph<int, long> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddEdge(1, 1);
  graph.AddEdge(2, 2);

  auto components = StronglyConnectedComponents(graph);

  ASSERT_EQ(components.size(), 3);

  ASSERT_EQ(components[0], std::vector<int>({1}));
  ASSERT_EQ(components[2], std::vector<int>({2}));
}

TEST(SCCSTA_Test, ConnectedComponents) {
  Graph<int, long> graph;
  graph.AddVert(1);
  graph.AddVert(2);
  graph.AddVert(3);
  graph.AddVert(4);
  graph.AddEdge(1, 2);
  graph.AddEdge(2, 1);
  graph.AddEdge(3, 4);
  graph.AddEdge(4, 3);
  graph.AddEdge(1, 3);

  auto components = StronglyConnectedComponents(graph);

  ASSERT_EQ(components.size(), 2);

  std::sort(components.begin(), components.end());

  std::sort(components[0].begin(), components[0].end());
  std::sort(components[1].begin(), components[1].end());

  ASSERT_EQ(components[0], std::vector<int>({1, 2}));
  ASSERT_EQ(components[1], std::vector<int>({3, 4}));
}
