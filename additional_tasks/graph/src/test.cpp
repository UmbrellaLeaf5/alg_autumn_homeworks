#include <gtest/gtest.h>

#include "graph/graph.hpp"

TEST(GraphTest_size_t, CreateNonWeightedGraph) {
  std::vector<std::pair<size_t, size_t>> edges_pairs = {{0, 1}, {1, 2}, {2, 0}};
  Graph<size_t, long> graph =
      Graph<size_t, long>::GraphNonWeighted(edges_pairs);

  ASSERT_EQ(graph.VertsAmount(), 3);
  ASSERT_EQ(graph.EdgesAmount(), 3);

  ASSERT_FALSE(graph.IsWeighted());
  ASSERT_TRUE(graph.IsDirected());
}

TEST(GraphTest_size_t, CreateWeightedGraph) {
  std::vector<std::pair<size_t, size_t>> edges_pairs = {{0, 1}, {1, 2}, {2, 0}};
  std::vector<long> weights = {1, 2, 3};
  Graph<size_t, long> graph =
      Graph<size_t, long>::GraphWeighted(edges_pairs, weights);

  ASSERT_EQ(graph.VertsAmount(), 3);
  ASSERT_EQ(graph.EdgesAmount(), 3);

  ASSERT_TRUE(graph.IsWeighted());
  ASSERT_TRUE(graph.IsDirected());
}

TEST(GraphTest_size_t, CreateGraphFromAdjacencyMatrix) {
  std::vector<std::vector<long>> adj_matrix = {{0, 1, 0}, {1, 0, 1}, {0, 1, 0}};
  Graph<size_t, long> graph =
      Graph<size_t, long>::GraphFromAdjMatrix(adj_matrix);

  // std::cout << graph.GetAdjMatrix() << std::endl;
  // std::cout << graph.GetAdjListWithoutKeys() << std::endl;

  graph.MakeUndirected();

  ASSERT_EQ(graph.VertsAmount(), 3);
  ASSERT_EQ(graph.EdgesAmount(), 2);

  ASSERT_FALSE(graph.IsWeighted());

  // std::cout << graph.GetAdjMatrix() << std::endl;
  // std::cout << graph.GetAdjListWithoutKeys() << std::endl;
}

TEST(GraphTest_size_t, CreateGraphFromAdjacencyMatrix2) {
  std::vector<std::vector<long>> adj_matrix = {{0, 1, 0}, {1, 0, 1}, {0, 1, 0}};
  Graph<size_t, long> graph =
      Graph<size_t, long>::GraphFromAdjMatrix(adj_matrix);

  ASSERT_EQ(graph.VertsAmount(), 3);
  ASSERT_EQ(graph.EdgesAmount(), 4);

  ASSERT_FALSE(graph.IsWeighted());

  // std::cout << graph.GetAdjMatrix() << std::endl;
  // std::cout << graph.GetAdjListWithoutKeys() << std::endl;
}

TEST(GraphTest_size_t, CreateWeightedGraphFromAdjacencyMatrix) {
  std::vector<std::vector<long>> adj_matrix = {{0, 1, 0}, {1, 0, 5}, {0, 7, 0}};
  Graph<size_t, long> graph =
      Graph<size_t, long>::GraphFromAdjMatrix(adj_matrix, true);

  // std::cout << graph.GetAdjMatrix() << std::endl;
  // graph.PrintAdjList();
  // std::cout << graph << std::endl;

  graph.MakeUndirected();

  ASSERT_EQ(graph.VertsAmount(), 3);
  ASSERT_EQ(graph.EdgesAmount(), 2);

  ASSERT_TRUE(graph.IsWeighted());

  // std::cout << graph.GetAdjMatrix() << std::endl;
  // graph.PrintAdjList();
  // std::cout << graph << std::endl;
}

