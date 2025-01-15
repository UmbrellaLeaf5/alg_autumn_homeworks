#pragma once

#include "euler_path.hpp"

/**
 * @brief Проверяет, можно ли составить цепочку из заданных слов так, чтобы
 * последняя буква каждого слова совпадала с первой буквой следующего.
 *
 * @param words вектор строк (слов), которые нужно проверить.
 *
 * @return `true`: слова можно объединить в цепочку
 * @return `false`: в противном случае
 */
bool CanBeChained(const std::vector<std::string>& words) {
  if (words.empty()) return true;

  Graph<char> chars_graph;

  for (const std::string& s : words) {
    if (s.empty()) continue;  // пропускаем пустые строки

    chars_graph.AddEdge({s[0], s.back()});
  }

  // дополнительная проверка на связность:
  if (chars_graph.VertsAmount() > 0 && chars_graph.EdgesAmount() == 0)
    return false;  // есть изолированная вершина

  // проверка на Эйлеров цикл
  return HasEulerPath(chars_graph);
}

/// @brief Решает задачу: ""
void Solution(std::istream& is = std::cin, std::ostream& os = std::cout) {
  size_t words_amount;
  is >> words_amount;

  std::vector<std::string> words(words_amount);
  for (auto& word : words) is >> word;

  if (CanBeChained(words))
    os << "Yes";
  else
    os << "No";

  os << std::endl;
}
