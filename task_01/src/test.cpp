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

  graph.AddEdge({"A", "B", 1});
  graph.AddEdge({"A", "C", 2});
  graph.AddEdge({"B", "D", 3});
  graph.AddEdge({"C", "E", 4});
  graph.AddEdge({"D", "F", 5});
  graph.AddEdge({"E", "F", 6});

  std::vector<std::string> expected_order = {"A", "C", "E", "B", "D", "F"};

  auto actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}

TEST(TSTest, TopologicalSort_Empty) {
  Graph<std::string, long> graph;

  std::vector<std::string> expected_order = {};

  auto actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}

TEST(TSTest, TopologicalSort_SingleVertex) {
  Graph<std::string, long> graph;
  graph.AddVert("A");

  std::vector<std::string> expected_order = {"A"};

  auto actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}

TEST(TSTest, TopologicalSort_MultipleSources) {
  Graph<std::string, long> graph;
  graph.AddVert("A");
  graph.AddVert("B");
  graph.AddVert("C");
  graph.AddVert("D");
  graph.AddVert("E");

  graph.AddEdge({"A", "C", 1});
  graph.AddEdge({"B", "C", 2});
  graph.AddEdge({"C", "D", 3});
  graph.AddEdge({"C", "E", 4});

  std::vector<std::string> expected_order = {"B", "A", "C", "E", "D"};

  auto actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}

TEST(TSTest, TopologicalSort_Disconnected) {
  Graph<std::string, long> graph;
  graph.AddVert("A");
  graph.AddVert("B");
  graph.AddVert("C");
  graph.AddVert("D");
  graph.AddVert("E");

  graph.AddEdge({"A", "B", 1});
  graph.AddEdge({"C", "D", 2});

  std::vector<std::string> expected_order = {"E", "C", "D", "A", "B"};

  auto actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}

TEST(TSTest, TopologicalSort_LinearChain) {
  Graph<std::string, long> graph;
  graph.AddVert("A");
  graph.AddVert("B");
  graph.AddVert("C");
  graph.AddVert("D");
  graph.AddVert("E");

  graph.AddEdge({"A", "B", 1});
  graph.AddEdge({"B", "C", 2});
  graph.AddEdge({"C", "D", 3});
  graph.AddEdge({"D", "E", 4});

  std::vector<std::string> expected_order = {"A", "B", "C", "D", "E"};

  auto actual_order = TopologicalSort(graph);

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

  graph.AddEdge({"A", "B", 1});
  graph.AddEdge({"A", "C", 2});
  graph.AddEdge({"B", "D", 3});
  graph.AddEdge({"B", "E", 4});
  graph.AddEdge({"C", "F", 5});
  graph.AddEdge({"C", "G", 6});

  std::vector<std::string> expected_order = {"A", "C", "G", "F", "B", "E", "D"};

  auto actual_order = TopologicalSort(graph);

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

  graph.AddEdge({"A", "B", 1});
  graph.AddEdge({"A", "C", 2});
  graph.AddEdge({"B", "D", 3});
  graph.AddEdge({"B", "E", 4});
  graph.AddEdge({"C", "F", 5});
  graph.AddEdge({"C", "G", 6});
  graph.AddEdge({"D", "F", 7});  //  Добавляем "ветку"

  std::vector<std::string> expected_order = {"A", "C", "G", "B", "E", "D", "F"};

  auto actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);
}

TEST(TSTest, TopologicalSort_Main) {
  auto graph = Graph<size_t, long>::GraphWeighted({{0, 1, 1},
                                                   {0, 2, 2},
                                                   {1, 3, 3},
                                                   {1, 4, 4},
                                                   {2, 5, 5},
                                                   {3, 5, 6},
                                                   {4, 5, 7}});

  std::vector<size_t> expected_order = {0, 2, 1, 4, 3, 5};

  auto actual_order = TopologicalSort(graph);

  ASSERT_EQ(expected_order, actual_order);

  graph.MakeUndirected();

  EXPECT_THROW(TopologicalSort(graph), std::invalid_argument);
}

TEST(TSTest, TopologicalSort_Cyclic) {
  {
    Graph<int, long> graph;

    graph.AddVert(1);
    graph.AddVert(2);
    graph.AddVert(3);

    graph.AddEdge({1, 2});
    graph.AddEdge({2, 3});
    graph.AddEdge({3, 1});

    ASSERT_THROW(TopologicalSort(graph), std::invalid_argument);
  }

  {
    Graph<std::string, long> graph;

    graph.AddVert("G");
    graph.AddVert("D");
    graph.AddVert("B");
    graph.AddVert("C");
    graph.AddVert("F");
    graph.AddVert("E");
    graph.AddVert("H");
    graph.AddVert("A");

    graph.AddEdge({"E", "B"});
    graph.AddEdge({"A", "G"});
    graph.AddEdge({"G", "F"});
    graph.AddEdge({"D", "H"});
    graph.AddEdge({"H", "G"});
    graph.AddEdge({"B", "F"});
    graph.AddEdge({"G", "E"});
    graph.AddEdge({"F", "D"});

    ASSERT_THROW(TopologicalSort(graph), std::invalid_argument);
  }

  {
    Graph<size_t, long> graph;

    graph.AddVert(1);
    graph.AddVert(2);
    graph.AddVert(3);
    graph.AddVert(4);
    graph.AddVert(5);
    graph.AddVert(6);
    graph.AddVert(7);
    graph.AddVert(8);
    graph.AddVert(9);
    graph.AddVert(10);
    graph.AddVert(11);

    graph.AddEdge({1, 10});
    graph.AddEdge({10, 11});
    graph.AddEdge({5, 1});
    graph.AddEdge({5, 6});
    graph.AddEdge({4, 8});
    graph.AddEdge({2, 3});
    graph.AddEdge({10, 9});
    graph.AddEdge({4, 5});
    graph.AddEdge({5, 7});
    graph.AddEdge({4, 7});
    graph.AddEdge({1, 2});
    graph.AddEdge({3, 4});
    graph.AddEdge({1, 6});

    ASSERT_THROW(TopologicalSort(graph), std::invalid_argument);
  }
}