TEST(GraphTest_size_t, CreateWeightedGraphFromAdjacencyMatrix2) {
  std::vector<std::vector<long>> adj_matrix = {{0, 1, 0}, {1, 0, 9}, {0, 6, 0}};
  Graph<size_t, long> graph =
      Graph<size_t, long>::GraphFromAdjMatrix(adj_matrix, true);

  ASSERT_EQ(graph.VertsAmount(), 3);
  ASSERT_EQ(graph.EdgesAmount(), 4);

  ASSERT_TRUE(graph.IsWeighted());

  // std::cout << graph.GetAdjMatrix() << std::endl;
  // std::cout << graph.GetAdjListWithoutKeys() << std::endl;
}

TEST(GraphTest_size_t, CreateGraphFromAdjacencyList) {
  std::vector<std::vector<size_t>> adj_list = {{1}, {0, 2}, {1}};
  Graph<size_t, long> graph = Graph<size_t, long>::GraphFromAdjList(adj_list);

  // std::cout << graph.GetAdjMatrix() << std::endl;
  // std::cout << graph.GetAdjListWithoutKeys() << std::endl;

  graph.MakeUndirected();

  ASSERT_EQ(graph.VertsAmount(), 3);
  ASSERT_EQ(graph.EdgesAmount(), 2);

  ASSERT_FALSE(graph.IsWeighted());

  graph.MakeDirected();
  ASSERT_TRUE(graph.IsDirected());

  // std::cout << graph.GetAdjMatrix() << std::endl;
  // std::cout << graph.GetAdjListWithoutKeys() << std::endl;
}

TEST(GraphTest_size_t, MakeUndirected) {
  std::vector<std::pair<size_t, size_t>> edges = {{0, 1}, {1, 2}, {2, 0}};
  Graph<size_t, long> g = Graph<size_t, long>::GraphNonWeighted(edges);

  ASSERT_TRUE(g.IsDirected());
  g.MakeUndirected();
  ASSERT_FALSE(g.IsDirected());
}

TEST(GraphTest_size_t, CreateGraphWithEmptyEdges) {
  std::vector<std::pair<size_t, size_t>> edges;
  Graph<size_t, long> g = Graph<size_t, long>::GraphNonWeighted(edges);

  ASSERT_EQ(g.VertsAmount(), 0);
  ASSERT_EQ(g.EdgesAmount(), 0);
  ASSERT_FALSE(g.IsWeighted());
  ASSERT_TRUE(g.IsDirected());
}

TEST(GraphTest_size_t, CreateGraphWithDuplicateEdges) {
  std::vector<std::pair<size_t, size_t>> edges = {
      {0, 1}, {1, 2}, {0, 1}, {2, 1}, {1, 2}};
  // для скорости работы по умолчанию дубликаты допускаются
  Graph<size_t, long> g = Graph<size_t, long>::GraphNonWeighted(edges);

  ASSERT_EQ(g.VertsAmount(), 3);
  ASSERT_EQ(g.EdgesAmount(), 5);
  ASSERT_FALSE(g.IsWeighted());
  ASSERT_TRUE(g.IsDirected());

  g.RemoveDuplicates();

  ASSERT_EQ(g.VertsAmount(), 3);
  ASSERT_EQ(g.EdgesAmount(), 3);
  ASSERT_FALSE(g.IsWeighted());
  ASSERT_TRUE(g.IsDirected());

  g.MakeUndirected();

  ASSERT_EQ(g.VertsAmount(), 3);
  ASSERT_EQ(g.EdgesAmount(), 2);
  ASSERT_FALSE(g.IsWeighted());
  ASSERT_FALSE(g.IsDirected());
}

TEST(GraphTest_size_t, CreateGraphWithInvalidWeights) {
  std::vector<std::pair<size_t, size_t>> edges = {{0, 1}, {1, 2}, {2, 0}};
  std::vector<long> weights = {1, 2, -1};

  // IDK: non-working
  // ASSERT_THROW((Graph<size_t, long>::GraphWeighted(edges, weights)),
  //              std::invalid_argument());

  try {
    Graph<size_t, long>::GraphWeighted(edges, weights);
    FAIL() << "Expected exception was not thrown.";

  } catch (const std::invalid_argument& e) {
    SUCCEED() << "CreateGraphWithInvalidWeights";
  } catch (const std::exception& e) {
    FAIL() << "Unexpected exception caught: " << e.what();
  }
}

