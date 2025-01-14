#pragma once

#include <cmath>

#include "graph.hpp"

/**
 * @brief Класс для нахождения наименьшего общего предка (LCA) в дереве.
 *
 * @details Реализует алгоритм Фараха-Колтона и Бендера для нахождения LCA.
 *          Этот алгоритм сводит задачу LCA к задаче RMQ (Range Minimum Query)
 *          на специальном массиве, где разница между соседними элементами равна
 *          +1 или -1. Он обеспечивает поиск LCA за O(1) после препроцессинга за
 *          O(N), где N - количество вершин в дереве.
 *
 * @tparam vert_t: тип вершин.
 */
template <AllowedVertType vert_t>
class LCA {
 public:
  /**
   * @brief Инициализирует новый экземпляр LCA.
   *
   * @param graph: граф, для которого нужно найти LCA.
   * @param root: корень дерева.
   *
   * @throw `std::invalid_argument("LCA: there is no such root vertice in
   * graph.")`.
   */
  LCA(const Graph<vert_t, size_t>& graph, vert_t root)
      : graph_(graph), root_(root) {
    if (!graph.ContainsVert(root))
      throw std::invalid_argument(
          "LCA: there is no such root vertice in graph.");

    if (graph.EdgesAmount() && graph.VertsAmount() > 1) BuildLCA_();
  }

  /**
   * @brief Находит наименьшего общего предка двух вершин.
   *
   * @param left: первая вершина.
   * @param right: вторая вершина.
   *
   * @throw `std::invalid_argument("LCA: there is no such left vertice in
   * graph.")`.
   * @throw `std::invalid_argument("LCA: there is no such right vertice in
   * graph.")`.
   * @throw `std::logic_error("LCA: graph has no edges.")`.
   * @throw `std::logic_error("LCA: left vertice is not connected to root.")`.
   * @throw `std::logic_error("LCA: right vertice is not connected to root.")`.
   *
   * @return `vert_t`: наименьший общий предок вершин `left` и `right`.
   */
  vert_t Ancestor(const vert_t& left, const vert_t& right) const {
    if (!graph_.ContainsVert(left))
      throw std::invalid_argument(
          "LCA: there is no such left vertice in graph.");

    if (!graph_.ContainsVert(right))
      throw std::invalid_argument(
          "LCA: there is no such right vertice in graph.");

    if (left == right) return left;

    if (!graph_.EdgesAmount())
      throw std::logic_error("LCA: graph has no edges.");

    if (first_pos_in_euler_tour_.find(left) == first_pos_in_euler_tour_.end())
      throw std::logic_error("LCA: left vertice is not connected to root.");

    if (first_pos_in_euler_tour_.find(right) == first_pos_in_euler_tour_.end())
      throw std::logic_error("LCA: right vertice is not connected to root.");

    // позиции вершин в обходе Эйлера
    ssize_t l = first_pos_in_euler_tour_.at(left),
            r = first_pos_in_euler_tour_.at(right);

    if (l > r) std::swap(l, r);

    // номера блоков, содержащих позиции l и r.
    ssize_t block_l = l / block_, block_r = r / block_;

    // вершины в одном блоке
    if (block_l == block_r)
      return euler_tour_.at(LCAInBlock_(block_l, l % block_, r % block_));

    // LCA для концов отрезков блоков
    ssize_t ans1 = LCAInBlock_(block_l, l % block_, block_ - 1);
    ssize_t ans2 = LCAInBlock_(block_r, 0, r % block_);
    ssize_t ans = MinHeight_(ans1, ans2);

    // LCA для блоков между левым и правым
    if (block_l < block_r - 1) {
      ssize_t pw2 = log2_[block_r - block_l - 1];

      ssize_t ans3 = block_sparse_table_[block_l + 1][pw2];
      ssize_t ans4 = block_sparse_table_[block_r - (1 << pw2)][pw2];
      ans = MinHeight_(ans, MinHeight_(ans3, ans4));
    }

    return euler_tour_[ans];
  }

  /**
   * @brief Находит наименьшего общего предка пары вершин.
   *
   * @param pair: пара вершин.
   *
   * @throw `std::invalid_argument("LCA: there is no such left vertice in
   * graph.")`.
   * @throw `std::invalid_argument("LCA: there is no such right vertice in
   * graph.")`.
   * @throw `std::logic_error("LCA: graph has no edges.")`.
   * @throw `std::logic_error("LCA: left vertice is not connected to root.")`.
   * @throw `std::logic_error("LCA: right vertice is not connected to root.")`.
   *
   * @return `vert_t`: наименьший общий предок вершин `pair`.
   */
  vert_t Ancestor(const std::pair<vert_t, vert_t>& pair) const {
    return Ancestor(pair.first, pair.second);
  }

