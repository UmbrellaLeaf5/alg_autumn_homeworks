#include <gtest/gtest.h>

#include <random>

#include "rmq.hpp"

TEST(RMQTest, IntArray) {
  std::vector<int> arr = {5, 2, 8, 1, 9, 4};
  RMQ<int> rmq(arr);

  EXPECT_EQ(rmq.Query(0, 5), 1);
  EXPECT_EQ(rmq.Query(1, 3), 1);
  EXPECT_EQ(rmq.Query(2, 4), 1);
  EXPECT_EQ(rmq.Query(0, 1), 2);
  EXPECT_EQ(rmq.Query(4, 5), 4);
  EXPECT_EQ(CalculateRMQ(arr, 0, 5), 1);
  EXPECT_EQ(CalculateRMQ(arr, std::make_pair(1, 3)), 1);
}

TEST(RMQTest, IntArraySameElements) {
  std::vector<int> arr = {5, 5, 5, 5, 5};
  RMQ<int> rmq(arr);

  EXPECT_EQ(rmq.Query(0, 4), 5);
  EXPECT_EQ(rmq.Query(1, 3), 5);
  EXPECT_EQ(CalculateRMQ(arr, 0, 4), 5);
  EXPECT_EQ(CalculateRMQ(arr, std::make_pair(1, 3)), 5);
}

TEST(RMQTest, IntArraySingleElement) {
  std::vector<int> arr = {7};

  EXPECT_THROW((RMQ<int>(arr)), std::invalid_argument);
  EXPECT_THROW(CalculateRMQ(arr, 0, 0), std::invalid_argument);
  EXPECT_THROW(CalculateRMQ(arr, std::make_pair(0, 0)), std::invalid_argument);
}

TEST(RMQTest, IntArrayReversed) {
  std::vector<int> arr = {9, 8, 7, 6, 5, 4, 3, 2, 1};
  RMQ<int> rmq(arr);

  EXPECT_EQ(rmq.Query(0, 8), 1);
  EXPECT_EQ(rmq.Query(2, 5), 4);
  EXPECT_EQ(rmq.Query(0, 3), 6);
  EXPECT_EQ(CalculateRMQ(arr, 0, 8), 1);
  EXPECT_EQ(CalculateRMQ(arr, std::make_pair(2, 5)), 4);
}

TEST(RMQTest, IntArrayOutOfBound) {
  std::vector<int> arr = {1, 2, 3, 4, 5};
  RMQ<int> rmq(arr);

  EXPECT_THROW(rmq.Query(0, 6), std::invalid_argument);
  EXPECT_THROW(rmq.Query(6, 4), std::invalid_argument);
  EXPECT_THROW(CalculateRMQ(arr, 0, 6), std::invalid_argument);
  EXPECT_THROW(CalculateRMQ(arr, std::make_pair(6, 4)), std::invalid_argument);
}

// Тесты для RMQ с массивом char
TEST(RMQTest, CharArray) {
  std::vector<char> arr = {'b', 'a', 'c', 'd', 'e'};
  RMQ<char> rmq(arr);

  EXPECT_EQ(rmq.Query(0, 4), 'a');
  EXPECT_EQ(rmq.Query(1, 2), 'a');
  EXPECT_EQ(rmq.Query(0, 1), 'a');
  EXPECT_EQ(CalculateRMQ(arr, 0, 4), 'a');
  EXPECT_EQ(CalculateRMQ(arr, std::make_pair(1, 2)), 'a');
}

// Тесты для RMQ с массивом size_t
TEST(RMQTest, SizeTArray) {
  std::vector<size_t> arr = {10, 5, 12, 3, 15, 8};
  RMQ<size_t> rmq(arr);

  EXPECT_EQ(rmq.Query(0, 5), 3);
  EXPECT_EQ(rmq.Query(1, 3), 3);
  EXPECT_EQ(rmq.Query(2, 4), 3);
  EXPECT_EQ(rmq.Query(0, 1), 5);
  EXPECT_EQ(rmq.Query(4, 5), 8);
  EXPECT_EQ(CalculateRMQ(arr, 0, 5), 3);
  EXPECT_EQ(CalculateRMQ(arr, std::make_pair(1, 3)), 3);
}

