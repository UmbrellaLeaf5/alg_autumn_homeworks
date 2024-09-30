#include <gtest/gtest.h>

#include "topological_sort.hpp"

TEST(TSTest, TopologicalSort) {
  Graph<std::string, long> graph;
  graph.AddVert("A");
  graph.AddVert("B");
  graph.AddVert("C");
  graph.AddVert("D");
  graph.AddVert("E");
  graph.AddVert("F");

  graph.AddEdge("A", "B", 1);
  graph.AddEdge("A", "C", 2);
  graph.AddEdge("B", "D", 3);
  graph.AddEdge("C", "E", 4);
  graph.AddEdge("D", "F", 5);
  graph.AddEdge("E", "F", 6);

  std::vector<std::string> expected_order = {"A", "C", "E", "B", "D", "F"};

  std::vector<std::string> actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}

TEST(TSTest, TopologicalSort_Empty) {
  Graph<std::string, long> graph;

  std::vector<std::string> expected_order = {};

  std::vector<std::string> actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}

TEST(TSTest, TopologicalSort_SingleVertex) {
  Graph<std::string, long> graph;
  graph.AddVert("A");

  std::vector<std::string> expected_order = {"A"};

  std::vector<std::string> actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}

TEST(TSTest, TopologicalSort_MultipleSources) {
  Graph<std::string, long> graph;
  graph.AddVert("A");
  graph.AddVert("B");
  graph.AddVert("C");
  graph.AddVert("D");
  graph.AddVert("E");

  graph.AddEdge("A", "C", 1);
  graph.AddEdge("B", "C", 2);
  graph.AddEdge("C", "D", 3);
  graph.AddEdge("C", "E", 4);

  std::vector<std::string> expected_order = {"B", "A", "C", "E", "D"};

  std::vector<std::string> actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}

TEST(TSTest, TopologicalSort_Disconnected) {
  Graph<std::string, long> graph;
  graph.AddVert("A");
  graph.AddVert("B");
  graph.AddVert("C");
  graph.AddVert("D");
  graph.AddVert("E");

  graph.AddEdge("A", "B", 1);
  graph.AddEdge("C", "D", 2);

  std::vector<std::string> expected_order = {"E", "C", "D", "A", "B"};

  std::vector<std::string> actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}

TEST(TSTest, TopologicalSort_LinearChain) {
  Graph<std::string, long> graph;
  graph.AddVert("A");
  graph.AddVert("B");
  graph.AddVert("C");
  graph.AddVert("D");
  graph.AddVert("E");

  graph.AddEdge("A", "B", 1);
  graph.AddEdge("B", "C", 2);
  graph.AddEdge("C", "D", 3);
  graph.AddEdge("D", "E", 4);

  std::vector<std::string> expected_order = {"A", "B", "C", "D", "E"};

  std::vector<std::string> actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}

TEST(TSTest, TopologicalSort_Tree) {
  Graph<std::string, long> graph;
  graph.AddVert("A");
  graph.AddVert("B");
  graph.AddVert("C");
  graph.AddVert("D");
  graph.AddVert("E");
  graph.AddVert("F");
  graph.AddVert("G");

  graph.AddEdge("A", "B", 1);
  graph.AddEdge("A", "C", 2);
  graph.AddEdge("B", "D", 3);
  graph.AddEdge("B", "E", 4);
  graph.AddEdge("C", "F", 5);
  graph.AddEdge("C", "G", 6);

  std::vector<std::string> expected_order = {"A", "C", "G", "F", "B", "E", "D"};

  std::vector<std::string> actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}

TEST(TSTest, TopologicalSort_Branching) {
  Graph<std::string, long> graph;
  graph.AddVert("A");
  graph.AddVert("B");
  graph.AddVert("C");
  graph.AddVert("D");
  graph.AddVert("E");
  graph.AddVert("F");
  graph.AddVert("G");

  graph.AddEdge("A", "B", 1);
  graph.AddEdge("A", "C", 2);
  graph.AddEdge("B", "D", 3);
  graph.AddEdge("B", "E", 4);
  graph.AddEdge("C", "F", 5);
  graph.AddEdge("C", "G", 6);
  graph.AddEdge("D", "F", 7);  //  Добавляем "ветку"

  std::vector<std::string> expected_order = {"A", "C", "G", "B", "E", "D", "F"};

  std::vector<std::string> actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}