 private:
  /**
   * @brief Обход дерева в глубину (DFS) для построения обхода Эйлера.
   *
   * @details В процессе обхода заполняет `euler_tour_`, `vert_heights_` и
   * `first_pos_in_euler_tour_`, необходимые для алгоритма.
   *
   * @param curr_vert: текущая вершина.
   * @param curr_height: текущая высота вершины.
   */
  void DFS_(const vert_t& curr_vert, ssize_t curr_height) {
    // записываем высоту текущей вершины и первое вхождение в обходе Эйлера
    vert_heights_[curr_vert] = curr_height;
    first_pos_in_euler_tour_[curr_vert] = euler_tour_.size();

    // добавляем в сам обход Эйлера
    euler_tour_.push_back(curr_vert);

    // проходимся по всем смежным вершинам
    const auto neighbors = graph_.GetAdjList()[curr_vert];
    for (const auto& neighbor : neighbors)
      // если соседняя вершина еще не посещена, рекурсивно вызываем DFS
      if (vert_heights_.find(neighbor) == vert_heights_.end()) {
        DFS_(neighbor, curr_height + 1);
        euler_tour_.push_back(curr_vert);
      }
  }

  /**
   * @brief Вычисляет индекс элемента с наименьшей высотой.
   *
   * @param i: индекс первого.
   * @param j: индекс второго.
   *
   * @return `ssize_t`: индекс элемента с наименьшей высотой.
   */
  ssize_t MinHeight_(ssize_t i, ssize_t j) const {
    return vert_heights_.at(euler_tour_[i]) < vert_heights_.at(euler_tour_[j])
               ? i
               : j;
  }

  /**
   * @brief Выполняет препроцессинг для нахождения LCA.
   *
   * @details 1.  Выполняет обход DFS для создания `euler_tour_`,
   * `vert_heights_` и `first_pos_in_euler_tour_`.
   *          2.  Разбивает `euler_tour_` на блоки размером `0.5 * log2(N)`.
   *          3.  Строит разреженную таблицу `block_sparse_table_` для RMQ на
   * блоках.
   *          4.  Вычисляет хеши блоков `block_hash_` на основе высот вершин в
   * блоках.
   *          5.  Предвычисляет `RMQ` внутри каждого уникального блока в
   * `block_RMQ_`.
   *          6.  Предвычисляет значения логарифмов в `log2_`.
   */
  void BuildLCA_() {
    DFS_(root_, 0);
    // размер обхода Эйлера
    ssize_t size = euler_tour_.size();

    // размер блока и разбиение массива на блоки
    block_ = (std::floor(std::log2(size)) + 1) / 2;
    ssize_t blocks_amount = size / block_ + (size % block_ ? 1 : 0);

    // разреженная таблица для минимумов на блоках
    block_sparse_table_ = std::vector<std::vector<ssize_t>>(
        blocks_amount,
        std::vector<ssize_t>(std::floor(std::log2(size)) + 1, -1));

    // заполнение block_sparse_table_ для базовых блоков
    for (ssize_t i = 0, block_num = 0, j = 0; i < size; i++, j++) {
      if (j == block_) j = 0, block_num++;

      // минимальный элемент в блоке
      if (block_sparse_table_[block_num][0] == -1 ||
          MinHeight_(i, block_sparse_table_[block_num][0]) == i)
        block_sparse_table_[block_num][0] = i;
    }

    // дозаполнение block_sparse_table_ на основе уже вычисленных блоков
    for (ssize_t j = 1; j <= std::floor(std::log2(size)); j++)
      for (ssize_t i = 0; i < blocks_amount; i++) {
        ssize_t ni = i + (1 << (j - 1));

        if (ni >= blocks_amount)
          block_sparse_table_[i][j] = block_sparse_table_[i][j - 1];
        else
          block_sparse_table_[i][j] = MinHeight_(
              block_sparse_table_[i][j - 1], block_sparse_table_[ni][j - 1]);
      }

    // хеши блоков
    block_hash_.resize(blocks_amount, 0);
    for (ssize_t i = 0, block_num = 0, j = 0; i < size || j < block_;
         i++, j++) {
      if (j == block_) j = 0, block_num++;
      if (j > 0 && (i >= size || MinHeight_(i - 1, i) == i - 1))
        block_hash_[block_num] += 1 << (j - 1);
    }

    // максимальный хеш
    ssize_t max_hash = 0;
    for (ssize_t hash : block_hash_) max_hash = std::max(max_hash, hash);

    // структура для хранения результатов RMQ внутри каждого блока
    block_RMQ_ = std::vector<std::vector<std::vector<ssize_t>>>(
        max_hash + 1, std::vector<std::vector<ssize_t>>(
                          block_, std::vector<ssize_t>(block_, -1)));

    // RMQ внутри каждого типа блока
    for (ssize_t i = 0; i < blocks_amount; i++) {
      ssize_t id = block_hash_[i];

      if (block_RMQ_[id][0][0] != -1) continue;

      for (ssize_t l = 0; l < block_; ++l) {
        block_RMQ_[id][l][l] = l;

        for (ssize_t r = l + 1; r < block_; ++r) {
          block_RMQ_[id][l][r] = block_RMQ_[id][l][r - 1];

          if (i * block_ + r < size)
            block_RMQ_[id][l][r] =
                MinHeight_(i * block_ + block_RMQ_[id][l][r], i * block_ + r) -
                i * block_;
        }
      }
    }

    // логарифмы :)
    log2_.resize(size);
    for (ssize_t i = 0, j = 0; i < size; i++) {
      if (1 << (j + 1) <= i) j++;
      log2_[i] = j;
    }
  }

