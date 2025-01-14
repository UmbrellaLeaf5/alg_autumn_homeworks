#include <gtest/gtest.h>

#include "lca.hpp"

TEST(LCATest, IntGraphDirected) {
  Graph<int> graph =
      Graph<int>::GraphFromAdjList({{1, 2}, {0, 3, 4}, {0, 5, 6}});
  LCA<int> lca(graph, 0);
  EXPECT_EQ(lca.Ancestor(3, 4), 1);
  EXPECT_EQ(lca.Ancestor(3, 6), 0);
  EXPECT_EQ(lca.Ancestor(5, 6), 2);
  EXPECT_EQ(lca.Ancestor(1, 6), 0);
  EXPECT_EQ(CalculateLCA(graph, 0, 3, 4), 1);  // Test FindLCA function
  EXPECT_EQ(CalculateLCA(graph, 0, 3, 6), 0);
}

TEST(LCATest, IntGraphUndirected) {
  Graph<int> graph =
      Graph<int>::GraphFromAdjList({{1, 2}, {0, 3, 4}, {0, 5, 6}});
  graph.MakeUndirected();
  LCA<int> lca(graph, 0);
  EXPECT_EQ(lca.Ancestor(3, 4), 1);
  EXPECT_EQ(lca.Ancestor(3, 6), 0);
  EXPECT_EQ(lca.Ancestor(5, 6), 2);
  EXPECT_EQ(lca.Ancestor(1, 6), 0);
  EXPECT_EQ(CalculateLCA(graph, 0, 3, 4), 1);  // Test FindLCA function
  EXPECT_EQ(CalculateLCA(graph, 0, 3, 6), 0);
}

TEST(LCATest, IntGraphSingleNode) {
  Graph<int> graph;

  EXPECT_THROW((LCA<int>(graph, 0)), std::invalid_argument);

  graph.AddVert(0);

  LCA<int> lca(graph, 0);
  // If there is only the root, LCA should still work, return the root itself
  EXPECT_EQ(lca.Ancestor(0, 0), 0);
  // test if we can actually call CalculateLCA function with an empty graph
  EXPECT_EQ(CalculateLCA(graph, 0, 0, 0), 0);
}

// Тесты для класса LCA и функции FindLCA с string вершинами
TEST(LCATest, StringGraphDirected) {
  Graph<std::string> graph = Graph<std::string>::GraphFromStrs(
      {"a->b", "a->c", "b->d", "b->e", "c->f", "c->g"});
  LCA<std::string> lca(graph, "a");
  EXPECT_EQ(lca.Ancestor("d", "e"), "b");
  EXPECT_EQ(lca.Ancestor("d", "f"), "a");
  EXPECT_EQ(lca.Ancestor("g", "e"), "a");
  EXPECT_EQ(lca.Ancestor("b", "g"), "a");
  EXPECT_EQ(
      CalculateLCA(graph, std::string("a"), std::string("d"), std::string("e")),
      std::string("b"));  // Test FindLCA function
  EXPECT_EQ(
      CalculateLCA(graph, std::string("a"), std::string("d"), std::string("f")),
      std::string("a"));
}

TEST(LCATest, StringGraphUndirected) {
  Graph<std::string> graph = Graph<std::string>::GraphFromStrs(
      {"a->b", "a->c", "b->d", "b->e", "c->f", "c->g"});
  graph.MakeUndirected();
  LCA<std::string> lca(graph, "a");
  EXPECT_EQ(lca.Ancestor("d", "e"), "b");
  EXPECT_EQ(lca.Ancestor("d", "f"), "a");
  EXPECT_EQ(lca.Ancestor("g", "e"), "a");
  EXPECT_EQ(lca.Ancestor("b", "g"), "a");
  EXPECT_EQ(
      CalculateLCA(graph, std::string("a"), std::string("d"), std::string("e")),
      std::string("b"));  // Test FindLCA function
  EXPECT_EQ(
      CalculateLCA(graph, std::string("a"), std::string("d"), std::string("f")),
      std::string("a"));
}

TEST(LCATest, StringGraphSingleNode) {
  Graph<std::string> graph = Graph<std::string>::GraphFromStrs({});

  EXPECT_THROW((LCA<std::string>(graph, "a")), std::invalid_argument);

  graph.AddVert("a");

  LCA<std::string> lca(graph, "a");
  EXPECT_EQ(lca.Ancestor("a", "a"), "a");
  EXPECT_EQ(
      CalculateLCA(graph, std::string("a"), std::string("a"), std::string("a")),
      std::string("a"));
}

