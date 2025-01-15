#include <gtest/gtest.h>

#include "bridge_guards.hpp"

TEST(BridgeGuardsTest, SimpleTest) {
  std::stringstream ss;
  ss << "1 1\n"
     << "1\n"
     << "1 1\n";

  std::stringstream output;

  Solution(ss, output);
  EXPECT_EQ(output.str(), "1\n");
}

TEST(BridgeGuardsTest, Test_1) {
  std::stringstream ss;
  ss << "4 3\n"
     << "4\n"
     << "1 3\n"
     << "1 2\n"
     << "2 1\n"
     << "2 3\n";

  std::stringstream output;

  Solution(ss, output);
  EXPECT_EQ(output.str(), "2\n");
}

TEST(BridgeGuardsTest, Test_2) {
  std::stringstream ss;
  ss << "4 3\n"
     << "4\n"
     << "1 3\n"
     << "1 2\n"
     << "2 2\n"
     << "4 1\n";

  std::stringstream output;

  Solution(ss, output);
  EXPECT_EQ(output.str(), "3\n");
}

TEST(BridgeGuardsTest, Test_3) {
  std::stringstream ss;
  ss << "4 4\n"
     << "4\n"
     << "1 1\n"
     << "2 2\n"
     << "3 3\n"
     << "4 4\n";

  std::stringstream output;

  Solution(ss, output);
  EXPECT_EQ(output.str(), "4\n");
}

TEST(BridgeGuardsTest, Test_4) {
  std::stringstream ss;
  ss << "5 2\n"
     << "5\n"
     << "1 1\n"
     << "2 1\n"
     << "3 1\n"
     << "4 2\n"
     << "5 2\n";

  std::stringstream output;

  Solution(ss, output);
  EXPECT_EQ(output.str(), "2\n");
}

TEST(BridgeGuardsTest, HardTest) {
  std::stringstream ss;
  ss << "10 6\n"
     << "12\n"
     << "1 1\n"
     << "2 1\n"
     << "3 1\n"
     << "4 3\n"
     << "5 3\n"
     << "6 2\n"
     << "7 3\n"
     << "8 3\n"
     << "9 4\n"
     << "9 5\n"
     << "10 5\n"
     << "10 6\n";

  std::stringstream output;

  Solution(ss, output);
  EXPECT_EQ(output.str(), "5\n");
}

TEST(BridgeGuardsTest, EmptyGraph) {
  std::stringstream ss;
  ss << "0 0\n"  // 0 вершин в обеих долях
     << "0\n";   // 0 мостов

  std::stringstream output;

  Solution(ss, output);
  EXPECT_EQ(output.str(), "0\n");
}

TEST(BridgeGuardsTest, NoEdges) {
  std::stringstream ss;
  ss << "5 3\n"  //  вершин в первой доле, 3 во второй
     << "0\n";   // 0 мостов

  std::stringstream output;

  Solution(ss, output);
  EXPECT_EQ(output.str(), "0\n");
}

TEST(BridgeGuardsTest, OneSidedGraph) {
  std::stringstream ss;
  ss << "5 0\n"  // 5 вершин в первой доле, 0 во второй
     << "0\n";   // 0 мостов

  std::stringstream output;

  Solution(ss, output);
  EXPECT_EQ(output.str(), "0\n");
}

TEST(BridgeGuardsTest, OneSidedGraph2) {
  std::stringstream ss;
  ss << "0 3\n"  // 0 вершин в первой доле, 3 во второй
     << "0\n";   // 0 мостов

  std::stringstream output;

  Solution(ss, output);
  EXPECT_EQ(output.str(), "0\n");
}

TEST(BridgeGuardsTest, LargeGraphSmallEdges) {
  std::stringstream ss;
  ss << "100 100\n"  // 100 вершин в каждой доле
     << "5\n"        // 5 мостов
     << "1 1\n"
     << "2 2\n"
     << "3 3\n"
     << "4 4\n"
     << "5 5\n";

  std::stringstream output;

  Solution(ss, output);
  EXPECT_EQ(output.str(), "5\n");
}

TEST(BridgeGuardsTest, CompleteBipartiteGraph) {
  std::stringstream ss;
  ss << "3 3\n"
     << "9\n"
     << "1 1\n"
     << "1 2\n"
     << "1 3\n"
     << "2 1\n"
     << "2 2\n"
     << "2 3\n"
     << "3 1\n"
     << "3 2\n"
     << "3 3\n";

  std::stringstream output;

  Solution(ss, output);
  EXPECT_EQ(output.str(), "3\n");
}

TEST(BridgeGuardsTest, StarShapedGraph) {
  std::stringstream ss;
  ss << "5 1\n"
     << "5\n"
     << "1 1\n"
     << "2 1\n"
     << "3 1\n"
     << "4 1\n"
     << "5 1\n";

  std::stringstream output;
  Solution(ss, output);
  EXPECT_EQ(output.str(), "1\n");
}

TEST(BridgeGuardsTest, StarShapedGraph2) {
  std::stringstream ss;
  ss << "1 5\n"
     << "5\n"
     << "1 1\n"
     << "1 2\n"
     << "1 3\n"
     << "1 4\n"
     << "1 5\n";

  std::stringstream output;
  Solution(ss, output);
  EXPECT_EQ(output.str(), "1\n");
}

TEST(BridgeGuardsTest, DisconnectedGraph) {
  std::stringstream ss;
  ss << "6 6\n"
     << "4\n"
     << "1 1\n"
     << "2 2\n"
     << "3 3\n"
     << "4 4\n";

  std::stringstream output;
  Solution(ss, output);
  EXPECT_EQ(output.str(), "4\n");
}

TEST(BridgeGuardsTest, Test_5) {
  std::stringstream ss;
  ss << "3 2\n"
     << "4\n"
     << "1 1\n"
     << "1 2\n"
     << "2 1\n"
     << "3 1\n";

  std::stringstream output;
  Solution(ss, output);
  EXPECT_EQ(output.str(), "2\n");
}

TEST(BridgeGuardsTest, Test_6) {
  std::stringstream ss;
  ss << "2 3\n"
     << "4\n"
     << "1 1\n"
     << "1 2\n"
     << "2 2\n"
     << "2 3\n";

  std::stringstream output;
  Solution(ss, output);
  EXPECT_EQ(output.str(), "2\n");
}