TEST(GraphTest_size_t, CreateGraphFromAdjMatrixWithInvalidSize) {
  std::vector<std::vector<long>> adj_matrix = {{0, 1}, {1, 0, 1}, {1, 0}};

  EXPECT_THROW((Graph<size_t, long>::GraphFromAdjMatrix(adj_matrix)),
               std::invalid_argument);
}

TEST(GraphTest_size_t, GraphFromMapTest) {
  std::unordered_map<std::string, long> edges_dict = {
      {"0->1", 5}, {"2->1", 1}, {"3->2", 2}, {"1->3", 3}};
  Graph<size_t, long> graph = Graph<size_t, long>::GraphFromMap(edges_dict);

  // graph.PrintAdjList();
  // std::cout << graph.GetAdjListWithoutKeys() << std::endl;

  ASSERT_EQ(graph.VertsAmount(), 4);
  ASSERT_EQ(graph.EdgesAmount(), 4);

  ASSERT_TRUE(graph.IsWeighted());
  ASSERT_TRUE(graph.IsDirected());

  std::unordered_map<std::string, long> empty_edges_dict;
  Graph<size_t, long> empty_graph =
      Graph<size_t, long>::GraphFromMap(empty_edges_dict);

  ASSERT_EQ(empty_graph.VertsAmount(), 0);
  ASSERT_EQ(empty_graph.EdgesAmount(), 0);

  ASSERT_FALSE(empty_graph.IsWeighted());
  ASSERT_TRUE(empty_graph.IsDirected());

  std::unordered_map<std::string, long> invalid_edges_dict = {
      {"0-1", 0}, {"2-1", 1}, {"3->2", 2}, {"1>3", 3}};

  EXPECT_THROW((Graph<size_t, long>::GraphFromMap(invalid_edges_dict)),
               std::invalid_argument);

  edges_dict = {{"0->14", 5}, {"25->1", 1}, {"3->2", 2}, {"1->3", 3}};

  graph = Graph<size_t, long>::GraphFromMap(edges_dict);

  // graph.PrintAdjList();
  // std::cout << graph.GetAdjListWithoutKeys() << std::endl;
  // std::cout << graph << std::endl;

  ASSERT_EQ(graph.VertsAmount(), 26);
  ASSERT_EQ(graph.EdgesAmount(), 4);

  ASSERT_TRUE(graph.IsWeighted());
  ASSERT_TRUE(graph.IsDirected());
}

TEST(GraphTest_size_t, GraphFromStrsTest) {
  std::vector<std::string> edges_strs = {"0->1", "2->1", "3->2", "1->3"};
  Graph<size_t, long> graph = Graph<size_t, long>::GraphFromStrs(edges_strs);

  ASSERT_EQ(graph.VertsAmount(), 4);
  ASSERT_EQ(graph.EdgesAmount(), 4);

  ASSERT_FALSE(graph.IsWeighted());
  ASSERT_TRUE(graph.IsDirected());

  std::vector<std::string> empty_edges_strs;
  Graph<size_t, long> empty_graph =
      Graph<size_t, long>::GraphFromStrs(empty_edges_strs);

  ASSERT_EQ(empty_graph.VertsAmount(), 0);
  ASSERT_EQ(empty_graph.EdgesAmount(), 0);

  ASSERT_FALSE(empty_graph.IsWeighted());
  ASSERT_TRUE(empty_graph.IsDirected());

  std::vector<std::string> invalid_edges_strs = {"0-1", "2-1", "3->2", "1>3"};
  EXPECT_THROW((Graph<size_t, long>::GraphFromStrs(invalid_edges_strs)),
               std::invalid_argument);

  invalid_edges_strs = {"0->vert", "2->1", "3->2", "1->3"};
  EXPECT_THROW((Graph<size_t, long>::GraphFromStrs(invalid_edges_strs)),
               std::invalid_argument);
}