// Тесты для граничных случаев
TEST(LCATest, SameNodeLCA) {
  Graph<int> graph =
      Graph<int>::GraphFromAdjList({{1, 2}, {0, 3, 4}, {0, 5, 6}});
  LCA<int> lca(graph, 0);
  EXPECT_EQ(lca.Ancestor(3, 3), 3);
  EXPECT_EQ(CalculateLCA(graph, 0, 3, 3), 3);
}

TEST(LCATest, RootNodeLCA) {
  Graph<int> graph =
      Graph<int>::GraphFromAdjList({{1, 2}, {0, 3, 4}, {0, 5, 6}});
  LCA<int> lca(graph, 0);
  EXPECT_EQ(lca.Ancestor(0, 3), 0);
  EXPECT_EQ(CalculateLCA(graph, 0, 0, 3), 0);
}

TEST(LCATest, IsolatedIntVertex) {
  Graph<int> graph =
      Graph<int>::GraphFromAdjList({{1, 2}, {0, 3, 4}, {0, 5, 6}});
  LCA<int> lca(graph, 0);

  EXPECT_THROW({ lca.Ancestor(3, 7); }, std::invalid_argument);
  EXPECT_THROW({ CalculateLCA(graph, 0, 3, 7); }, std::invalid_argument);
}

TEST(LCATest, IsolatedStringVertex) {
  Graph<std::string> graph = Graph<std::string>::GraphFromStrs(
      {"a->b", "a->c", "b->d", "b->e", "c->f", "c->g"});
  LCA<std::string> lca(graph, "a");
  EXPECT_THROW({ lca.Ancestor("d", "h"); }, std::invalid_argument);
  EXPECT_THROW(
      {
        CalculateLCA(graph, std::string("a"), std::string("d"),
                     std::string("h"));
      },
      std::invalid_argument);
}

TEST(LCATest, IsolatedIntRoot) {
  Graph<int> graph;
  EXPECT_THROW((LCA<int>(graph, 0)),
               std::invalid_argument);  // проверяем исключение в конструкторе
}

TEST(LCATest, NotConnectedToInt) {
  Graph<int> graph =
      Graph<int>::GraphFromAdjList(std::vector<std::vector<int>>({{}, {}}));

  EXPECT_THROW({ CalculateLCA(graph, 0, 0, 1); }, std::logic_error);
}

TEST(LCATest, NotConnectedToString) {
  Graph<std::string> graph = Graph<std::string>::GraphFromStrs({"a->b"});
  LCA<std::string> lca(graph, "a");
  EXPECT_THROW({ lca.Ancestor("a", "c"); }, std::logic_error);
  EXPECT_THROW(
      {
        CalculateLCA(graph, std::string("a"), std::string("a"),
                     std::string("c"));
      },
      std::logic_error);
}

TEST(LCATest, IntGraphDirected_Complex1) {
  Graph<int> graph =
      Graph<int>::GraphFromAdjList({{1, 2}, {3, 4}, {5}, {}, {}, {}});
  LCA<int> lca(graph, 0);
  EXPECT_EQ(lca.Ancestor(3, 4), 1);
  EXPECT_EQ(lca.Ancestor(3, 5), 0);
  EXPECT_EQ(lca.Ancestor(4, 5), 0);
  EXPECT_EQ(lca.Ancestor(1, 2), 0);
  EXPECT_EQ(CalculateLCA(graph, 0, 3, 4), 1);
  EXPECT_EQ(CalculateLCA(graph, 0, 3, 5), 0);
}

TEST(LCATest, IntGraphDirected_Complex2) {
  Graph<int> graph =
      Graph<int>::GraphFromAdjList({{1, 2}, {3, 4}, {5}, {6}, {}, {7}, {}});
  LCA<int> lca(graph, 0);
  EXPECT_EQ(lca.Ancestor(6, 7), 0);
  EXPECT_EQ(lca.Ancestor(3, 6), 3);
  EXPECT_EQ(lca.Ancestor(5, 7), 5);
  EXPECT_EQ(lca.Ancestor(4, 7), 0);

  EXPECT_EQ(CalculateLCA(graph, 0, 6, 7), 0);
  EXPECT_EQ(CalculateLCA(graph, 0, 3, 6), 3);
}