// Тесты для RMQ с массивом строк (std::string)
TEST(RMQTest, StringArray) {
  std::vector<std::string> arr = {"zebra", "apple", "banana", "cat"};
  RMQ<std::string> rmq(arr);

  EXPECT_EQ(rmq.Query(0, 3), "apple");
  EXPECT_EQ(rmq.Query(1, 2), "apple");
  EXPECT_EQ(rmq.Query(2, 3), "banana");
  EXPECT_EQ(CalculateRMQ(arr, 0, 3), "apple");
  EXPECT_EQ(CalculateRMQ(arr, std::make_pair(1, 2)), "apple");
}

TEST(RMQTest, StringArraySameElements) {
  std::vector<std::string> arr = {"test", "test", "test", "test"};
  RMQ<std::string> rmq(arr);

  EXPECT_EQ(rmq.Query(0, 3), "test");
  EXPECT_EQ(rmq.Query(1, 2), "test");
  EXPECT_EQ(CalculateRMQ(arr, 0, 3), "test");
  EXPECT_EQ(CalculateRMQ(arr, std::make_pair(1, 2)), "test");
}

// Тест для пустого массива
TEST(RMQTest, EmptyArray) {
  std::vector<int> arr;

  EXPECT_THROW((RMQ<int>(arr)), std::invalid_argument);
  EXPECT_THROW(CalculateRMQ(arr, 0, 0), std::logic_error);
  EXPECT_THROW(CalculateRMQ(arr, std::make_pair(0, 0)), std::logic_error);
}

std::vector<int> GenerateRandomIntArray(size_t size, int min_val, int max_val) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> distribution(min_val, max_val);

  std::vector<int> arr(size);
  std::generate(arr.begin(), arr.end(), [&] { return distribution(gen); });
  return arr;
}

// Функция для генерации случайного массива строк
std::vector<std::string> GenerateRandomStringArray(size_t size,
                                                   size_t str_len) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> distribution(97, 122);  // ASCII a-z

  std::vector<std::string> arr(size);

  for (size_t i = 0; i < size; ++i) {
    std::string str(str_len, ' ');

    std::generate(str.begin(), str.end(),
                  [&] { return static_cast<char>(distribution(gen)); });

    arr[i] = str;
  }

  return arr;
}

// Функция для проверки RMQ с большим массивом целых чисел
std::vector<int> TestLargeIntArray(size_t num_queries, size_t size, int min_val,
                                   int max_val) {
  std::vector<int> arr = GenerateRandomIntArray(size, min_val, max_val);
  RMQ<int> rmq(arr);

  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> distribution(0, size - 1);

  for (size_t i = 0; i < num_queries; ++i) {
    size_t left = distribution(gen);
    size_t right = distribution(gen);

    if (left > right) std::swap(left, right);

    int min_val_in_range = arr[left];

    for (size_t k = left + 1; k <= right; ++k)
      if (arr[k] < min_val_in_range) min_val_in_range = arr[k];

    EXPECT_EQ(rmq.Query(left, right), min_val_in_range);
  }

  return arr;
}

// Функция для проверки RMQ с большим массивом строк
std::vector<std::string> TestLargeStringArray(size_t num_queries, size_t size,
                                              size_t str_len) {
  std::vector<std::string> arr = GenerateRandomStringArray(size, str_len);
  RMQ<std::string> rmq(arr);

  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> distribution(0, size - 1);

  for (size_t i = 0; i < num_queries; ++i) {
    size_t left = distribution(gen);
    size_t right = distribution(gen);

    if (left > right) std::swap(left, right);

    std::string min_val_in_range = arr[left];

    for (size_t k = left + 1; k <= right; ++k)
      if (arr[k] < min_val_in_range) min_val_in_range = arr[k];

    EXPECT_EQ(rmq.Query(left, right), min_val_in_range);
  }

  return arr;
}

// Тест для RMQ с большим массивом целых чисел
TEST(RMQTest, LargeIntArray) {
  TestLargeIntArray(10, 10, -10, 10);
  TestLargeIntArray(100, 100, -100, 100);
  TestLargeIntArray(1000, 1000, -1000, 1000);
  TestLargeIntArray(10000, 10000, -10000, 10000);
}

// Тест для RMQ с большим массивом строк
TEST(RMQTest, LargeStringArray) {
  TestLargeStringArray(10, 10, 10);
  TestLargeStringArray(100, 100, 100);
  TestLargeStringArray(1000, 1000, 1000);
  TestLargeStringArray(10000, 10000, 10000);
}