TEST(GraphTest_size_t, GraphWeighted_EmptyInput) {
  std::vector<std::tuple<size_t, size_t, long>> empty_input;
  Graph<size_t, long> graph = Graph<size_t, long>::GraphWeighted(empty_input);

  ASSERT_EQ(graph.VertsAmount(), 0);
  ASSERT_EQ(graph.EdgesAmount(), 0);
}

TEST(GraphTest_size_t, GraphWeighted_SingleEdge) {
  std::vector<std::tuple<size_t, size_t, long>> input = {{0, 1, 5}};
  Graph<size_t, long> graph = Graph<size_t, long>::GraphWeighted(input);

  ASSERT_EQ(graph.VertsAmount(), 2);
  ASSERT_EQ(graph.EdgesAmount(), 1);
  ASSERT_TRUE(graph.IsWeighted());
}

TEST(GraphTest_size_t, GraphWeighted_MultipleEdges) {
  std::vector<std::tuple<size_t, size_t, long>> input = {
      {0, 1, 5}, {1, 2, 10}, {2, 0, 3}};
  Graph<size_t, long> graph = Graph<size_t, long>::GraphWeighted(input);

  ASSERT_EQ(graph.VertsAmount(), 3);
  ASSERT_EQ(graph.EdgesAmount(), 3);
  ASSERT_TRUE(graph.IsWeighted());
}

TEST(GraphTest_size_t, Edges_EmptyGraph) {
  std::vector<std::tuple<size_t, size_t, long>> empty_input;
  Graph<size_t, long> graph = Graph<size_t, long>::GraphWeighted(empty_input);
  auto edges = graph.Edges();

  ASSERT_TRUE(edges.empty());
}

TEST(GraphTest_size_t, Edges_SingleEdge) {
  std::vector<std::tuple<size_t, size_t, long>> input = {{0, 1, 5}};
  Graph<size_t, long> graph = Graph<size_t, long>::GraphWeighted(input);
  auto edges = graph.Edges();

  ASSERT_EQ(edges.size(), 1);
  ASSERT_EQ(StartVertFromTuple(edges[0]), 0);
  ASSERT_EQ(EndVertFromTuple(edges[0]), 1);
  ASSERT_EQ(WeightFromTuple(edges[0]), 5);
}

TEST(GraphTest_size_t, Edges_MultipleEdges) {
  std::vector<std::tuple<size_t, size_t, long>> input = {
      {0, 1, 5}, {1, 2, 10}, {2, 0, 3}};
  Graph<size_t, long> graph = Graph<size_t, long>::GraphWeighted(input);
  auto edges = graph.Edges();

  ASSERT_EQ(edges.size(), 3);

  ASSERT_EQ(StartVertFromTuple(edges[0]), 0);
  ASSERT_EQ(EndVertFromTuple(edges[0]), 1);
  ASSERT_EQ(WeightFromTuple(edges[0]), 5);

  ASSERT_EQ(StartVertFromTuple(edges[1]), 1);
  ASSERT_EQ(EndVertFromTuple(edges[1]), 2);
  ASSERT_EQ(WeightFromTuple(edges[1]), 10);

  ASSERT_EQ(StartVertFromTuple(edges[2]), 2);
  ASSERT_EQ(EndVertFromTuple(edges[2]), 0);
  ASSERT_EQ(WeightFromTuple(edges[2]), 3);
}

TEST(GraphTest_size_t, Edges_ConstAccess) {
  std::vector<std::tuple<size_t, size_t, long>> input = {
      {0, 1, 5}, {1, 2, 10}, {2, 0, 3}};
  Graph<size_t, long> graph = Graph<size_t, long>::GraphWeighted(input);
  const auto& edges = graph.Edges();

  ASSERT_EQ(edges.size(), 3);

  ASSERT_EQ(StartVertFromTuple(edges[0]), 0);
  ASSERT_EQ(EndVertFromTuple(edges[0]), 1);
  ASSERT_EQ(WeightFromTuple(edges[0]), 5);
}

