
#include <gtest/gtest.h>

#include <stack>

#include "find_bridges.hpp"
#include "find_cut_vertices.hpp"

template <typename T>
bool operator==(const std::vector<T>& lhs, const std::vector<T>& rhs) {
  if (lhs.size() != rhs.size()) return false;

  for (size_t i = 0; i < lhs.size(); ++i)
    if (lhs[i] != rhs[i]) return false;

  return true;
}

template <typename T1, typename T2>
bool operator==(const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) {
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

TEST(FindBridgesTest, EmptyGraph) {
  Graph<size_t, long> graph;
  auto bridges = FindBridges(graph);
  ASSERT_EQ(bridges.size(), 0);
}

TEST(FindBridgesTest, SimpleGraphs) {
  Graph<size_t, long> graph;
  graph.AddEdge(0, 1);
  graph.AddEdge(1, 2);

  auto bridges = FindBridges(graph);
  std::vector<std::pair<size_t, size_t>> answer = {{0, 1}, {1, 2}};

  ASSERT_EQ(bridges, answer);

  graph.AddEdge(2, 3);

  bridges = FindBridges(graph);
  answer = {{0, 1}, {1, 2}, {2, 3}};

  ASSERT_EQ(bridges, answer);

  graph.MakeUndirected();
  ASSERT_EQ(bridges, answer);
}

TEST(FindBridgesTest, MultipleBridges) {
  Graph<size_t, long> graph;
  graph.AddEdge(0, 1);
  graph.AddEdge(0, 2);
  graph.AddEdge(1, 2);
  graph.AddEdge(2, 3);
  graph.AddEdge(3, 4);
  graph.AddEdge(4, 5);

  auto bridges = FindBridges(graph);
  std::vector<std::pair<size_t, size_t>> answer = {
      {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}};

  ASSERT_EQ(bridges, answer);
}

TEST(FindBridgesTest, ComplexGraph) {
  Graph<size_t, long> graph;
  graph.AddEdge(0, 1);
  graph.AddEdge(0, 2);
  graph.AddEdge(1, 2);
  graph.AddEdge(1, 3);
  graph.AddEdge(2, 3);
  graph.AddEdge(3, 4);
  graph.AddEdge(4, 5);
  graph.AddEdge(4, 6);
  graph.AddEdge(5, 6);
  graph.AddEdge(6, 7);
  graph.MakeUndirected();

  auto bridges = FindBridges(graph);
  std::vector<std::pair<size_t, size_t>> answer = {{3, 4}, {6, 7}};

  ASSERT_EQ(bridges, answer);
}

TEST(FindBridgesTest, NonSimpleGraphs) {
  std::unordered_map<std::string, std::vector<std::string>> adj_list_dict = {
      {"A", {"B"}}, {"B", {"C", "D"}}, {"C", {"A", "D"}}, {"D", {"E"}},
      {"E", {"D"}}, {"F", {"E", "G"}}, {"G", {"F", "H"}}, {"H", {"E", "G"}}};

  auto graph = Graph<std::string, long>::GraphFromAdjList(adj_list_dict);
  graph.MakeUndirected();

  auto bridges = FindBridges(graph);
  std::vector<std::pair<std::string, std::string>> answer = {{"E", "D"}};

  ASSERT_EQ(bridges, answer);
}

TEST(FindCutVerticesTest, EmptyGraph) {
  Graph<size_t, long> graph;
  auto cut_vertices = FindCutVertices(graph);
  ASSERT_EQ(cut_vertices.size(), 0);
}

TEST(FindCutVerticesTest, SimpleGraphs) {
  Graph<size_t, long> graph;
  graph.AddEdge(0, 1);
  graph.AddEdge(1, 2);

  auto cut_vertices = FindCutVertices(graph);
  std::vector<size_t> answer = {1};

  ASSERT_EQ(cut_vertices, answer);

  graph.AddEdge(2, 3);

  cut_vertices = FindCutVertices(graph);
  answer = {1, 2};

  ASSERT_EQ(cut_vertices, answer);

  graph.MakeUndirected();
  ASSERT_EQ(cut_vertices, answer);
}

TEST(FindCutVerticesTest, MultipleBridges) {
  Graph<size_t, long> graph;
  graph.AddEdge(0, 1);
  graph.AddEdge(0, 2);
  graph.AddEdge(1, 2);
  graph.AddEdge(2, 3);
  graph.AddEdge(3, 4);
  graph.AddEdge(4, 5);

  graph.MakeUndirected();

  auto cut_vertices = FindCutVertices(graph);
  std::vector<size_t> answer = {2, 3, 4};

  ASSERT_EQ(cut_vertices, answer);
}

TEST(FindCutVerticesTest, ComplexGraph) {
  Graph<size_t, long> graph;
  graph.AddEdge(0, 1);
  graph.AddEdge(0, 2);
  graph.AddEdge(1, 2);
  graph.AddEdge(1, 3);
  graph.AddEdge(2, 3);
  graph.AddEdge(3, 4);
  graph.AddEdge(4, 5);
  graph.AddEdge(4, 6);
  graph.AddEdge(5, 6);
  graph.AddEdge(6, 7);
  graph.MakeUndirected();

  auto cut_vertices = FindCutVertices(graph);
  std::vector<size_t> answer = {3, 4, 6};

  ASSERT_EQ(cut_vertices, answer);
}

TEST(FindCutVerticesTest, NonSimpleGraphs) {
  std::unordered_map<std::string, std::vector<std::string>> adj_list_dict = {
      {"A", {"B"}}, {"B", {"C", "D"}}, {"C", {"A", "D"}}, {"D", {"E"}},
      {"E", {"D"}}, {"F", {"E", "G"}}, {"G", {"F", "H"}}, {"H", {"E", "G"}}};

  auto graph = Graph<std::string, long>::GraphFromAdjList(adj_list_dict);
  graph.MakeUndirected();

  auto cut_vertices = FindCutVertices(graph);
  std::vector<std::string> answer = {"E", "D"};

  ASSERT_EQ(cut_vertices, answer);
}