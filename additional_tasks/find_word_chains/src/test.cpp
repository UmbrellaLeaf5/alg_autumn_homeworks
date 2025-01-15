#include <gtest/gtest.h>

#include "find_word_chains.hpp"

TEST(CanBeChainedTest, EmptyInput) {
  std::vector<std::string> words = {};
  ASSERT_TRUE(CanBeChained(words));
}

TEST(CanBeChainedTest, SingleWord) {
  std::vector<std::string> words = {"abc"};
  ASSERT_TRUE(CanBeChained(words));
}

TEST(CanBeChainedTest, SimpleChain) {
  std::vector<std::string> words = {"for", "rig", "geek", "kaf"};
  ASSERT_TRUE(CanBeChained(words));
}

TEST(CanBeChainedTest, SimpleChain2) {
  std::vector<std::string> words = {"abc", "cba"};
  ASSERT_TRUE(CanBeChained(words));
}

TEST(CanBeChainedTest, NoChain2) {
  std::vector<std::string> words = {"abc", "def"};
  ASSERT_FALSE(CanBeChained(words));
}

TEST(CanBeChainedTest, SameStartEnd) {
  std::vector<std::string> words = {"aba", "cba", "acc"};
  ASSERT_TRUE(CanBeChained(words));
}

TEST(CanBeChainedTest, MultipleSameWords) {
  std::vector<std::string> words = {"aba", "aba", "aca"};
  ASSERT_TRUE(CanBeChained(words));
}

TEST(CanBeChainedTest, WordsWithSameStartEnd) {
  std::vector<std::string> words = {"aaa", "bbb", "ccc"};
  ASSERT_FALSE(CanBeChained(words));
}

TEST(CanBeChainedTest, MultipleWordsWithSameStartEnd) {
  std::vector<std::string> words = {"aaa", "aba", "aca"};
  ASSERT_TRUE(CanBeChained(words));
}

TEST(CanBeChainedTest, MultipleWordsWithSameStartEnd2) {
  std::vector<std::string> words = {"aaa", "abb", "aba"};
  ASSERT_TRUE(CanBeChained(words));
}

TEST(CanBeChainedTest, ComplexChain) {
  std::vector<std::string> words = {"aba", "bab", "bba"};
  ASSERT_FALSE(CanBeChained(words));
}

TEST(CanBeChainedTest, ComplexChain2) {
  std::vector<std::string> words = {"abc", "cde", "efg", "gba"};
  ASSERT_TRUE(CanBeChained(words));
}

TEST(CanBeChainedTest, ComplexNoChain) {
  std::vector<std::string> words = {"abc", "cde", "efg", "hbl"};
  ASSERT_FALSE(CanBeChained(words));
}

TEST(CanBeChainedTest, OneWordMultipleSameLetters) {
  std::vector<std::string> words = {"aaaa"};
  ASSERT_TRUE(CanBeChained(words));
}

TEST(CanBeChainedTest, TwoWordsSameStartEnd) {
  std::vector<std::string> words = {"aaa", "bbb"};
  ASSERT_FALSE(CanBeChained(words));
}

TEST(CanBeChainedTest, EmptyWords) {
  std::vector<std::string> words = {"abc", "", "def"};
  ASSERT_FALSE(CanBeChained(words));
}

TEST(CanBeChainedTest, EmptyWordsOnly) {
  std::vector<std::string> words = {"", "", ""};
  ASSERT_TRUE(CanBeChained(words));
}

TEST(CanBeChainedTest, LongChain) {
  std::vector<std::string> words = {"a", "ab", "bc", "cd", "de", "ef", "fa"};
  ASSERT_TRUE(CanBeChained(words));
}