TEST(GraphTest_size_t, ContainsEdgeWeighted) {
  Graph<size_t, long> g =
      Graph<size_t, long>::GraphWeighted({{0, 1, 5}, {1, 2, 10}, {2, 0, 3}});

  ASSERT_TRUE(g.ContainsEdge({0, 1, 5}));
  ASSERT_TRUE(g.ContainsEdge({1, 2, 10}));
  ASSERT_TRUE(g.ContainsEdge({2, 0, 3}));

  ASSERT_FALSE(g.ContainsEdge({0, 2, 5}));
  ASSERT_FALSE(g.ContainsEdge({1, 0, 10}));
  ASSERT_FALSE(g.ContainsEdge({2, 1, 3}));
}

TEST(GraphTest_size_t, ContainsEdgeNonWeighted) {
  Graph<size_t, long> g =
      Graph<size_t, long>::GraphNonWeighted({{0, 1}, {1, 2}, {2, 0}});

  ASSERT_TRUE(g.ContainsEdge({0, 1}));
  ASSERT_TRUE(g.ContainsEdge({1, 2}));
  ASSERT_TRUE(g.ContainsEdge({2, 0}));

  ASSERT_FALSE(g.ContainsEdge({0, 2}));
  ASSERT_FALSE(g.ContainsEdge({1, 0}));
  ASSERT_FALSE(g.ContainsEdge({2, 1}));
}

TEST(GraphTest_size_t, ContainsEdgeThrowsOnNonWeightedGraph) {
  Graph<size_t, long> g =
      Graph<size_t, long>::GraphNonWeighted({{0, 1}, {1, 2}, {2, 0}});
  ASSERT_THROW(g.ContainsEdge({0, 1, 5}), std::logic_error);
}

TEST(GraphTest_size_t, ContainsEdgeThrowsOnNegativeWeight) {
  Graph<size_t, long> g =
      Graph<size_t, long>::GraphWeighted({{0, 1, 5}, {1, 2, 10}, {2, 0, 3}});
  ASSERT_THROW(g.ContainsEdge({0, 1, -5}), std::logic_error);
}

TEST(GraphTest_size_t, GetWeightOfExistingEdge) {
  Graph<size_t, long> graph = Graph<size_t, long>::GraphWeighted(
      {{0, 1, 250L}, {1, 2, 370L}, {2, 0, 120L}});

  ASSERT_EQ(graph.GetWeightOfEdge({0, 1}), 250L);
  ASSERT_EQ(graph.GetWeightOfEdge({1, 2}), 370L);
  ASSERT_EQ(graph.GetWeightOfEdge({2, 0}), 120L);
}

TEST(GraphTest_size_t, GetWeightOfNonExistingEdge) {
  Graph<size_t, long> graph = Graph<size_t, long>::GraphWeighted(
      {{0, 1, 250L}, {1, 2, 370L}, {2, 0, 120L}});

  ASSERT_THROW(graph.GetWeightOfEdge({0, 2}), std::invalid_argument);
  ASSERT_THROW(graph.GetWeightOfEdge({1, 0}), std::invalid_argument);
}

TEST(GraphTest_size_t, GetWeightOfEdgeInNonWeightedGraph) {
  Graph<size_t, long> non_weighted_graph =
      Graph<size_t, long>::GraphNonWeighted({{0, 1}, {1, 2}, {2, 0}});

  ASSERT_THROW(non_weighted_graph.GetWeightOfEdge({0, 1}), std::logic_error);
}

TEST(GraphTest_size_t, AddVert) {
  Graph<size_t, long> g;

  // Тест добавления новой вершины
  g.AddVert(0);
  EXPECT_EQ(g.VertsAmount(), 1);
  EXPECT_TRUE(Contains(g.Verts(), size_t(0)));

  // Тест добавления уже существующей вершины
  g.AddVert(0);
  EXPECT_EQ(g.VertsAmount(), 1);
  EXPECT_TRUE(Contains(g.Verts(), size_t(0)));
}