  /**
   * @brief Находит индекс элемента с наименьшей высотой в отрезке блока.
   *
   * @details Использует предвычисленные значения из `block_RMQ_`.
   *
   * @param block_num: номер блока.
   * @param l: левая граница отрезка.
   * @param r: правая граница отрезка.
   *
   * @return `ssize_t`: индекс минимума в отрезке.
   */
  ssize_t LCAInBlock_(ssize_t block_num, ssize_t l, ssize_t r) const {
    return block_RMQ_[block_hash_[block_num]][l][r] + block_num * block_;
  }

  const Graph<vert_t, size_t> graph_;

  /// @brief Корень дерева
  const vert_t root_;

  /// @brief Обход Эйлера (для вершин)
  std::vector<vert_t> euler_tour_;

  /// @brief Высота каждой вершины от корня
  std::unordered_map<vert_t, ssize_t> vert_heights_;

  /// @brief Первое вхождение вершины в обходе Эйлера
  std::unordered_map<vert_t, ssize_t> first_pos_in_euler_tour_;

  /// @brief Размер блока для RMQ (приблизительно 0.5 * log2(N)).
  ssize_t block_;

  /// @brief Разреженная таблица для поиска минимума на блоках.
  std::vector<std::vector<ssize_t>> block_sparse_table_;

  /// @brief Хэш каждого блока на основе разницы высот внутри блока.
  std::vector<ssize_t> block_hash_;

  /// @brief Предвычисленные значения для RMQ внутри каждого типа блока.
  std::vector<std::vector<std::vector<ssize_t>>> block_RMQ_;

  /// @brief Предвычисленные логарифмы для разреженной таблицы.
  std::vector<ssize_t> log2_;
};

/**
 * @brief Находит наименьшего общего предка двух вершин.
 *
 * @details Эта функция предназначена для однократного вычисления LCA. При
 * множественных запросах к одному и тому же графу рекомендуется использовать
 * класс `LCA`.
 *
 * @tparam vert_t: тип вершин.
 * @param graph: граф, для которого нужно найти LCA.
 * @param root: корень дерева.
 * @param left: первая вершина.
 * @param right: вторая вершина.
 *
 * @return `vert_t`: наименьший общий предок вершин `left` и `right`.
 */
template <AllowedVertType vert_t>
inline vert_t CalculateLCA(const Graph<vert_t, size_t>& graph, vert_t root,
                           vert_t left, vert_t right) {
  return LCA<vert_t>(graph, root).Ancestor(left, right);
}

/**
 * @brief Находит наименьшего общего предка двух вершин.
 *
 * @details Эта функция предназначена для однократного вычисления LCA. При
 * множественных запросах к одному и тому же графу рекомендуется использовать
 * класс `LCA`.
 *
 * @tparam vert_t: тип вершин.
 * @param graph: граф, для которого нужно найти LCA.
 * @param root: корень дерева.
 * @param pair: пара вершин.
 *
 * @return `vert_t`: наименьший общий предок вершин `pair`.
 */
template <AllowedVertType vert_t>
inline vert_t CalculateLCA(const Graph<vert_t, size_t>& graph, vert_t root,
                           const std::pair<vert_t, vert_t>& pair) {
  return LCA<vert_t>(graph, root).Ancestor(pair);
}