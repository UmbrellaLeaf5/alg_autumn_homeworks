#pragma once

#include "implicit_treap.hpp"
#include "lca.hpp"

/**
 * @brief Класс для выполнения RMQ на массиве.
 *
 * @details Реализует RMQ, используя сведение к задаче LCA (Lowest Common
 * Ancestor). Для этого массив преобразуется в декартово дерево, а затем
 * используется алгоритм LCA для нахождения минимума на заданном диапазоне.
 *
 * @tparam vert_t: тип элементов массива. Должен соответствовать требованиям
 * `AllowedVertType`.
 */
template <AllowedVertType vert_t>
class RMQ {
 public:
  /**
   * @brief Конструктор RMQ.
   *
   * @details Создает объект RMQ на основе заданного массива. Строит неявное
   * декартово дерево из массива и инициализирует LCA для этого дерева.
   *
   * @param arr: входной массив для RMQ.
   *
   * @throw `std::invalid_argument("RMQ: array should have at least 2
   * elemets")`.
   */
  RMQ(const std::vector<vert_t>& arr)
      : arr_(arr),
        lca_(ImplicitIndexTreap<vert_t>(arr).GetGraph(),
             ImplicitIndexTreap<vert_t>(arr).Root()) {
    if (arr_.size() <= 1)
      throw std::invalid_argument("RMQ: array should have at least 2 elemets");
  }

  /**
   * @brief Выполняет запрос RMQ на заданном диапазоне массива.
   *
   * @details Возвращает значение наименьшего элемента в массиве в диапазоне
   * [left, right]. Использует ранее построенное декартово дерево и алгоритм
   * LCA.
   *
   * @param left: левая граница диапазона.
   * @param right: правая граница диапазона.
   *
   * @return `vert_t`: значение наименьшего элемента в диапазоне `[left,
   * right]`.
   *
   * @throw `std::invalid_argument("RMQ: left index is out of bound.")`.
   * @throw `std::invalid_argument("RMQ: right index is out of bound.")`.
   */
  vert_t Query(size_t left, size_t right) const {
    if (left >= arr_.size())
      throw std::invalid_argument("RMQ: left index is out of bound.");

    if (right >= arr_.size())
      throw std::invalid_argument("RMQ: right index is out of bound.");

    if (left > right) std::swap(left, right);

    return arr_[lca_.Ancestor(left, right)];
  }

  /**
   * @brief Выполняет запрос RMQ на заданном диапазоне массива.
   *
   * @details Возвращает значение наименьшего элемента в массиве в диапазоне
   * `[pair.first, pair.second]`. Использует ранее построенное декартово дерево
   * и алгоритм LCA.
   *
   * @param pair: пара индексов, определяющая границы диапазона.
   *
   * @return `vert_t`: значение наименьшего элемента в диапазоне.
   *
   * @throw `std::invalid_argument("RMQ: array should have at least 2
   * elemets")`.
   * @throw `std::invalid_argument("RMQ: left index is out of bound.")`.
   * @throw `std::invalid_argument("RMQ: right index is out of bound.")`.
   */
  vert_t Query(const std::pair<size_t, size_t>& pair) const {
    return Query(pair.first, pair.second);
  }

 private:
  /// @brief Ссылка на исходный массив.
  const std::vector<vert_t>& arr_;

  /// @brief Объект LCA для использования в запросах RMQ.
  LCA<size_t> lca_;
};

/**
 * @brief Выполняет запрос RMQ на заданном диапазоне массива. (Внешняя функция)
 *
 * @details Создает объект RMQ и выполняет запрос.
 *
 * @param arr: входной массив для RMQ.
 * @param left: левая граница диапазона.
 * @param right: правая граница диапазона.
 *
 * @return `vert_t`: значение наименьшего элемента в диапазоне [left, right].
 *
 * @throw `std::invalid_argument("RMQ: array should have at least 2 elemets")`.
 * @throw `std::invalid_argument("RMQ: left index is out of bound.")`.
 * @throw `std::invalid_argument("RMQ: right index is out of bound.")`.
 */
template <AllowedVertType vert_t>
inline vert_t CalculateRMQ(const std::vector<vert_t>& arr, size_t left,
                           size_t right) {
  return RMQ<vert_t>(arr).Query(left, right);
}

/**
 * @brief Выполняет запрос RMQ на заданном диапазоне массива. (Внешняя функция)
 *
 * @details Создает объект RMQ и выполняет запрос.
 *
 * @param arr: входной массив для RMQ.
 * @param pair: пара индексов, определяющая границы диапазона.
 *
 * @return `vert_t`: значение наименьшего элемента в диапазоне.
 *
 * @throw `std::invalid_argument("RMQ: array should have at least 2 elemets")`.
 * @throw `std::invalid_argument("RMQ: left index is out of bound.")`.
 * @throw `std::invalid_argument("RMQ: right index is out of bound.")`.
 */
template <AllowedVertType vert_t>
inline vert_t CalculateRMQ(const std::vector<vert_t>& arr,
                           const std::pair<size_t, size_t>& pair) {
  return RMQ<vert_t>(arr).Query(pair);
}