TEST(GraphTest_size_t, AddEdgeUnweighted) {
  Graph<size_t, long> g;

  g.AddEdge(0, 1);
  EXPECT_EQ(g.EdgesAmount(), 1);
  EXPECT_TRUE(g.ContainsEdge({0, 1}));

  g.AddEdge(0, 1);
  EXPECT_EQ(g.EdgesAmount(), 2);
  EXPECT_TRUE(g.ContainsEdge({0, 1}));

  g.AddEdge(2, 3);
  EXPECT_EQ(g.VertsAmount(), 4);
  EXPECT_TRUE(Contains(g.Verts(), size_t(2)));
  EXPECT_TRUE(Contains(g.Verts(), size_t(3)));
}

TEST(GraphTest_size_t, AddEdgeWeighted) {
  Graph<size_t, long> g;

  g.AddEdge(0, 1, 10);
  EXPECT_EQ(g.EdgesAmount(), 1);
  EXPECT_TRUE(g.ContainsEdge({0, 1, 10}));
  EXPECT_EQ(g.GetWeightOfEdge({0, 1}), 10);

  // std::cout << g << std::endl;

  g.AddEdge(0, 1, 15);

  // если пользователь решает добавить то же ребро,
  // но с другим весом - он сам дурак

  // (иначе добавление ребра будет работать за O(E))

  g.RemoveDuplicates();

  EXPECT_EQ(g.EdgesAmount(), 2);
  EXPECT_TRUE(g.ContainsEdge({0, 1, 10}));
  EXPECT_EQ(g.GetWeightOfEdge({0, 1}), 10);

  EXPECT_THROW(g.AddEdge(2, 3), std::logic_error);
}

TEST(GraphTest_size_t, RemoveVert) {
  Graph<size_t, long> g;
  g.AddVert(1);
  g.AddVert(2);
  g.AddVert(3);
  g.AddEdge(1, 2, 5);
  g.AddEdge(2, 3, 7);

  ASSERT_NO_THROW(g.RemoveVert(2));
  EXPECT_FALSE(Contains(g.Verts(), size_t(2)));
  EXPECT_FALSE(g.ContainsEdge({1, 2}));
  EXPECT_FALSE(g.ContainsEdge({2, 3}));

  ASSERT_THROW(g.RemoveVert(4), std::invalid_argument);
}

TEST(GraphTest_size_t, RemoveEdgeByPair) {
  Graph<size_t, long> g;
  g.AddVert(1);
  g.AddVert(2);
  g.AddVert(3);
  g.AddEdge(1, 2, 5);
  g.AddEdge(2, 3, 7);

  // std::cout << g << std::endl;

  ASSERT_NO_THROW(g.RemoveEdge({1, 2}));
  EXPECT_FALSE(g.ContainsEdge({1, 2}));
  EXPECT_TRUE(g.ContainsEdge({2, 3}));

  // std::cout << g << std::endl;

  ASSERT_THROW(g.RemoveEdge({1, 3}), std::invalid_argument);
}

TEST(GraphTest_size_t, RemoveEdgeByTuple) {
  Graph<size_t, long> g;
  g.AddVert(1);
  g.AddVert(2);
  g.AddVert(3);
  g.AddEdge(1, 2, 5);
  g.AddEdge(2, 3, 7);

  ASSERT_NO_THROW(g.RemoveEdge({2, 3, 7}));
  EXPECT_FALSE(g.ContainsEdge({2, 3, 7}));
  EXPECT_TRUE(g.ContainsEdge({1, 2, 5}));

  ASSERT_THROW(g.RemoveEdge({1, 3, 10}), std::invalid_argument);
}

TEST(GraphTest_size_t, AddAndRemoveVerts) {
  Graph<size_t, long> g;
  EXPECT_EQ(g.VertsAmount(), 0);

  g.AddVert(1);
  g.AddVert(2);
  g.AddVert(3);
  EXPECT_EQ(g.VertsAmount(), 3);
  EXPECT_TRUE(Contains(g.Verts(), size_t(1)));
  EXPECT_TRUE(Contains(g.Verts(), size_t(2)));
  EXPECT_TRUE(Contains(g.Verts(), size_t(3)));

  g.RemoveVert(2);
  EXPECT_EQ(g.VertsAmount(), 2);
  EXPECT_TRUE(Contains(g.Verts(), size_t(1)));
  EXPECT_FALSE(Contains(g.Verts(), size_t(2)));
  EXPECT_TRUE(Contains(g.Verts(), size_t(3)));
}