TEST(LCATest, IntGraphUndirected_Complex1) {
  Graph<int> graph =
      Graph<int>::GraphFromAdjList({{1, 2}, {0, 3, 4}, {0}, {1}, {1}, {}});
  graph.MakeUndirected();
  LCA<int> lca(graph, 0);
  EXPECT_EQ(lca.Ancestor(3, 4), 1);
  EXPECT_EQ(lca.Ancestor(2, 3), 0);
  EXPECT_EQ(lca.Ancestor(4, 2), 0);
  EXPECT_EQ(lca.Ancestor(1, 4), 1);
  EXPECT_EQ(CalculateLCA(graph, 0, 3, 4), 1);
  EXPECT_EQ(CalculateLCA(graph, 0, 2, 3), 0);
}

TEST(LCATest, IntGraphUndirected_Complex2) {
  Graph<int> graph = Graph<int>::GraphFromAdjList(
      {{1}, {0, 2, 3}, {1}, {1, 4}, {3, 5, 6}, {4}, {4}});
  graph.MakeUndirected();
  LCA<int> lca(graph, 0);
  EXPECT_EQ(lca.Ancestor(4, 5), 4);
  EXPECT_EQ(lca.Ancestor(2, 5), 1);
  EXPECT_EQ(lca.Ancestor(1, 6), 1);
  EXPECT_EQ(lca.Ancestor(1, 4), 1);
  EXPECT_EQ(CalculateLCA(graph, 0, 4, 5), 4);
  EXPECT_EQ(CalculateLCA(graph, 0, 2, 5), 1);
}

TEST(LCATest, StringGraphDirected_Complex1) {
  Graph<std::string> graph = Graph<std::string>::GraphFromStrs(
      {"a->b", "a->c", "b->d", "c->e", "c->f", "f->g"});
  LCA<std::string> lca(graph, "a");
  EXPECT_EQ(lca.Ancestor("d", "g"), "a");
  EXPECT_EQ(lca.Ancestor("g", "e"), "c");
  EXPECT_EQ(lca.Ancestor("b", "f"), "a");
  EXPECT_EQ(
      CalculateLCA(graph, std::string("a"), std::string("d"), std::string("g")),
      std::string("a"));
  EXPECT_EQ(
      CalculateLCA(graph, std::string("a"), std::string("g"), std::string("e")),
      std::string("c"));
}

TEST(LCATest, StringGraphDirected_Complex2) {
  Graph<std::string> graph = Graph<std::string>::GraphFromStrs(
      {"a->b", "a->c", "b->d", "b->e", "c->f", "f->g", "f->h", "g->i"});
  LCA<std::string> lca(graph, "a");
  EXPECT_EQ(lca.Ancestor("i", "h"), "f");
  EXPECT_EQ(lca.Ancestor("d", "i"), "a");
  EXPECT_EQ(lca.Ancestor("e", "d"), "b");
  EXPECT_EQ(
      CalculateLCA(graph, std::string("a"), std::string("i"), std::string("h")),
      std::string("f"));
  EXPECT_EQ(
      CalculateLCA(graph, std::string("a"), std::string("d"), std::string("e")),
      std::string("b"));
}

TEST(LCATest, StringGraphUndirected_Complex1) {
  Graph<std::string> graph = Graph<std::string>::GraphFromStrs(
      {"a->b", "a->c", "b->d", "c->e", "c->f", "f->g"});
  graph.MakeUndirected();
  LCA<std::string> lca(graph, "a");
  EXPECT_EQ(lca.Ancestor("e", "g"), "c");
  EXPECT_EQ(lca.Ancestor("b", "e"), "a");
  EXPECT_EQ(lca.Ancestor("b", "c"), "a");
  EXPECT_EQ(
      CalculateLCA(graph, std::string("a"), std::string("e"), std::string("g")),
      std::string("c"));
  EXPECT_EQ(
      CalculateLCA(graph, std::string("a"), std::string("b"), std::string("e")),
      std::string("a"));
}

TEST(LCATest, StringGraphUndirected_Complex2) {
  Graph<std::string> graph = Graph<std::string>::GraphFromStrs(
      {"a->b", "b->c", "b->d", "c->e", "c->f", "d->g", "d->h", "e->i", "f->j"});
  graph.MakeUndirected();
  LCA<std::string> lca(graph, "a");
  EXPECT_EQ(lca.Ancestor("i", "j"), "c");
  EXPECT_EQ(lca.Ancestor("c", "g"), "b");
  EXPECT_EQ(lca.Ancestor("a", "j"), "a");
  EXPECT_EQ(
      CalculateLCA(graph, std::string("a"), std::string("i"), std::string("j")),
      std::string("c"));
  EXPECT_EQ(
      CalculateLCA(graph, std::string("a"), std::string("c"), std::string("g")),
      std::string("b"));
}