TEST(GraphTest_size_t, AddAndRemoveEdges) {
  Graph<size_t, long> g;
  g.AddVert(1);
  g.AddVert(2);
  g.AddVert(3);

  g.AddEdge(1, 2, 5);
  g.AddEdge(2, 3, 7);
  EXPECT_EQ(g.EdgesAmount(), 2);
  EXPECT_TRUE(g.ContainsEdge({1, 2, 5}));
  EXPECT_TRUE(g.ContainsEdge({2, 3, 7}));

  g.RemoveEdge({1, 2, 5});
  EXPECT_EQ(g.EdgesAmount(), 1);
  EXPECT_FALSE(g.ContainsEdge({1, 2, 5}));
  EXPECT_TRUE(g.ContainsEdge({2, 3, 7}));
}

TEST(GraphTest_size_t, ContainsAndGetVerts) {
  Graph<size_t, long> g;
  g.AddVert(1);
  g.AddVert(2);
  g.AddVert(3);

  EXPECT_TRUE(Contains(g.Verts(), size_t(1)));
  EXPECT_TRUE(Contains(g.Verts(), size_t(2)));
  EXPECT_TRUE(Contains(g.Verts(), size_t(3)));
  EXPECT_FALSE(Contains(g.Verts(), size_t(4)));

  std::vector<size_t> verts = g.Verts();
  EXPECT_EQ(verts.size(), 3);
  EXPECT_TRUE(std::find(verts.begin(), verts.end(), 1) != verts.end());
  EXPECT_TRUE(std::find(verts.begin(), verts.end(), 2) != verts.end());
  EXPECT_TRUE(std::find(verts.begin(), verts.end(), 3) != verts.end());
}

TEST(GraphTest_size_t, RemoveEdgeFromDirectedGraph) {
  Graph<size_t, long> graph_directed{};
  graph_directed.AddVert(0);
  graph_directed.AddVert(1);
  graph_directed.AddVert(2);
  graph_directed.AddEdge(0, 1, 1);
  graph_directed.AddEdge(1, 2, 2);

  ASSERT_NO_THROW(graph_directed.RemoveEdge({0, 1}));
  ASSERT_FALSE(graph_directed.ContainsEdge({0, 1}));
  ASSERT_TRUE(graph_directed.ContainsEdge({1, 2}));
}

TEST(GraphTest_size_t, RemoveEdgeFromUndirectedGraph) {
  Graph<size_t, long> graph_undirected{};
  graph_undirected.AddVert(0);
  graph_undirected.AddVert(1);
  graph_undirected.AddVert(2);
  graph_undirected.AddEdge(0, 1, 1);
  graph_undirected.AddEdge(1, 2, 2);

  graph_undirected.MakeUndirected();

  ASSERT_NO_THROW(graph_undirected.RemoveEdge({0, 1}));
  ASSERT_FALSE(graph_undirected.ContainsEdge({0, 1}));
  ASSERT_FALSE(graph_undirected.ContainsEdge({1, 0}));
  ASSERT_TRUE(graph_undirected.ContainsEdge({1, 2}));
}

TEST(GraphTest_size_t, RemoveNonExistentEdgeThrowsException) {
  Graph<size_t, long> graph_directed{};
  graph_directed.AddVert(0);
  graph_directed.AddVert(1);
  graph_directed.AddVert(2);
  graph_directed.AddEdge(0, 1, 1);
  graph_directed.AddEdge(1, 2, 2);

  ASSERT_THROW(graph_directed.RemoveEdge({0, 2}), std::invalid_argument);

  Graph<size_t, long> graph_undirected{};
  graph_undirected.AddVert(0);
  graph_undirected.AddVert(1);
  graph_undirected.AddVert(2);
  graph_undirected.AddEdge(0, 1, 1);
  graph_undirected.AddEdge(1, 2, 2);

  graph_undirected.MakeUndirected();

  ASSERT_THROW(graph_undirected.RemoveEdge({0, 2}), std::invalid_argument);
}
